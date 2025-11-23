#include "disk_steps.hpp"

StepResult ShowPartitionInfo::run(WINDOW* win) {
    std::string description = "You will be asked to partition your disk.\n\nAvailable layouts:\n1. boot + root\n2. boot + root + swap\n\n• boot — separate /boot partition for kernels and bootloader\n• root — your main system (/)\n• swap — used for memory overflow, hibernation, and more.\n\nAll data on the selected disk WILL BE ERASED.\nMake sure you've selected the correct device.";
    dialogs::dialog_text(win, "partition info", description);
    return StepResult::Next;
}

StepResult ShowDisks::run(WINDOW* win) {
    nlohmann::json result = nlohmann::json::parse(exec("lsblk -Jndo NAME,TYPE,MODEL"));
    std::vector<std::string> disks;
    std::map<std::string, std::string> list;
    std::string disk;
    std::string cmd;

    for (const auto& dev : result["blockdevices"]) {
        if (!dev.contains("name") || !dev.contains("type"))
            continue;
        if (dev["type"] != "disk")
            continue;

        std::string disk = "/dev/";
        disk.append(dev["name"]);

        disks.push_back(disk);
        list[disk] = dev["model"];
    }

    int choice = 0;
    dialogs::dialog_menu(win, "select disk", list, choice);
    auto& ctx = InstallerContext::get();
    ctx.selectedDisk = disks[choice];
    Logger::get().log(LogLevel::INFO, "Selected disk: "+ctx.selectedDisk);
    return StepResult::Next;
}

StepResult ShowPartitions::run(WINDOW* win) {
    std::vector<std::string> list = {"BootRoot", "BootRootSwap"};
    std::vector<std::string> partitions = {"Boot + Root partitions", "Boot + Root + Swap partitions"};
    int choice = 0;

    dialogs::dialog_menu(win, "Select partitioning scheme", partitions, choice);

    auto& ctx = InstallerContext::get();
    ctx.partitionScheme = list[choice];
    Logger::get().log(LogLevel::INFO, "Selected partitioning scheme: "+ctx.partitionScheme);

    return StepResult::Next;
}