#pragma once
#include <string>

class App {
public:
    App(int argc, char** argv);
    int run();
private:
    int argc;
    char** argv;
    void init();
    void shutdown();
    int mainMenu();
};
