#include <chrono>
#include <filesystem>
#include <memory>
#include <thread>

#include "config.hpp"
#include "keyboard.hpp"
#include "logger.hpp"
#include "platform.hpp"
#include "udp_listener.hpp"

static const std::string DEFAULT_DESC_FILE = "devices_description.json";
static const std::string LOG_DIR = "data_log";

static std::pair<std::string, std::string> parse_args(int argc, const char *argv[])
{
   std::string desc_file = DEFAULT_DESC_FILE;
   std::string log_level = "INFO";
   for (int i = 1; i < argc - 1; i++) {
      std::string arg = argv[i];
      if (arg == "-d")
         desc_file = argv[i + 1];
      else if (arg == "-l")
         log_level = argv[i + 1];
   }
   return {desc_file, log_level};
}

int main(int argc, const char *argv[])
{
#ifdef _WIN32
   WSADATA wsa;
   WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

   auto [desc_file, log_level] = parse_args(argc, argv);
   Logger::set_level(log_level);

   auto shared = std::make_shared<SharedState>();
   if (!load_description(desc_file, shared->desc))
      return 1;

   Logger::info(shared->desc.devices.size(), " devices loaded");

   if (!std::filesystem::exists(LOG_DIR))
      std::filesystem::create_directory(LOG_DIR);

   auto listener_thread = std::thread(udp_listener, shared);
   auto last_check = std::chrono::steady_clock::now();

   while (true) {
      std::string key = get_ch();
      if (!key.empty()) {
         Logger::debug("key: ", key);
         if (key == "q") {
            Logger::info("exit");
            {
               std::lock_guard<std::mutex> lock(shared->mtx);
               shared->running = false;
            }
            break;
         }
      }

      double elapsed = std::chrono::duration<double>(
                          std::chrono::steady_clock::now() - last_check)
                          .count();
      if (elapsed >= 5.0) {
         Description new_desc;
         if (load_description(desc_file, new_desc)) {
            std::lock_guard<std::mutex> lock(shared->mtx);
            if (!descriptions_equal(shared->desc, new_desc)) {
               shared->desc = new_desc;
               shared->desc_updated = true;
            }
         }
         last_check = std::chrono::steady_clock::now();
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(10));
   }

   listener_thread.join();

#ifdef _WIN32
   WSACleanup();
#endif

   return 0;
}
