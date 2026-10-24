#include "core/core.hpp"

int main(int argc, char* argv[]) {
    core::App app;

    core::ExitCode status = app.init(argc, argv);
    if (status != core::ExitCode::SUCCESS) {
        return static_cast<int>(status);
    }

    return app.run();
}