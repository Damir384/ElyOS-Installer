#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <locale.h>
#include <thread>
#include <chrono>

int dialog_text(WINDOW * win) {
    bool done = false;
    std::string test = "test";
    while(!done){
	    box(win, 0, 0);
        int ch = wgetch(win);
        switch (ch) {
            case 'q':
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                done = true;
                break;
            case KEY_BACKSPACE:
                if (test.length() != 0){
                    test.pop_back();
                    mvwprintw(win, 1, 1, " ", test.length()+1);
                    mvwprintw(win, 1, 1, "%s ", test.c_str());
                }
                break;
            default:
                test += static_cast<char>(ch);
                mvwprintw(win, 1, 1, "%s", test.c_str());
                break;
        }
        wrefresh(win);
    }
    return 0;
}

int main() {
    std::cout << "┌────────────────────┐" << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // задержка 200 мс
    std::cout << "│░█▀▀░█░░░█░█░█▀█░█▀▀│" << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "│░█▀▀░█░░░░█░░█░█░▀▀█│" << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "│░▀▀▀░▀▀▀░░▀░░▀▀▀░▀▀▀│" << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "└────────────────────┘" << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));


    setlocale(LC_ALL, "");  // чтобы русские буквы работали
    initscr(); // инициализация ncurses
    start_color(); // включение цветов
    cbreak(); // 
    noecho(); // отключение вывода вводимых символов
    keypad(stdscr, TRUE); // включение полного захвата клавиатуры
    curs_set(1); // видимость курсора от 0 до 2

    move(0,2);
    printw("ElyOS Installer");
    refresh();

    int height = LINES - 2;
    int width = COLS - 4;
    WINDOW* win = newwin(height, width, 1, 2);
    keypad(win, TRUE); // включение полного захвата клавиатуры

    dialog_text(win);

    endwin(); // завершение ncurses
}