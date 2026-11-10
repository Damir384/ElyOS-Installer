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
    int parse_status = utils::parse_cli_args(argc, argv, this->m_ctx);

    Logger::instance().init(m_ctx.log_file_path);

    if (parse_status == 1) {
        utils::print_console("ElyOS Installer v" ELY_VERSION "\nUsage: sudo ./ElyOS_Installer [-d|--debug] [-h|--help]");
        return ExitCode::HELP_REQUESTED;
    } else if (parse_status < 0) {
        return ExitCode::ENVIRONMENT_INVALID;
    }

    if (this->m_ctx.debug_mode) {
        Logger::instance().set_min_level(LogLevel::DEBUG);
        Logger::instance().log(LogLevel::INFO, "Debug logging threshold engaged globally.");
    } else {
        Logger::instance().set_min_level(LogLevel::INFO);
        Logger::instance().log(LogLevel::INFO, "Debug logging disable globally.");
    }

    core::print_welcome_message();

    std::string message;
    if(core::check_environment(this->m_ctx, message) != 0){
        utils::print_console_error(message);
        return ExitCode::ENVIRONMENT_INVALID;
    };

    utils::print_console(std::string("Host firmware: ") + (this->m_ctx.is_efi ? "UEFI" : "Legacy BIOS (MBR)"));

    utils::print_console("Hardware verified. Node environment valid.");
    this->m_is_initialized = true;

    return ExitCode::SUCCESS;
}

int App::run(UIContext* ui) {
    Logger::instance().log(LogLevel::INFO, "Core engine execution loop engaged.");
    
    this->m_ctx.ui = ui;

    core::StateMachine state_machine;
    return state_machine.run(this->m_ctx);
}

} // namespace core
