#pragma once
#include "step.hpp"
#include "dialogs.hpp"
#include <ncurses.h>

class ShowPartitionInfo : public Step {
public:
    StepResult run(WINDOW* win) override;
};

class ShowDisks : public Step {
public:
    StepResult run(WINDOW* win) override;
};
