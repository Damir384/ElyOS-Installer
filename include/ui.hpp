#pragma once
#include <ncurses.h>
#include <string>

namespace ui {
    WINDOW* createMainWindow(int minH, int minW, std::string& err);
    WINDOW* createPopup(int h, int w, int y, int x);
    void deleteMainWindow(WINDOW* win);
    void deletePopup(WINDOW* win);
}
