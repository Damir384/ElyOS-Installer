#include "../../include/step.hpp"
#include "../../include/dialogs.hpp"
#include <ncurses.h>

class StepTest : public Step {
public:
    StepResult run(WINDOW* win) override {
        dialogs::dialog_text(win, "Test Step", "This is a step example.");
        return StepResult::Next;
    }
};
