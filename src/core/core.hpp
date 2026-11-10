#pragma once

#include "utils/logger.hpp"
#include "install_context.hpp"
#include <string>

class UIContext;

namespace core {

enum class ExitCode : int {
    SUCCESS = 0,
    HELP_REQUESTED = 1,
    ENVIRONMENT_INVALID = 2
};

class App {
public:
    App();
    ~App() = default;

    // Единая точка входа: проверки хоста + запуск UI + исполнение FSM
    static int bootstrap(int argc, char* argv[], UIContext* ui);

private:
    ExitCode init(int argc, char* argv[]);
    int run(UIContext* ui);

    bool m_is_initialized = false;

    InstallContext m_ctx;
};

} // namespace core
