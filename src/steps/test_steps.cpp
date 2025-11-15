#include "../../include/step.hpp"
#include "../../include/dialogs.hpp"

class StepTest : public Step {
public:
    StepResult run() override {
        dialogs::dialog_text(stdscr, "Test Step", "This is a step example.");
        return StepResult::Next;
    }
};
