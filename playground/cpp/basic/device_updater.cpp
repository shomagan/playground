#include "device_updater.hpp"

#include <chrono>
#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>

#include "http_client.hpp"
#include "logger.hpp"

using json = nlohmann::json;

static const std::string LOG_DIR = "data_log";
static const std::string HTTP_PATH = "/get_json.cgi";
static const int HTTP_PORT = 80;

static std::optional<json> send_request(const std::string &address, const std::string &payload)
{
   auto body = http_get(address, HTTP_PORT, HTTP_PATH, payload);
   if (!body)
      return std::nullopt;
   try {
      return json::parse(*body);
   } catch (...) {
      Logger::error("Failed to parse HTTP response: ", *body);
      return std::nullopt;
   }
}

static void handle_write(const std::string &address, const Space &space)
{
   std::string read_req =
      R"({"request":"regs","reg_num":1,"get_type":"only_value","name":")" + space.var_name + R"("})";
   auto result = send_request(address, read_req);
   Logger::debug("read result - ", result ? result->dump() : "null");

   if (!result || !result->contains("value"))
      return;

   if ((*result)["value"].get<double>() != space.value) {
      std::ostringstream wr;
      wr << R"({"request":"regs_set","reg_num":1,"name":")" << space.var_name
         << R"(","value":)" << space.value << "}";
      auto wr_result = send_request(address, wr.str());
      Logger::debug("write result - ", wr_result ? wr_result->dump() : "null");
   }
}

static void handle_read(const std::string &address, const Device &device, const Space &space)
{
   std::string file_name = LOG_DIR + "/" + device.device_name + "_" +
                           std::to_string(device.mdb_address) + "_" + space.var_name + ".csv";

   bool update = true;
   std::ifstream in(file_name);
   if (in.is_open()) {
      std::string last_line, line;
      while (std::getline(in, line)) {
         if (!line.empty())
            last_line = line;
      }
      in.close();
      size_t sep = last_line.find(';');
      if (sep != std::string::npos) {
         try {
            double last_time = std::stod(last_line.substr(0, sep));
            double now = std::chrono::duration<double>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count();
            Logger::debug("last_time - ", last_time);
            if (std::abs(now - last_time) <= space.interval_sec)
               update = false;
         } catch (...) {
         }
      }
   }

   if (!update)
      return;

   std::string req = R"({"request":"regs","reg_num":1,"get_type":"only_value","name":")" +
                     space.var_name + R"("})";
   auto result = send_request(address, req);
   Logger::debug("read result - ", result ? result->dump() : "null");

   if (result && result->contains("value")) {
      double now =
         std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
      std::ofstream out(file_name, std::ios::app);
      out << now << "; " << (*result)["value"] << "\n";
   }
}

void update_vars(const std::string &address, const Device &device)
{
   for (const auto &space : device.spaces) {
      if (space.function.empty() || space.var_name.empty())
         continue;
      if (space.function == "write") {
         handle_write(address, space);
      } else if (space.function == "read") {
         handle_read(address, device, space);
      }
   }
}
