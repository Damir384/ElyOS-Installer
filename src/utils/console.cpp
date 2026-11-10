#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "console.hpp"
#include "logger.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <array>
#include "core/install_context.hpp"
#include <getopt.h>

extern char** environ;

namespace utils {

ProcessResult ProcessRunner::execute(const std::unordered_map<std::string, std::string>& env,
                                     const std::string& command, 
                                     const std::vector<std::string>& args, 
                                     bool capture_output) 
{
    ProcessResult result;
    int pipefd[2];
    pipe(pipefd);
    pid_t pid = fork();
    if (pid == 0) {
        if (capture_output) {
            dup2(pipefd[1], STDOUT_FILENO);
            dup2(pipefd[1], STDERR_FILENO);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        std::vector<char*> argv;
        argv.push_back(const_cast<char*>(command.c_str()));
        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);

        std::unordered_map<std::string, std::string> env_map;
        for (char** current_env = environ; *current_env != nullptr; ++current_env) {
            std::string_view entry(*current_env);
            size_t eq_pos = entry.find('=');
            if (eq_pos != std::string_view::npos) {
                std::string key(entry.substr(0, eq_pos));
                std::string val(entry.substr(eq_pos + 1));
                env_map[key] = val;
            }
        }

        env_map["LANG"] = "C";
        env_map["LC_ALL"] = "C";

        for (const auto& [key, val] : env) {
            env_map[key] = val;
        }

        std::vector<std::string> env_strings;
        env_strings.reserve(env_map.size());

        for (const auto& [key, val] : env_map) {
            env_strings.push_back(key + "=" + val);
        }

        // Д. Формируем Си-массив указателей char*
        std::vector<char*> envp;
        envp.reserve(env_strings.size() + 1);

        for (auto& s : env_strings) {
            envp.push_back(s.data()); // Гарантированно живет в пределах env_strings
        }
        envp.push_back(nullptr); // Терминатор массива

        // -------------------------------------------------------------
        // 3. Вызов функции ядра
        // -------------------------------------------------------------
        execvpe(command.c_str(), argv.data(), envp.data());
        _exit(127);
    }

    if (pid > 0) {
        close(pipefd[1]);

        if (capture_output) {
            char buffer[4096];
            ssize_t n;

            while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
                result.output.append(buffer, n);
            }
        }

        close(pipefd[0]);

        int status = 0;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            result.exit_code = WEXITSTATUS(status);
        }
    }

    Logger::instance().log(LogLevel::DEBUG, "Command "+command+" ended with code "+std::to_string(result.exit_code));

    return result;
}

void print_console(std::string_view message, bool log_to_file) {
    std::cout << message << std::endl;

    if (log_to_file) {
        Logger::instance().log(LogLevel::INFO, std::string("[CONSOLE] ") + std::string(message));
    }
}

void print_console_error(std::string_view message, bool log_to_file) {
    std::cerr << "\033[31m[ERROR] " << message << "\033[0m" << std::endl;

    if (log_to_file) {
        Logger::instance().log(LogLevel::ERROR, std::string("[CONSOLE] ") + std::string(message));
    }
}

int parse_cli_args(int argc, char* argv[], InstallContext& ctx) {
    static struct option long_options[] = {
        {"debug", no_argument, nullptr, 'd'},
        {"help",  no_argument, nullptr, 'h'},
        {"log-file",  required_argument, nullptr, OPT_LOG_FILE},
        {"partition-profiles",  required_argument, nullptr, OPT_PARTITION_PROFILES},
        {nullptr, 0,           nullptr, 0}
    };

    optind = 1;
    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "dh", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'd':
                ctx.debug_mode = true;
                break;
            case 'h':
                ctx.show_help = true;
                return 1;
            case OPT_LOG_FILE:
                ctx.log_file_path = optarg;
                break;
            case OPT_PARTITION_PROFILES:
                ctx.partition_profiles_path = optarg;
                break;
            default:
                return -1;
        }
    }

    return 0;
}

} // namespace utils
