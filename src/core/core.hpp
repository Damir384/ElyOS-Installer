#pragma once

#include "utils/logger.hpp"
#include <string>

class UIContext;

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

    // Единая точка входа: проверки хоста + запуск UI + исполнение FSM
    static int bootstrap(int argc, char* argv[], UIContext* ui);

    void log(LogLevel level, const std::string& message);

    std::string log_file_path = "elyos-installer.log";
    bool debug = false;

private:
    ExitCode init(int argc, char* argv[]);
    int run(UIContext* ui);

    bool m_is_initialized = false;
};

} // namespace core
