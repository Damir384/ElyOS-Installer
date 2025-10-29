#include "dialogs.hpp"

namespace dialogs {
    int dialog_text(WINDOW* win, std::string title, std::string content){
        box(win, 0, 0);
        mvwprintw(win, 0, 1, "%s", title.c_str());
        wrefresh(win);
        int h, w = 0;
        getmaxyx(win, h, w);

        int pad_h = 1;

        for (size_t i = 0; i < content.size(); i++){
            if (content[i] == '\n')
                pad_h++;
        }

        int text_lines = ((content.size() - pad_h) / (w - 2)) + pad_h;

        int max = (text_lines - h + 1);

        if (max < -1)
            max = -1;

        WINDOW* pad = newpad(text_lines, w - 2);

        mvwprintw(pad, 0, 0, "%s", content.c_str());

        int scroll = 0;

        while (true) {
            prefresh(pad, scroll, 0, 2, 3, h-1, w);
            int key = wgetch(win);
            if (key == KEY_UP && scroll >= 1)
                scroll--;
            else if (key == KEY_DOWN && scroll <= max)
                scroll++;
            else if (key == KEY_HOME && scroll)
                scroll = 0;
            else if (key == 10 || key == 27)
                break;
        }

        delwin(pad);
        return 0;
    };
}
