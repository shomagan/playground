#pragma once

#include <optional>
#include <string>

std::optional<std::string> http_get(
   const std::string &host, int port, const std::string &path, const std::string &json_header);
