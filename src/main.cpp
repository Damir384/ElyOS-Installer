#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <locale.h>
#include <thread>
#include <chrono>
#include "dialogs.hpp"

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

int main() {
    start_msg(); //функция вывода логотипа системы в консоль

    setlocale(LC_ALL, "");  // поддержка всех локалей системы
    initscr(); // инициализация ncurses
    start_color(); // включение цветов
    cbreak(); // отключаем полный контроль над клавиатурой
    noecho(); // отключение вывода вводимых символов
    keypad(stdscr, TRUE); // включение полного захвата клавиатуры
    curs_set(0); // видимость курсора от 0 до 2

    int height = LINES - 2;
    int width = COLS - 4;
    WINDOW* win = newwin(height, width, 1, 2);
    keypad(win, TRUE);
    std::string title = "Test";
    std::string content = "Test message for function \"dialog_test\"";

    dialogs::dialog_text(win, title, content);

    endwin(); // завершение ncurses
}
