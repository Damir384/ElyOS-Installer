#include "state_machine.hpp"
#include "utils/logger.hpp"

namespace core {

void StateMachine::clear_steps_from(size_t index) {
    if (index < m_steps.size()) {
        m_steps.erase(m_steps.begin() + index, m_steps.end());
    }
}

int StateMachine::run(InstallContext& ctx) {
    while (m_current_index < m_steps.size()) {
        auto& step = m_steps[m_current_index];
        
        Logger::instance().log(LogLevel::INFO, "Entering step [" + std::to_string(m_current_index) + "]: " + step.title);

        StepResult result = step.execute(ctx);

        switch (result) {
            case StepResult::NEXT:
                if (m_current_index == m_max_unlocked_index) {
                    m_max_unlocked_index++;
                }
                m_current_index++;
                break;

            case StepResult::BACK:
                if (m_current_index > 0) {
                    m_current_index--;
                }
                break;

            case StepResult::CANCEL:
                Logger::instance().log(LogLevel::WARNING, "Installation canceled by user.");
                return 1;

            case StepResult::FATAL_ERROR:
                Logger::instance().log(LogLevel::ERROR, "Fatal error encountered at step: " + step.title);
                return -1;
            
            default:
                Logger::instance().log(LogLevel::ERROR, "The function returned an unknown status.");
                return -1;
        }
    }

    Logger::instance().log(LogLevel::INFO, "All steps completed successfully.");
    return 0;
}

}