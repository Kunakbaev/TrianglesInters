// smart_logger.hpp - WORKING VERSION
#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>
#include <iostream>

inline void setup_logger() {
  spdlog::set_default_logger(spdlog::stdout_color_mt(""));
  // Simple pattern without source location - we'll add it manually
  spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
  spdlog::set_level(spdlog::level::debug);
}

inline std::string get_short_filename(const char* full_path) {
  std::string file(full_path);
  size_t last_slash = file.find_last_of("/\\");
  return (last_slash != std::string::npos) ? file.substr(last_slash + 1) : file;
}

inline std::string build_log_format(const char* names) {
  std::string result;
  std::string names_str(names);
  size_t start = 0, end = 0;
  
  while ((end = names_str.find(',', start)) != std::string::npos) {
    auto var_name = names_str.substr(start, end - start);
    var_name.erase(0, var_name.find_first_not_of(" \t"));
    var_name.erase(var_name.find_last_not_of(" \t") + 1);
    result += var_name + " = {}, ";
    start = end + 1;
  }
  
  auto last_var = names_str.substr(start);
  last_var.erase(0, last_var.find_first_not_of(" \t"));
  last_var.erase(last_var.find_last_not_of(" \t") + 1);
  result += last_var + " = {}";
  
  return result;
}

// Main macro with manual source location
#define LOG_VARS(...) do { \
  std::string format = "[" + get_short_filename(__FILE__) + ":" + \
                       std::to_string(__LINE__) + "] " + \
                       build_log_format(#__VA_ARGS__); \
  spdlog::info(format, __VA_ARGS__);\
} while(0)


