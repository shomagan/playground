#include "device_updater.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>

#include "http_client.hpp"
#include "logger.hpp"

using json = nlohmann::json;

static const std::string LOG_DIR = "data_log";
static const std::string HTTP_PATH = "/get_json.cgi";
static const int HTTP_PORT = 80;
static const double VALUE_EPSILON = 1e-9;

static bool nearly_equal(double a, double b) {
   return std::abs(a - b) < VALUE_EPSILON;
}

static std::optional<std::string> read_last_line(const std::string &file_name) {
   std::ifstream in(file_name, std::ios::binary);
   if (!in.is_open())
      return std::nullopt;

   in.seekg(0, std::ios::end);
   std::streamoff size = in.tellg();
   if (size == 0)
      return std::nullopt;

   std::streamoff chunk = std::min<std::streamoff>(size, 4096);
   std::string buf(static_cast<size_t>(chunk), '\0');
   in.seekg(size - chunk);
   in.read(buf.data(), chunk);

   while (!buf.empty() && buf.back() == '\n')
      buf.pop_back();

   size_t last_nl = buf.find_last_of('\n');
   return last_nl == std::string::npos ? buf : buf.substr(last_nl + 1);
}

static std::optional<json> send_request(const std::string &address, const std::string &payload) {
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

static void handle_write(const std::string &address, const Space &space) {
   std::string read_req = R"({"request":"regs","reg_num":1,"get_type":"only_value","name":")" + space.var_name + R"("})";
   auto result = send_request(address, read_req);
   Logger::debug("read result - ", result ? result->dump() : "null");

   if (!result || !result->contains("value"))
      return;

   if (!nearly_equal((*result)["value"].get<double>(), space.value)) {
      std::ostringstream wr;
      wr << R"({"request":"regs_set","reg_num":1,"name":")" << space.var_name << R"(","value":)" << space.value << "}";
      auto wr_result = send_request(address, wr.str());
      Logger::debug("write result - ", wr_result ? wr_result->dump() : "null");
   }
}

static void handle_read(const std::string &address, const Device &device, const Space &space) {
   std::string file_name =
      LOG_DIR + "/" + device.device_name + "_" + std::to_string(device.mdb_address) + "_" + space.var_name + ".csv";

   bool update = true;
   if (auto last_line = read_last_line(file_name)) {
      size_t sep = last_line->find(';');
      if (sep != std::string::npos) {
         try {
            double last_time = std::stod(last_line->substr(0, sep));
            double now = std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
            Logger::debug("last_time - ", last_time);
            if (std::abs(now - last_time) <= space.interval_sec)
               update = false;
         } catch (...) {
         }
      }
   }

   if (!update)
      return;

   std::string req = R"({"request":"regs","reg_num":1,"get_type":"only_value","name":")" + space.var_name + R"("})";
   auto result = send_request(address, req);
   Logger::debug("read result - ", result ? result->dump() : "null");

   if (result && result->contains("value")) {
      double now = std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
      std::ofstream out(file_name, std::ios::app);
      out << std::fixed << std::setprecision(3) << now << "; " << (*result)["value"] << "\n";
   }
}

void update_vars(const std::string &address, const Device &device) {
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
