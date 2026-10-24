#include "env.hpp"
#include <unistd.h>
#include <filesystem>

namespace core {

EnvCheckResult check_environment() {
    EnvCheckResult result;

    // 1. Проверка прав суперпользователя
    result.is_root = (geteuid() == 0);

    // 2. Проверка режима загрузки (UEFI vs BIOS)
    result.is_efi = std::filesystem::exists("/sys/firmware/efi");

    // Критический критерий для установщика ОС: запуск из-под root
    if (!result.is_root) {
        result.error_details = "Installer must be run with root privileges (sudo).";
        result.is_valid = false;
    } else {
        result.is_valid = true;
    }

    return result;
}

} // namespace core