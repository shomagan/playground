#pragma once

#include <string>
#include <vector>

struct Space
{
   std::string var_name;
   std::string function;
   double value = 0.0;
   double interval_sec = 0.0;
};

struct Device
{
   int mdb_address = 0;
   std::string device_name;
   std::vector<Space> spaces;
};

struct Description
{
   std::vector<Device> devices;
};

bool load_description(const std::string &file_path, Description &desc);
bool descriptions_equal(const Description &a, const Description &b);
