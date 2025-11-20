#include "disk_steps.hpp"

StepResult ShowPartitionInfo::run(WINDOW* win) {
    std::string description = "You will be asked to partition your disk.\n\nAvailable layouts:\n1. boot + root\n2. boot + root + swap\n\n• boot — separate /boot partition for kernels and bootloader\n• root — your main system (/)\n• swap — used for memory overflow, hibernation, and more.\n\nAll data on the selected disk WILL BE ERASED.\nMake sure you've selected the correct device.";
    dialogs::dialog_text(win, "partition info", description);
    return StepResult::Next;
}

StepResult ShowDisks::run(WINDOW* win) {
    std::string description = exec("lsblk -Jndo NAME,TYPE");
    dialogs::dialog_text(win, "select disk", description);
    return StepResult::Next;
}