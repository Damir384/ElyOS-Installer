#include "step.hpp"
#include <vector>
#include <memory>
#include <ncurses.h>

class StepManager {
public:
    void add(std::unique_ptr<Step> step);

    StepResult run(WINDOW* win);

private:
    std::vector<std::unique_ptr<Step>> steps;
};
