#pragma once

#include <string>
#include "install_context.hpp"

namespace core {

int check_environment(InstallContext& ctx, std::string& message);

} // namespace core