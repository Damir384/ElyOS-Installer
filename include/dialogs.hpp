#ifndef DIALOGS_HPP
#define DIALOGS_HPP

#include <string>
#include <ncurses.h>

namespace dialogs {
    void info(const std::string &title, const std::string &message);
    bool yesno(const std::string &title, const std::string &question);
}

#endif
