#ifndef DIALOGS_HPP
#define DIALOGS_HPP

#include <string>
#include <ncurses.h>

namespace dialogs {
    int dialog_text(WINDOW* win, std::string title, std::string content);
}

#endif
