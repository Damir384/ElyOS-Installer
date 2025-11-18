#pragma once
#include <string>
#include <vector>
#include <ncurses.h>
#include <sstream>

namespace dialogs {
    int dialog_text(WINDOW* win, const std::string& title, const std::string& content);
    int dialog_menu(WINDOW* win, const std::string& title, const std::vector<std::string>& items, int& choice);
}
