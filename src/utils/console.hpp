#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

struct InstallContext; // Forward declaration

namespace utils {

enum {
    OPT_LOG_FILE = 1000,
    OPT_PARTITION_PROFILES
};

struct ProcessResult {
    int exit_code = -1;
    std::string output;
    bool success() const { return exit_code == 0; }
};

class ProcessRunner {
public:
    static ProcessResult execute(const std::unordered_map<std::string, std::string>& env,
                                 const std::string& command, 
                                 const std::vector<std::string>& args, 
                                 bool capture_output = true);
};

// POSIX разбор аргументов напрямую в структуру контекста
int parse_cli_args(int argc, char* argv[], InstallContext& ctx);

void print_console(std::string_view message, bool log_to_file = true);
void print_console_error(std::string_view message, bool log_to_file = true);

} // namespace utils
