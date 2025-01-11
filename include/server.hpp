#include <condition_variable>
#include <string_view>

#include <cstdint>

#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/detail/thread_group.hpp>


#include <netinet/in.h>

namespace hyperhttpd {
  class server {
  public:
    server(std::uint16_t port, std::uint32_t workers);

    void listen();

    constexpr inline bool failed() { return m_failed; }
  private:
    std::uint16_t m_port;
    std::uint32_t m_worker_amount;

    boost::thread_group m_threadpool;

    int m_socket;
    sockaddr_in m_server_addr;

    bool m_failed;

    std::condition_variable m_cv;
    std::mutex m_mtx;
    std::queue<int> m_socket_queue;

    void worker_thread();
  };
}