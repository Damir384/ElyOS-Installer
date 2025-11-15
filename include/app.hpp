#pragma once
#include <string>
#include <ncurses.h>

class App {
public:
    App(int argc, char** argv);
    int run();
    WINDOW* getMainWindow() const { return mainWin; }
private:
    int argc;
    char** argv;
    WINDOW* mainWin;
    void init();
    void shutdown();
    int mainMenu();
    void mainInfo();
};
