#include "core/core.hpp"
#include "ftxui_context.hpp"
#include <memory>

int main(int argc, char* argv[]) {
    // Фронтенд создает только свой UI-контекст и отдает управление ядру
    auto tui = std::make_unique<FTXUIContext>();
    return core::App::bootstrap(argc, argv, tui.get());
}
