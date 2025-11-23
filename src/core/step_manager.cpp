#include "step_manager.hpp"

void StepManager::add(std::unique_ptr<Step> step) {
    steps.push_back(std::move(step));
}

StepResult StepManager::run(WINDOW* win) {
    int index = 0;
    auto& ctx = InstallerContext::get();
    
    while (index >= 0 && index < static_cast<int>(steps.size())) {
        StepResult result;

        result = steps[index]->run(win);
        
        switch (result) {
            case StepResult::Next:
                index++;
                break;
                
            case StepResult::Prev:
                index = (index > 0) ? index - 1 : 0;
                break;

            case StepResult::Skip:
                index = StepManager::skip();
                
            case StepResult::Exit:
                return StepResult::Exit;
        }
    }

    return StepResult::Exit; // завершение после прохода всех шагов
}

int StepManager::skip(){
    int index = 0;
    while (index >= 0 && index < static_cast<int>(steps.size())) {
        if (steps[index]->name() == selectAddPackages)
            return index;
    }
    return -1;
}
