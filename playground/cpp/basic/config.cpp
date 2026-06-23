#include "config.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include "logger.hpp"

using json = nlohmann::json;

bool load_description(const std::string &file_path, Description &desc)
{
   std::ifstream file(file_path);
   if (!file.is_open()) {
      Logger::info("did't find file description - ", file_path);
      return false;
   }
   try {
      json j;
      file >> j;
      desc.devices.clear();
      for (const auto &d : j["devices"]) {
         Device device;
         device.mdb_address = d.value("mdb_address", 0);
         device.device_name = d.value("device_name", "");
         for (const auto &s : d["spaces"]) {
            Space space;
            space.var_name = s.value("var_name", "");
            space.function = s.value("function", "");
            space.value = s.value("value", 0.0);
            space.interval_sec = s.value("interval_sec", 0.0);
            device.spaces.push_back(space);
         }
         desc.devices.push_back(device);
      }
      return true;
   } catch (const std::exception &e) {
      Logger::error("Failed to parse description: ", e.what());
      return false;
   }
}

bool descriptions_equal(const Description &a, const Description &b)
{
   if (a.devices.size() != b.devices.size())
      return false;
   for (size_t i = 0; i < a.devices.size(); i++) {
      const auto &da = a.devices[i];
      const auto &db = b.devices[i];
      if (da.device_name != db.device_name || da.mdb_address != db.mdb_address)
         return false;
      if (da.spaces.size() != db.spaces.size())
         return false;
      for (size_t j = 0; j < da.spaces.size(); j++) {
         const auto &sa = da.spaces[j];
         const auto &sb = db.spaces[j];
         if (sa.var_name != sb.var_name || sa.function != sb.function || sa.value != sb.value ||
             sa.interval_sec != sb.interval_sec)
            return false;
      }
   }
   return true;
}
