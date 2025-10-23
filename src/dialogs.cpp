#include "dialogs.hpp"
#include <cstring>

namespace dialogs {

void info(const std::string &title, const std::string &message) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int win_height = 7;
    int win_width = cols / 2;
    int start_y = (rows - win_height) / 2;
    int start_x = (cols - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);

    mvwprintw(win, 1, (win_width - title.size()) / 2, "%s", title.c_str());
    mvwprintw(win, 3, (win_width - message.size()) / 2, "%s", message.c_str());
    mvwprintw(win, 5, (win_width - 14) / 2, "Press any key");

    wrefresh(win);
    getch();
    delwin(win);
    clear();
    refresh();
}

bool yesno(const std::string &title, const std::string &question) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int win_height = 7;
    int win_width = cols / 2;
    int start_y = (rows - win_height) / 2;
    int start_x = (cols - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);

    mvwprintw(win, 1, (win_width - title.size()) / 2, "%s", title.c_str());
    mvwprintw(win, 3, (win_width - question.size()) / 2, "%s", question.c_str());
    mvwprintw(win, 5, (win_width - 18) / 2, "[Y] Yes   [N] No");

    wrefresh(win);

    int ch;
    while (true) {
        ch = getch();
        if (ch == 'y' || ch == 'Y') { delwin(win); return true; }
        if (ch == 'n' || ch == 'N') { delwin(win); return false; }
    }
}
}
