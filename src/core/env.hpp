#pragma once

#include <string>

namespace core {

struct EnvCheckResult {
    bool is_root = false;
    bool is_efi = false;
    bool is_valid = false;
    std::string error_details;
};

// Проверяет UID процесса и наличие /sys/firmware/efi
EnvCheckResult check_environment();

} // namespace core