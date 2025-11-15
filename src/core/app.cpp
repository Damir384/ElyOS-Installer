#include "app.hpp"
#include "dialogs.hpp"
#include <ncurses.h>
#include <vector>
#include <string>
#include "../modes/auto_install.cpp"
#include "../modes/manual_install.cpp"
#include "logger.hpp"
#include "ui.hpp"

App::App(int argc, char** argv) : argc(argc), argv(argv) {}

void App::init() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    std::string err;
    // mainWin = ui::createMainWindow(37, 100, err);
    mainWin = ui::createMainWindow(24, 80, err);
    Logger::get().log(LogLevel::INFO, "Installer started");
}

void App::shutdown() {
    ui::deleteMainWindow(mainWin);
    Logger::get().log(LogLevel::INFO, "Installer stoped");
    endwin();
}

int App::mainMenu() {
    std::vector<std::string> items = {"Auto Install", "Manual Install", "Exit"};
    int choice = 0;
    dialogs::dialog_menu(mainWin, "Select Mode", items, choice);
    return choice;
}

void App::mainInfo() {
    std::string description = "Welcome to ElyOS installer.\nFor continue press Enter";
    dialogs::dialog_text(mainWin, "Welcome", description);
}

int App::run() {
    init();
    mainInfo();
    int mode = mainMenu();
    shutdown();
    return 0;
}