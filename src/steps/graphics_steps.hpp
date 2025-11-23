#pragma once
#include "step.hpp"
#include "dialogs.hpp"
#include "context.hpp"
#include "logger.hpp"
#include <ncurses.h>
#include <vector>

class ChoiceGraphics : public Step {
public:
    StepResult run(WINDOW* win) override;
    std::string name() override {
        return "ChoiceGraphics";
    };
};
