#include "core.hpp"
#include "args.hpp"
#include "env.hpp"
#include "welcome.hpp"
#include "utils/console.hpp"
#include "state_machine.hpp"
#include "ui/ftxui_context.hpp" // Подключаем конкретную реализацию

namespace core {

App::App() = default;

ExitCode App::init(int argc, char* argv[]) {
    Logger::instance().init(log_file_path);

    ParsedArgs parsed = core::parse_arguments(argc, argv);
    this->debug = parsed.debug;

    if (parsed.show_help) {
        utils::print_console("ElyOS Installer. Usage: sudo ./ElyOS_Installer [--debug|-d] [--help|-h]");
        return ExitCode::HELP_REQUESTED;
    }

    // Выводим только в консоль (лог остаётся чистым)
    core::print_welcome_message();

    // Сообщения ниже автоматически попадут и в stdout, и в лог с тегом [CONSOLE]
    EnvCheckResult env = core::check_environment();
    utils::print_console(std::string("Host firmware: ") + (env.is_efi ? "UEFI" : "Legacy BIOS (MBR)"));

    if (!env.is_root) {
        utils::print_console_error("Root privileges required. Run installer via sudo.");
        return ExitCode::PERMISSION_DENIED;
    }

    if (!env.is_valid) {
        utils::print_console_error("Hardware environment check failed: " + env.error_details);
        return ExitCode::ENVIRONMENT_INVALID;
    }

    utils::print_console("Hardware verified. Node environment valid.");

    this->m_is_initialized = true;

    return ExitCode::SUCCESS;
}

void App::log(LogLevel level, const std::string& message) {
    if (level == LogLevel::DEBUG && !this->debug) return;
    Logger::instance().log(level, message);
}

int App::run() {
    this->log(LogLevel::INFO, "Kernel handoff to TUI completed.");
    std::unique_ptr<UIContext> ui_context = std::make_unique<FTXUIContext>();
    ui_context->init_screen();
    InstallContext ctx;
    ctx.ui = ui_context.get();
    core::StateMachine StateMachine;
    StateMachine.run(ctx);
    ui_context->shutdown_screen();
    return 0;
}

} // namespace core