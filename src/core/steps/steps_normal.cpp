#include "steps_normal.hpp"
#include <vector>

    namespace core::steps {
    std::vector<Step> get_normal_installation_steps() {
        using namespace core::steps;
        return {
            {"Тестовый шаг общего режима", template_step},
            {"Тестовый шаг нормального режима", test_normal_step}
        };
    }
    StepResult test_normal_step(InstallContext& ctx) {
        return StepResult::NEXT;
    }
}