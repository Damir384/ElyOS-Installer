#pragma once

#include "install_context.hpp"
#include "steps/steps_common.hpp"
#include <vector>
#include <functional>

namespace core {

class StateMachine {
public:
    StateMachine() = default;

    void clear_steps_from(size_t index);
    int run(InstallContext& ctx);

private:
    std::vector<Step> m_steps = core::steps::get_initial_steps();
    size_t m_current_index = 0;
    size_t m_max_unlocked_index = 0;
};

}