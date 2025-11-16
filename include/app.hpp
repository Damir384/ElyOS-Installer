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
    bool debug;
    WINDOW* mainWin;
    void init();
    // void initColors();
    void shutdown();
    int mainMenu();
    void mainInfo();
    void getMinResolution(int & minH, int & minW);
};
