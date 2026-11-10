#pragma once

#include <string>
#include "install_context.hpp"

namespace core {

/**
 * @brief Performs pre-flight host validation checks.
 * @details Checks for root privileges (geteuid() == 0) and inspects firmware
 * interface presence (/sys/firmware/efi) to determine UEFI vs Legacy BIOS mode.
 * 
 * @param [in,out] ctx Installation context where the is_efi flag is recorded.
 * @param [out] message Output string describing the missing requirement on failure.
 * @return int Returns 0 if validation succeeds, -1 if requirements are unmet.
 */
int check_environment(InstallContext& ctx, std::string& message);

} // namespace core