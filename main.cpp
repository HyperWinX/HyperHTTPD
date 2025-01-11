#include <server.hpp>
#include <stdexcept>

#include <cerrno>

int main() {
  hyperhttpd::server srvr(30500, 6);

  if (!srvr.failed()) {
    int num = errno;
    throw std::runtime_error("Failed to initialize!");
  }

  srvr.listen();
}