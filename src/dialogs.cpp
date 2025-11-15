#include "../include/dialogs.hpp"

namespace dialogs {

int dialog_text(WINDOW* win, const std::string& title, const std::string& content) {
    box(win, 0, 0);
    mvwprintw(win, 0, 2, "%s", title.c_str());
    mvwprintw(win, 2, 2, "%s", content.c_str());
    wrefresh(win);
    wgetch(win);
    return 0;
}

int dialog_menu(WINDOW* win, const std::string& title, const std::vector<std::string>& items, int& choice) {
    box(win, 0, 0);
    mvwprintw(win, 0, 2, "%s", title.c_str());

    int index = 0;
    while (true) {
        for (size_t i = 0; i < items.size(); i++) {
            if ((int)i == index)
                wattron(win, A_REVERSE);
            mvwprintw(win, 2 + i, 2, "%s", items[i].c_str());
            wattroff(win, A_REVERSE);
        }
        wrefresh(win);

        switch(wgetch(win)) {
            case KEY_UP:   index = (index == 0) ? items.size() - 1 : index - 1; break;
            case KEY_DOWN: index = (index + 1) % items.size(); break;
            case 10: choice = index; return 0;
        }
    }
}
}
