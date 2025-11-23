#pragma once
#include <ncurses.h>
#include "utils.hpp"

enum class StepResult {
    Next,
    Prev,
    Exit,
    Skip
};

class Step {
public:
    virtual ~Step() = default;
    virtual StepResult run(WINDOW* win) = 0;
    virtual std::string name() = 0;
};
