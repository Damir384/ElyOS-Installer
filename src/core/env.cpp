#include "env.hpp"
#include <unistd.h>
#include <filesystem>

namespace core {

int check_environment(InstallContext& ctx, std::string& message) {
    // 1. Проверка прав суперпользователя
    if (geteuid() != 0) {
        message = "Root privileges required. Run installer via sudo.";
        return -1;
    };

    // 2. Проверка режима загрузки (UEFI vs BIOS)
    ctx.is_efi = false;
    if(std::filesystem::exists("/sys/firmware/efi")) {
        ctx.is_efi = true;
    };

    return 0;
}

} // namespace core