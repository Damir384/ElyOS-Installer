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

/**
 * @brief Low-level child process execution engine.
 */
class ProcessRunner {
public:
    /**
     * @brief Executes a system command using POSIX pipe, fork, and execvpe.
     * @details Redirects stdout and stderr to a pipe buffer and injects
     * LANG=C / LC_ALL=C into the child environment to guarantee standard output format.
     * 
     * @param [in] env Map of custom environment variables to inject.
     * @param [in] command Binary name (resolved via PATH) or absolute path.
     * @param [in] args Ordered vector of command-line arguments.
     * @param [in] capture_output Whether to intercept stdout/stderr into the output buffer.
     * @return ProcessResult Structure containing the exit code and captured text output.
     */
    static ProcessResult execute(const std::unordered_map<std::string, std::string>& env,
                                 const std::string& command, 
                                 const std::vector<std::string>& args, 
                                 bool capture_output = true);
};

    /**
     * @brief Parses command-line options using standard POSIX getopt_long.
     * @details Reads arguments and mutates corresponding fields of the provided context.
     * 
     * @param [in] argc Number of arguments passed from main().
     * @param [in] argv Array of argument strings.
     * @param [in,out] ctx Target context structure where flags are written.
     * @return int Status code:
     *             -  0: Parsing completed successfully;
     *             -  1: Help option requested (-h / --help);
     *             - -1: Syntax error or unrecognized argument.
     */
    int parse_cli_args(int argc, char* argv[], InstallContext& ctx);

    /**
     * @brief Prints an informational message to stdout.
     * @param [in] message Text to display.
     * @param [in] log_to_file Whether to duplicate output to the logger instance (defaults to true).
     */
    void print_console(std::string_view message, bool log_to_file = true);

    /**
     * @brief Prints an error message to stderr with ANSI color styling.
     * @param [in] message Error text to display.
     * @param [in] log_to_file Whether to record this entry as an ERROR in the logger (defaults to true).
     */
    void print_console_error(std::string_view message, bool log_to_file = true);

} // namespace utils
