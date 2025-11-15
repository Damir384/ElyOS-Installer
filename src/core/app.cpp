#include "app.hpp"
#include "../../include/dialogs.hpp"
#include <ncurses.h>
#include <vector>
#include <string>
#include "../modes/auto_install.cpp"
#include "../modes/manual_install.cpp"

App::App(int argc, char** argv) : argc(argc), argv(argv) {}

void App::init() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void App::shutdown() {
    endwin();
}

int App::mainMenu() {
    WINDOW* win = stdscr;
    std::vector<std::string> items = {"Auto Install", "Manual Install", "Exit"};
    int choice = 0;
    dialogs::dialog_menu(win, "Select Mode", items, choice);
    return choice;
}

int App::run() {
    init();
    int mode = mainMenu();
    shutdown();
    return 0;
}
