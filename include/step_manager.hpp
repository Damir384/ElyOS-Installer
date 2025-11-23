#pragma once
#include "step.hpp"
#include "context.hpp"
#include <vector>
#include <memory>
#include <ncurses.h>
#include <algorithm>

class StepManager {
public:
    void add(std::unique_ptr<Step> step);

    StepResult run(WINDOW* win);
    std::string selectAddPackages;

private:
    std::vector<std::unique_ptr<Step>> steps;
    int skip();
};
