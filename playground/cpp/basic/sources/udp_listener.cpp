#include "udp_listener.hpp"

#include <cstring>
#include <string>

#include <nlohmann/json.hpp>

#include "device_updater.hpp"
#include "logger.hpp"
#include "platform.hpp"

using json = nlohmann::json;

static const int UDP_PORT = 65500;
static const std::string UDP_REQUEST_MSG = "UDP_REQUEST";

static SOCKET create_udp_socket() {
   SOCKET socket_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (socket_udp == INVALID_SOCKET) {
      return INVALID_SOCKET;
   }
   int broadcast = 1;
   setsockopt(socket_udp, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char *>(&broadcast), sizeof(broadcast));
   struct sockaddr_in addr{};
   addr.sin_family = AF_INET;
   addr.sin_port = htons(UDP_PORT);
   addr.sin_addr.s_addr = INADDR_ANY;
   if (bind(socket_udp, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) != 0) {
      closesocket(socket_udp);
      return INVALID_SOCKET;
   }

#ifdef _WIN32
   DWORD recv_timeout = 250;
   setsockopt(socket_udp, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&recv_timeout), sizeof(recv_timeout));
#else
   struct timeval tv{ 0, 250000 };
   setsockopt(socket_udp, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&tv), sizeof(tv));
#endif

   return socket_udp;
}

static void send_broadcast(SOCKET socket_udp) {
   struct sockaddr_in addr{};
   addr.sin_family = AF_INET;
   addr.sin_port = htons(UDP_PORT);
   addr.sin_addr.s_addr = INADDR_BROADCAST;

   sendto(socket_udp, UDP_REQUEST_MSG.c_str(), static_cast<int>(UDP_REQUEST_MSG.size()), 0, reinterpret_cast<struct sockaddr *>(&addr),
          sizeof(addr));
}

static bool is_timeout() {
#ifdef _WIN32
   return WSAGetLastError() == WSAETIMEDOUT;
#else
   return errno == EAGAIN || errno == EWOULDBLOCK;
#endif
}

void udp_listener(std::shared_ptr<SharedState> state) {
   SOCKET socket_udp = create_udp_socket();
   if (socket_udp == INVALID_SOCKET) {
      Logger::error("Failed to create UDP socket");
      return;
   }

   Logger::info("start listening");
   int packet_number = 0;
   Description local_desc;
   {
      std::lock_guard<std::mutex> lock(state->mtx);
      local_desc = state->desc;
   }

   while (true) {
      {
         std::lock_guard<std::mutex> lock(state->mtx);
         if (!state->running) {
            break;
         }
         if (state->desc_updated) {
            local_desc = state->desc;
            state->desc_updated = false;
         }
      }

      char buf[1024];
      struct sockaddr_in sender{};
      socklen_t sender_len = sizeof(sender);
      int bytes = recvfrom(socket_udp, buf, sizeof(buf) - 1, 0, reinterpret_cast<struct sockaddr *>(&sender), &sender_len);

      if (bytes <= 0) {
         if (is_timeout()){
            send_broadcast(socket_udp);
         }
         continue;
      }
      //Logger::info("Received UDP packet from ", inet_ntoa(sender.sin_addr), ":", ntohs(sender.sin_port));
      buf[bytes] = '\0';
      std::string data(buf, static_cast<size_t>(bytes));

      if (data == UDP_REQUEST_MSG){
         continue;
      }

      packet_number++;
      Logger::info("Address: ", inet_ntoa(sender.sin_addr), ", Packet: ", packet_number);
      Logger::debug("Data: ", data);

      size_t brace_end = data.find('}');
      if (brace_end != std::string::npos){
         data = data.substr(0, brace_end + 1);
      }

      Logger::debug("Data reduced: ", data);

      try {
         json j = json::parse(data);
         if (!j.contains("device_name")){
            continue;
         }
         std::string dev_name = j["device_name"];
         for (const auto &device : local_desc.devices) {
            if (device.device_name == dev_name) {
               Logger::info("device_name - ", dev_name);
               update_vars(inet_ntoa(sender.sin_addr), device);
            }
         }
      } catch (const std::exception &e) {
         Logger::error("JSON parse error: ", e.what());
      }
   }

   closesocket(socket_udp);
}
