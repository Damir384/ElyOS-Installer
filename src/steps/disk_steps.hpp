#pragma once
#include "step.hpp"
#include "dialogs.hpp"
#include <ncurses.h>
#include "json.hpp"
#include <vector>

class ShowPartitionInfo : public Step {
public:
    StepResult run(WINDOW* win) override;
};

class ShowDisks : public Step {
public:
    StepResult run(WINDOW* win) override;
};
