#include <iostream>
#include <ncurses.h>
#include "dialogs.hpp"

int main() {
    initscr();
    noecho();
    cbreak();

    dialogs::info("Hello", "This is a test dialog");
    bool result = dialogs::yesno("Question", "Do you like ncurses?");

    endwin();

    std::cout << "Answer: " << (result ? "YES" : "NO") << std::endl;
    return 0;
}
