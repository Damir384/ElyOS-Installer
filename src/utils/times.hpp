#pragma once

#include <chrono>
#include <string>

namespace utils {
    std::chrono::system_clock::time_point get_time();
    std::string time_to_string(const std::chrono::system_clock::time_point& tp);
}
