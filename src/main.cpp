#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <locale.h>
#include <thread>
#include <chrono>
#include "dialogs.hpp"

/**
 * Функция вывода логотипа в консоль
 */
void start_msg() {
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
}

/**
 * Функция получения размера окна по текущему разрешению консоли
 */
bool get_resolution(int & height, int & width) {
    if (COLS < 100 || LINES < 37) {
        return false;
    } else {
        height = 35;
        width = 96;
        return true;
    }
}

/**
 * Стартует сессию ncurses
 */
void session_start() {
    setlocale(LC_ALL, "");  // поддержка всех локалей системы
    initscr(); // инициализация ncurses
    start_color(); // включение цветов
    cbreak(); // отключаем полный контроль над клавиатурой
    noecho(); // отключение вывода вводимых символов
    keypad(stdscr, TRUE); // включение полного захвата клавиатуры
    curs_set(0); // видимость курсора от 0 до 2
}

int main(int argc, char* argv[]) {

    bool debug = false;

    if (argc > 1){
        for (int i = 0; i<argc; i++){
            if (std::string(argv[i]) != "--debug")
                debug = true;
        }
    }

    int height = 0;
    int width = 0;

    // Проверка режима работы программы
    if (!debug){
        start_msg();
        session_start();
        if(!get_resolution(height, width)){
            endwin();
            std::cout << "Minimal resolution is 100x37" << std::endl;
            return 1;
        }
    }else {
        session_start();
        height = LINES - 2;
        width = COLS - 4;
    }

    WINDOW* win = newwin(height, width, 1, 2);
    keypad(win, TRUE);
    std::string title = "Test";
    std::string content = "Test message for function \"dialog_test\"";

    dialogs::dialog_text(win, title, content);

    endwin(); // завершение ncurses
}
