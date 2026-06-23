#include "http_client.hpp"

#include <cstring>
#include <sstream>

#include "logger.hpp"
#include "platform.hpp"

std::optional<std::string> http_get(
   const std::string &host, int port, const std::string &path, const std::string &json_header)
{
   struct addrinfo hints
   {
   };
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

   if (connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
      closesocket(sock);
      freeaddrinfo(res);
      Logger::error("connect failed to: ", host, ":", port);
      return std::nullopt;
   }
   freeaddrinfo(res);

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
