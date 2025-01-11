#include <mutex>
#include <server.hpp>
#include <boost/thread.hpp>

#include <signal.h>
#include <sys/socket.h>

#include <iostream>

hyperhttpd::server::server(std::uint16_t port, std::uint32_t workers) : m_port(port), m_worker_amount(workers) {
  m_socket = socket(AF_INET, SOCK_STREAM, 0);
  m_server_addr.sin_family = AF_INET;
  m_server_addr.sin_addr.s_addr = INADDR_ANY;
  m_server_addr.sin_port = htons(m_port);

  if (bind(m_socket, reinterpret_cast<struct sockaddr*>(&m_server_addr), sizeof(m_server_addr))) {
    m_failed = true;
  }
  for (std::uint32_t i = 0; i < workers; ++i) {
    m_threadpool.create_thread(std::bind(&server::worker_thread, this));
  }

  signal(SIGPIPE, SIG_IGN);
}

void hyperhttpd::server::listen() {
  ::listen(m_socket, 10);

  while (true) {
    int client_socket = accept(m_socket, nullptr, nullptr);
    if (client_socket >= 0) {
      m_socket_queue.push(client_socket);
      m_cv.notify_one();
    }
  }
}

void hyperhttpd::server::worker_thread() {
  while (true) {
    int client_socket;
    {
      std::unique_lock<std::mutex> lock(m_mtx);
      m_cv.wait(lock, [this](){ return !m_socket_queue.empty(); });
      client_socket = m_socket_queue.front();
      m_socket_queue.pop();
    }
    
    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    int bytes = send(client_socket, response, strlen(response), MSG_NOSIGNAL);

    close(client_socket);
  }
}