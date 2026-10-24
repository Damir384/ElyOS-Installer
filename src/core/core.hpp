#pragma once

#include "utils/logger.hpp"
#include <string>

namespace core {

enum class ExitCode : int {
    SUCCESS = 0,
    HELP_REQUESTED = 1,
    PERMISSION_DENIED = 2,
    ENVIRONMENT_INVALID = 3
};

class App {
public:
    App();
    ~App() = default;

    ExitCode init(int argc, char* argv[]);
    void log(LogLevel level, const std::string& message);
    int run();

    std::string log_file_path = "elyos-installer.log";
    bool debug = false;

private:
    bool m_is_initialized = false;
};

}