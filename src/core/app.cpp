#include "app.hpp"

App::App(int argc, char** argv) : argc(argc), argv(argv), debug(false) {}

void App::init() {
    Logger::get().log(LogLevel::INFO, "Installer started");
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void App::shutdown() {
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
    std::string description = "Welcome to ElyOS installer alpha 0.1.0. This installer allows you to choose either an automatic or manual partitioning scheme.\n\n1. Auto (recommended) - Automatically partitions the disk and installs ElyOS.\n2. Manual - Allows you to manually partition the disk.\n3. ISO Build - Create a pre-configured ISO installer";
    dialogs::dialog_text(mainWin, "Title", description);
}

void App::getMinResolution(int & minH, int & minW) {
    if(!debug){
        if (LINES >= 37 && COLS >= 100) {
            minH = 37;
            minW = 100;
        } else {
            minH = 0;
            minW = 0;
        }
    } else {
        minH = LINES-2;
        minW = COLS-4;
        return;
    }
}

// void App::initColors() {
//     if (!has_colors()) return;
//     start_color();
//     use_default_colors();
    
//     init_pair(Colors::MAIN, COLOR_WHITE, COLOR_BLUE);
//     init_pair(Colors::HEADER, COLOR_BLACK, COLOR_CYAN);
//     init_pair(Colors::ALERT, COLOR_YELLOW, COLOR_BLACK);
//     init_pair(Colors::ERROR, COLOR_RED, COLOR_WHITE);
// }

int App::run() {
    std::string arg;
    if (argc > 1){
        for (int i = 1; i<argc; i++){
            if (std::string(argv[i]) == "--debug")
                debug = true;
        }
    }
    init();
    int minH = 0, minW = 0;
    getMinResolution(minH, minW);
    if (minH == 0 || minW == 0) {
        shutdown();
        return 1;
    }
    mainWin = ui::createMainWindow(minH, minW);
    mainInfo();
    int mode = mainMenu();
    if (mode == 0) {
        AutoInstall AutoInstall;
        AutoInstall.run(getMainWindow());
    }
    ui::deleteMainWindow(mainWin);
    shutdown();
    return 0;
}