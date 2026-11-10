#pragma once
#include <string>

namespace utils {
    int check_file_exist(const std::string& path, std::string& message);

    int check_file_readable(const std::string& path, std::string& message);

    std::string read_file(const std::string& path);
}