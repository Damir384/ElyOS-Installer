#include "times.hpp"
#include <iomanip>
#include <sstream>

namespace utils {

std::chrono::system_clock::time_point get_time() {
    return std::chrono::system_clock::now();
}

std::string time_to_string(const std::chrono::system_clock::time_point& tp) {
    std::time_t timeT = std::chrono::system_clock::to_time_t(tp);
    std::tm localTm = *std::localtime(&timeT);

    std::stringstream ss;
    ss << std::put_time(&localTm, "%H:%M:%S");
    return ss.str();
}

} // namespace utils
