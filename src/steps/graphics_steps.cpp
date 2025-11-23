#include "graphics_steps.hpp"

StepResult ChoiceGraphics::run(WINDOW* win) {
    std::vector<std::string> list = {"ElyOS Style"};
    int choice = 0;
    dialogs::dialog_menu(win, "Choice Graphic", list, choice);
    auto& ctx = InstallerContext::get();
    ctx.graphics = list[choice];
    if(choice == 0)
        return StepResult::Skip;
    return StepResult::Next;
}