#include "fs_utils.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cerrno>
#include <cstring>

namespace utils {

int check_file_exist(const std::string& path, std::string& message) {
    std::error_code ec;
    if (!std::filesystem::exists(path, ec) || ec) {
        message = "File does not exist: " + path;
        return -1;
    }
    return 0;
}

int check_file_readable(const std::string& path, std::string& message) {
    if (access(path.c_str(), R_OK) != 0) {
        message = "File is not readable (" + std::string(std::strerror(errno)) + "): " + path;
        return -1;
    }
    return 0;
}

std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

} // namespace utils