#include "step.hpp"
#include "dialogs.hpp"
#include <ncurses.h>

class StepTest : public Step {
public:
    StepResult run(WINDOW* win) override {
        dialogs::dialog_text(win, "Test Step fdsg", "This is a step example.");
        return StepResult::Next;
    }
};
