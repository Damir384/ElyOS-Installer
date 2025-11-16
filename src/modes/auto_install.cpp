#include "../../include/step.hpp"
#include "../core/step_manager.cpp"
#include "../steps/test_steps.cpp"

class AutoInstall {
public:
    int run(WINDOW* win) {
        StepManager manager;

        manager.add(std::make_unique<StepTest>());
        manager.add(std::make_unique<StepTest>());

        manager.run(win);
        return 0;
    }
};
