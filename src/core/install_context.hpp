#pragma once

#ifndef ELY_APP_NAME
#define ELY_APP_NAME "ElyOS Installer"
#endif

#ifndef ELY_VERSION
#define ELY_VERSION "0.0.1-dev"
#endif

#include <string>
#include <vector>

enum class StepResult {
    NEXT,
    BACK,
    CANCEL,
    FATAL_ERROR,
    RESET
};

class UIContext;

// Represents a single partition entry in the installation layout.
// Phase 1 fields are populated from the selected JSON profile;
// Phase 2 fields are filled by the core after disk partitioning (sgdisk / partprobe).
struct PartitionPlan {
    // --- Phase 1: Planned layout (before destructive disk operations) ---
    std::string parent_disk;     // Target block device path (copied from ctx.target_disk, e.g. "/dev/nvme0n1")
    int partition_number = 1;    // 1-based partition index (used for sgdisk -n)

    std::string size_spec;       // Size specification from JSON ("512M", "20G", or "auto" for remaining space)
    std::string type_code;       // GPT type code for sgdisk (e.g. "ef00" ESP, "8300" Linux, "8200" swap)
    bool is_bootable = false;    // Bootable/ESP flag indicator

    std::string fs_type;         // Target filesystem for mkfs ("fat32", "ext4", "btrfs", etc.)
    std::string mount_point;     // Target mount point in the installed system ("/boot", "/", "/home")

    // --- Phase 2: Resolved hardware state (after partitioning) ---
    std::string device_path;     // Kernel-assigned partition node (e.g. "/dev/nvme0n1p1", "/dev/sda1")
    std::string part_uuid;       // Created partition/FS UUID or PARTUUID (for fstab and bootloader)
};

// Persistent installation state shared across all StateMachine steps.
struct InstallContext {
    // --- CLI flags and host environment ---
    bool debug_mode = false;     // -d flag: enables verbose DEBUG logging
    bool show_help = false;      // -h flag: prints CLI usage and exits without starting TUI
    bool is_efi = false;         // Host firmware mode (true = UEFI, false = Legacy BIOS)

    // --- Metadata and FSM mode ---
    std::string app_name = ELY_APP_NAME;       // Application name injected by CMake
    std::string app_version = ELY_VERSION;     // Build version injected by CMake
    std::string mode = "Undecided";            // Active installation mode key for modes_registry

    // --- Target storage and partitioning plan ---
    std::string target_disk;                   // Selected destination block device (e.g. "/dev/nvme0n1")
    std::vector<PartitionPlan> partitions;     // Ordered list of partitions to create, format, and mount

    // --- Runtime file paths ---
    std::string log_file_path = "elyos-installer.log";            // Output log file path
    std::string partition_profiles_path = "partitions_list.json"; // Path to partition layouts JSON file

    // --- Execution gate and UI handle ---
    bool is_ready_to_install = false;          // Explicit user confirmation gate before Phase 2 starts
    UIContext* ui = nullptr;                   // Non-owning pointer to active UI backend (e.g. FTXUI)
};
