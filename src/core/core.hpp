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

    /**
     * @brief Central application lifecycle entry point (Bootstrap Facade).
     * @details Parses CLI parameters, performs host hardware/privilege validation,
     * initializes the provided UI context, executes the state machine, and shuts down UI.
     * 
     * @param [in] argc Argument count from main().
     * @param [in] argv Argument array from main().
     * @param [in] ui Pointer to the abstract UI context implementation.
     * @return int Process exit code (0 on success, non-zero on failure or cancellation).
     */
    static int bootstrap(int argc, char* argv[], UIContext* ui);

private:
    /**
     * @brief Performs environment verification and initial system setup.
     * @param [in] argc Argument count.
     * @param [in] argv Argument array.
     * @return ExitCode Initialization status.
     */
    ExitCode init(int argc, char* argv[]);

    /**
     * @brief Executes the primary state machine installation pipeline.
     * @param [in] ui Target UI delegate.
     * @return int State machine execution result.
     */
    int run(UIContext* ui);

    bool m_is_initialized = false;

    InstallContext m_ctx;
};

} // namespace core
