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

struct PartitionPlan {
    std::string parent_disk;
    int partition_number = 1;
    
    std::string size_spec;
    std::string type_code;
    bool is_bootable = false;
    
    std::string fs_type;
    std::string mount_point;
    
    std::string device_path;
    std::string part_uuid;
};

struct InstallContext {
    bool debug_mode = false;
    bool show_help = false;
    bool is_efi = false; 

    std::string app_name = ELY_APP_NAME;
    std::string app_version = ELY_VERSION;
    std::string mode_name = "Undecided";

    InstallMode mode = InstallMode::UNDECIDED;

    std::string target_disk;
    std::vector<PartitionPlan> partitions;

    std::string log_file_path = "elyos-installer.log";
    std::string partition_profiles_path = "partitions_list.json";

    bool is_ready_to_install = false;
    UIContext* ui = nullptr;
};
