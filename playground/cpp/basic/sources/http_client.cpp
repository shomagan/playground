#include "http_client.hpp"

#include <cstring>
#include <sstream>

#include "logger.hpp"
#include "platform.hpp"

#ifdef _WIN32
static void set_nonblocking(SOCKET sock, bool enable) {
   u_long mode = enable ? 1 : 0;
   ioctlsocket(sock, FIONBIO, &mode);
}
#else
#include <fcntl.h>
#include <sys/select.h>

static void set_nonblocking(SOCKET sock, bool enable) {
   int flags = fcntl(sock, F_GETFL, 0);
   fcntl(sock, F_SETFL, enable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK));
}
#endif

static bool connect_with_timeout(SOCKET sock, const struct sockaddr *addr, socklen_t addrlen, int timeout_ms) {
   set_nonblocking(sock, true);
   if (connect(sock, addr, addrlen) == 0) {
      set_nonblocking(sock, false);
      return true;
   }

#ifdef _WIN32
   if (WSAGetLastError() != WSAEWOULDBLOCK)
      return false;
#else
   if (errno != EINPROGRESS)
      return false;
#endif

   fd_set writefds;
   FD_ZERO(&writefds);
   FD_SET(sock, &writefds);
   struct timeval tv{ timeout_ms / 1000, (timeout_ms % 1000) * 1000 };

   int result = select(static_cast<int>(sock) + 1, nullptr, &writefds, nullptr, &tv);
   set_nonblocking(sock, false);
   if (result <= 0)
      return false;

   int err = 0;
   socklen_t len = sizeof(err);
   getsockopt(sock, SOL_SOCKET, SO_ERROR, reinterpret_cast<char *>(&err), &len);
   return err == 0;
}

static void set_io_timeout(SOCKET sock, int timeout_ms) {
#ifdef _WIN32
   DWORD timeout = static_cast<DWORD>(timeout_ms);
   setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&timeout), sizeof(timeout));
   setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&timeout), sizeof(timeout));
#else
   struct timeval tv{ timeout_ms / 1000, (timeout_ms % 1000) * 1000 };
   setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&tv), sizeof(tv));
   setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&tv), sizeof(tv));
#endif
}

std::optional<std::string> http_get(const std::string &host, int port, const std::string &path, const std::string &json_header,
                                    int timeout_ms) {
   struct addrinfo hints{};
   struct addrinfo *res = nullptr;
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;

   if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res) != 0) {
      Logger::error("getaddrinfo failed for host: ", host);
      return std::nullopt;
   }

   SOCKET sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
   if (sock == INVALID_SOCKET) {
      freeaddrinfo(res);
      return std::nullopt;
   }

   if (!connect_with_timeout(sock, res->ai_addr, static_cast<socklen_t>(res->ai_addrlen), timeout_ms)) {
      closesocket(sock);
      freeaddrinfo(res);
      Logger::error("connect failed to: ", host, ":", port);
      return std::nullopt;
   }
   freeaddrinfo(res);

   set_io_timeout(sock, timeout_ms);

   std::ostringstream req;
   req << "GET " << path << " HTTP/1.0\r\n"
       << "Host: " << host << "\r\n"
       << "Json: " << json_header << "\r\n"
       << "Connection: close\r\n\r\n";

   std::string request = req.str();
   if (send(sock, request.c_str(), static_cast<int>(request.size()), 0) == SOCKET_ERROR) {
      closesocket(sock);
      return std::nullopt;
   }

   std::string response;
   char buf[4096];
   int bytes;
   while ((bytes = recv(sock, buf, sizeof(buf) - 1, 0)) > 0) {
      buf[bytes] = '\0';
      response += buf;
   }
   closesocket(sock);

   size_t header_end = response.find("\r\n\r\n");
   if (header_end != std::string::npos)
      return response.substr(header_end + 4);
   return response;
}
