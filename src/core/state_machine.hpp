#pragma once

#include "install_context.hpp"
#include "steps/steps_common.hpp"
#include <vector>
#include <functional>

namespace core {

/**
 * @brief Finite State Machine controlling installation step sequence.
 * @details Enforces forward transitions through uncompleted steps while supporting
 * safe rollback navigation via StepResult::BACK without destroying state context.
 */
class StateMachine {
public:
    StateMachine() = default;

    /**
     * @brief Truncates registered step vector starting from a specific index.
     * @param [in] index Zero-based start index for step removal.
     */
    void clear_steps_from(size_t index);

    /**
     * @brief Runs the sequential step execution loop.
     * @details Dispatches execution to the active step, audits the returned StepResult,
     * adjusts current index, and handles cancellation or fatal error states.
     * 
     * @param [in,out] ctx Shared installation context passed through all steps.
     * @return int Returns 0 on completion, 1 on cancellation, -1 on fatal failure.
     */
    int run(InstallContext& ctx);

private:
    std::vector<Step> m_steps = core::steps::get_initial_steps();
    size_t m_current_index = 0;
    size_t m_max_unlocked_index = 0;
};

}