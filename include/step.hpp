#pragma once
#include <ncurses.h>
#include "utils.hpp"

enum class StepResult {
    Next,
    Prev,
    Exit
};

class Step {
public:
    virtual ~Step() = default;
    virtual StepResult run(WINDOW* win) = 0;
};
