#include "../../include/step.hpp"
#include <vector>
#include <memory>
#include <ncurses.h>

class StepManager {
public:
    void add(std::unique_ptr<Step> step) {
        steps.push_back(std::move(step));
    }

    StepResult run(WINDOW* win) {
        int index = 0;
        
        while (index >= 0 && index < static_cast<int>(steps.size())) {
            StepResult result = steps[index]->run(win);
            
            switch (result) {
                case StepResult::Next:
                    index++;
                    break;
                    
                case StepResult::Prev:
                    index = (index > 0) ? index - 1 : 0;
                    break;
                    
                case StepResult::Exit:
                    return StepResult::Exit;
            }
        }

        return StepResult::Exit; // завершение после прохода всех шагов
    }

private:
    std::vector<std::unique_ptr<Step>> steps;
};
