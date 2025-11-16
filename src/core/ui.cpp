#include "../../include/ui.hpp"

namespace ui {

WINDOW* createMainWindow(int minH, int minW) {
    int y = (LINES - minH) / 2;
    int x = (COLS - minW) / 2;

    WINDOW* win = newwin(minH, minW, y, x);
    keypad(win, TRUE);
    return win;
}

WINDOW* createPopup(int h, int w, int y, int x) {
    WINDOW* win = newwin(h, w, y, x);
    keypad(win, TRUE);
    return win;
}

void deleteMainWindow(WINDOW* win) {
    delwin(win);
}

void deletePopup(WINDOW* win) {
    delwin(win);
}

}
