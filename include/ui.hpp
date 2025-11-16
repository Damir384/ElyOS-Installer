#pragma once
#include <ncurses.h>
#include <string>

// namespace Colors {
//     constexpr short MAIN = 1;
//     constexpr short HEADER = 2;
//     constexpr short ALERT = 3;
//     constexpr short ERROR = 4;
// }

namespace ui {
    WINDOW* createMainWindow(int minH, int minW);
    WINDOW* createPopup(int h, int w, int y, int x);
    void deleteMainWindow(WINDOW* win);
    void deletePopup(WINDOW* win);
}
