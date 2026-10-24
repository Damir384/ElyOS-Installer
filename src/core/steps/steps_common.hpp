#pragma once

#include "../install_context.hpp"
#include "ui/ui_interface.hpp"
#include "utils/disk.hpp"
#include <string>
#include <functional>
#include <vector>

struct Step {
    std::string title;
    std::function<StepResult(InstallContext&)> execute;
};

namespace core::steps {

    std::vector<Step> get_initial_steps();

    StepResult template_step(InstallContext& ctx);

    StepResult welcome_step(InstallContext& ctx);

    StepResult disk_select_step(InstallContext& ctx);

} // namespace core::steps