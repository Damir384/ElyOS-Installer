#pragma once

#include <string>
#include <vector>

namespace utils {
struct ProcessResult {
    int exit_code = -1;
    std::string output;

    bool success() const { return exit_code == 0; }
};

class ProcessRunner {
public:
    static ProcessResult execute(const std::vector<std::string>& env,
                                 const std::string& command, 
                                 const std::vector<std::string>& args, 
                                 bool capture_output = true);
};

void print_console(std::string_view message, bool log_to_file = true);
void print_console_error(std::string_view message, bool log_to_file = true);
}