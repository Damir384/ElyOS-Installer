#pragma once

#include <chrono>
#include <string>

namespace utils {
    /**
     * @brief Retrieves the current system clock time point.
     * @return std::chrono::system_clock::time_point Native system time point.
     */
    std::chrono::system_clock::time_point get_time();

    /**
     * @brief Formats a system time point into a local time string (HH:MM:SS).
     * @param [in] tp Time point value to format.
     * @return std::string Formatted 24-hour time representation.
     */
    std::string time_to_string(const std::chrono::system_clock::time_point& tp);
}
