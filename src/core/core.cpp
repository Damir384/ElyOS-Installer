#include "core.hpp"
#include "env.hpp"
#include "welcome.hpp"
#include "utils/console.hpp"
#include "state_machine.hpp"
#include "ui/ui_interface.hpp"

namespace core {

App::App() = default;

int App::bootstrap(int argc, char* argv[], UIContext* ui) {
    App app;

    ExitCode status = app.init(argc, argv);
    if (status != ExitCode::SUCCESS) {
        return static_cast<int>(status);
    }

    if (ui) {
        ui->init_screen();
    }

    int exit_code = app.run(ui);

    if (ui) {
        ui->shutdown_screen();
    }

    return exit_code;
}

ExitCode App::init(int argc, char* argv[]) {
    Logger::instance().init(log_file_path);

    InstallContext temp_ctx;
    int parse_status = utils::parse_cli_args(argc, argv, temp_ctx);

    if (parse_status == 1) {
        utils::print_console("ElyOS Installer v" ELY_VERSION "\nUsage: sudo ./ElyOS_Installer [-d|--debug] [-h|--help]");
        return ExitCode::HELP_REQUESTED;
    } else if (parse_status < 0) {
        return ExitCode::ENVIRONMENT_INVALID;
    }

    if (temp_ctx.debug_mode) {
        this->debug = true;
        Logger::instance().set_min_level(LogLevel::DEBUG);
        this->log(LogLevel::DEBUG, "Debug logging threshold engaged globally.");
    } else {
        Logger::instance().set_min_level(LogLevel::INFO);
    }

    core::print_welcome_message();

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
    Logger::instance().log(level, message);
}

int App::run(UIContext* ui) {
    this->log(LogLevel::INFO, "Core engine execution loop engaged.");
    InstallContext ctx;
    ctx.ui = ui;
    ctx.debug_mode = this->debug;

    core::StateMachine state_machine;
    return state_machine.run(ctx);
}

} // namespace core
