#include "steps_common.hpp"

namespace core::steps {
    std::vector<Step> get_normal_installation_steps();
    StepResult test_normal_step(InstallContext& ctx);
}