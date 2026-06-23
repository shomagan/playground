#pragma once

#include <iostream>
#include <mutex>
#include <string>

enum class LogLevel
{
   DEBUG = 0,
   INFO = 1,
   WARNING = 2,
   ERROR_LEVEL = 3
};

class Logger
{
public:
   static LogLevel current_level;
   static std::mutex mtx;

   static void set_level(const std::string &lvl)
   {
      if (lvl == "DEBUG")
         current_level = LogLevel::DEBUG;
      else if (lvl == "WARNING")
         current_level = LogLevel::WARNING;
      else if (lvl == "ERROR")
         current_level = LogLevel::ERROR_LEVEL;
      else
         current_level = LogLevel::INFO;
   }

   template<typename... Args>
   static void debug(Args &&...args)
   {
      log(LogLevel::DEBUG, std::forward<Args>(args)...);
   }

   template<typename... Args>
   static void info(Args &&...args)
   {
      log(LogLevel::INFO, std::forward<Args>(args)...);
   }

   template<typename... Args>
   static void warning(Args &&...args)
   {
      log(LogLevel::WARNING, std::forward<Args>(args)...);
   }

   template<typename... Args>
   static void error(Args &&...args)
   {
      log(LogLevel::ERROR_LEVEL, std::forward<Args>(args)...);
   }

private:
   template<typename... Args>
   static void log(LogLevel lvl, Args &&...args)
   {
      if (lvl >= current_level) {
         std::lock_guard<std::mutex> lock(mtx);
         (std::cout << ... << args);
         std::cout << '\n';
      }
   }
};

inline LogLevel Logger::current_level = LogLevel::INFO;
inline std::mutex Logger::mtx;
