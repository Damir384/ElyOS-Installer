#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "console.hpp"
#include "logger.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <array>

extern char** environ;

namespace utils {

ProcessResult ProcessRunner::execute(const std::vector<std::string>& env,
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
        for (auto& s : args) {
            argv.push_back(const_cast<char*>(s.c_str()));
        }
        argv.push_back(nullptr);

        std::vector<char*> envp;
        for (char** env_it = environ; *env_it != nullptr; ++env_it) {
            std::string_view entry(*env_it);
            if (entry.rfind("LANG=", 0) == 0 || entry.rfind("LC_", 0) == 0) {
                continue;
            }
            envp.push_back(*env_it);
        }

        static char lang_c[] = "LANG=C";
        static char lc_all_c[] = "LC_ALL=C";
        envp.push_back(lang_c);
        envp.push_back(lc_all_c);

        for (const auto& s : env) {
            envp.push_back(const_cast<char*>(s.c_str()));
        }

        envp.push_back(nullptr);

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

}