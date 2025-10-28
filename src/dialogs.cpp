#include "dialogs.hpp"

namespace dialogs {
    int dialog_text(WINDOW* win, std::string title, std::string content){
        box(win, 0, 0);
        mvwprintw(win, 0, 1, "%s", title.c_str());
        wrefresh(win);
        int h, w;
        getmaxyx(win, h, w);

        int text_lines = content.size() / w;

        WINDOW* pad = newpad(text_lines + 1, w - 2);

        mvwprintw(pad, 0, 0, "%s", content.c_str());

        int scroll = 0;

        while (true) {
            prefresh(pad, scroll, 0, 2, 3, h - 1, w - 1);

            int key = wgetch(win);
            if (key == KEY_UP && scroll >= 1)
                scroll--;
            else if (key == KEY_DOWN && scroll <= text_lines){
                scroll++;
            }
            else if (key == 10 || key == 27)
                break;
        }

        delwin(pad);
        return 0;
    };
}
