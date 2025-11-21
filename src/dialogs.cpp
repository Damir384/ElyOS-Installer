#include "../include/dialogs.hpp"

namespace dialogs {

    std::vector<std::string> words_and_spaces(std::string str) {
        std::stringstream ss(str);
        std::string word;
        std::vector<std::string> result;

        if (ss >> word) {
            result.push_back(word);
        }

        while (ss >> word) {
            result.push_back(" ");
            result.push_back(word);
        }

        return result;
    }

    void addstr_wordwrap(std::string & content, std::string & str, long unsigned int & width, int & lines) {
        int x = 0;
        if ((content.size()) <= width) {
            str += content.c_str();
            str += "\n";
            lines++;
        } else {
            for(auto word : words_and_spaces(content)) {
                if ((word.size() + x) <= width) {
                    str += word.c_str();
                    x += word.size();
                    lines++;
                } else {
                    str += "\n";
                    str += word.c_str();
                    lines++;
                    x = 0;
                }
            }
            str += "\n";
        }
    }

    /**
     * Функция отображения текста внутри окна win
     * 
     * @param win
     * принимает на вход указатель на окно типа WINDOW* и выводит текст внутри этого окна
     * 
     * @param title
     * принимает на вход строку типа string и выводит её как заголовок окра
     * 
     * @param content
     * принимает на вход строку типа string и выводит её как содержимое окна с возможностью прокрутки
     */
    int dialog_text(WINDOW* win, const std::string& title, const std::string& content){
        // Создаём рамку в окне win
        box(win, 0, 0);
        
        // Выводим заголовок title в окно win
        mvwprintw(win, 0, 1, "%s", title.c_str());
        
        // Выводим изменения
        wrefresh(win);

        // Размер окна win
        int h, w = 0;
        
        // Расположение окна win
        int wh, ww = 0;

        getmaxyx(win, h, w);
        getbegyx(win, wh, ww);

        int lines = 0;
        std::string line;
        std::string str;
        long unsigned int www = w-2;

        for (long unsigned int i = 0; i <= content.size(); i++) {
            if (content[i] == '\n' || i == content.size()){
                addstr_wordwrap(line, str, www, lines);
                line = "";
            } else {
                line += content[i];
            }
        }

        // Ограничение прокрутки пада 
        int max = (lines - (h - 1));


        if (max < -1)
            max = -1;

        WINDOW* pad = newpad(lines, w - 2);

        mvwaddstr(pad, 0, 0, str.c_str());



        // Скрол пада
        int scroll = 0;

        // Цикл обновления пада и логика управления выводом
        while (true) {
            prefresh(pad, scroll, 0, wh+1, ww+1, wh+h-2, ww+w);
            switch(wgetch(win)) {
                case KEY_UP:
                    if(scroll > 0)
                        scroll--;
                    break;
                case KEY_DOWN:
                    if(scroll <= max)
                        scroll++;
                    break;
                case KEY_HOME:
                    if(scroll)
                        scroll = 0;
                    break;
                case 10:
                case 27:
                    delwin(pad);
                    werase(win);
                    return 0;
            }
        }

        delwin(pad);
        werase(win);
        return 0;
    };

    int dialog_menu(WINDOW* win, const std::string& title, const std::vector<std::string>& items, int& choice) 
    {
        // Создаём рамку в окне win
        box(win, 0, 0);
        
        // Выводим заголовок title в окно win
        mvwprintw(win, 0, 1, "%s", title.c_str());
        
        // Выводим изменения
        wrefresh(win);

        int h, w;
        getmaxyx(win, h, w);

        int y, x;

        // текущий выбранный пункт
        int highlight = 0;

        while (true) {
            // Вывод пунктов меню
            for (int i = 0; i < (int)items.size(); i++) {
                y = i + 1;                // начинаем с 1 (с учётом рамки)
                x = 2;                    // немного отступаем от рамки

                // Если пункт выходит за рамку — прекращаем (окно слишком маленькое)
                if (y >= h - 2) break;

                if (i == highlight) {
                    wattron(win, A_REVERSE);
                    mvwprintw(win, y, x, "%s", items[i].c_str());
                    wattroff(win, A_REVERSE);
                } else {
                    mvwprintw(win, y, x, "%s", items[i].c_str());
                }
            }

            wrefresh(win);

            int c = wgetch(win);

            switch (c) {
                case KEY_UP:
                    highlight--;
                    if (highlight < 0) highlight = items.size() - 1;
                    break;

                case KEY_DOWN:
                    highlight++;
                    if (highlight >= (int)items.size()) highlight = 0;
                    break;

                case '\n': // Enter
                case KEY_ENTER:
                    choice = highlight;
                    werase(win);
                    return 0;
                case 27: // ESC → выход без выбора
                    werase(win);
                    return 1;
            }
        }
        werase(win);
        return 0;
    }

    int dialog_menu(WINDOW* win, const std::string& title, const std::map<std::string, std::string>& items, int& choice)
    {
        // Создаём рамку в окне win
        box(win, 0, 0);
        
        // Выводим заголовок title в окно win
        mvwprintw(win, 0, 1, "%s", title.c_str());
        
        // Выводим изменения
        wrefresh(win);

        int h, w;
        getmaxyx(win, h, w);

        int y, x;

        // текущий выбранный пункт
        int highlight = 0;

        while (true) {
            int i = 0;

            // Вывод пунктов меню
            for (const auto& [disk, name] : items) {
                y = i + 1;                // начинаем с 1 (с учётом рамки)
                x = 2;                    // немного отступаем от рамки

                // Если пункт выходит за рамку — прекращаем (окно слишком маленькое)
                if (y >= h - 2) break;

                if (i == highlight) {
                    wattron(win, A_REVERSE);
                    mvwprintw(win, y, x, "%s", disk.c_str());
                    x+=disk.size() + 1;
                    mvwprintw(win, y, x, "%s", name.c_str());
                    wattroff(win, A_REVERSE);
                } else {
                    mvwprintw(win, y, x, "%s", disk.c_str());
                    x+=disk.size() + 1;
                    mvwprintw(win, y, x, "%s", name.c_str());
                }
                i++;
            }

            wrefresh(win);

            int c = wgetch(win);

            switch (c) {
                case KEY_UP:
                    highlight--;
                    if (highlight < 0) highlight = items.size() - 1;
                    break;

                case KEY_DOWN:
                    highlight++;
                    if (highlight >= (int)items.size()) highlight = 0;
                    break;

                case '\n': // Enter
                case KEY_ENTER:
                    choice = highlight;
                    werase(win);
                    return 0;
                case 27: // ESC → выход без выбора
                    werase(win);
                    return 1;
            }
        }
        werase(win);
        return 0;
    }

}