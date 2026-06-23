#pragma once

#include <memory>
#include <mutex>

#include "config.hpp"

struct SharedState
{
   std::mutex mtx;
   Description desc;
   bool desc_updated = false;
   bool running = true;
};

void udp_listener(std::shared_ptr<SharedState> state);
