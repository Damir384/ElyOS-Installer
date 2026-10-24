#pragma once

#ifndef ELY_APP_NAME
#define ELY_APP_NAME "ElyOS Installer"
#endif

#ifndef ELY_VERSION
#define ELY_VERSION "0.0.1-dev"
#endif

#include <string>
#include <vector>

enum class InstallMode {
    UNDECIDED,
    NORMAL,
    ADVANCED
};

enum class StepResult {
    NEXT,
    BACK,
    CANCEL,
    FATAL_ERROR,
    RESET
};

class UIContext;

struct InstallContext {
    std::string app_name = ELY_APP_NAME;
    std::string app_version = ELY_VERSION;
    std::string mode_name = "Undecided";

    InstallMode mode = InstallMode::UNDECIDED;
    std::string target_disk;
    std::string boot_part;
    std::string root_part;
    // std::string username;
    // std::string hostname = "elyos-node";
    
    bool is_ready_to_install = false;
    UIContext* ui = nullptr;
};