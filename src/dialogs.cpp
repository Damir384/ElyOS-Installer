#include "../include/dialogs.hpp"

namespace dialogs {

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
        int string = 0;

        if (!content.empty())
            for (size_t i = 0; i < content.size(); i++) {
                if (content[i] == '\n'){
                    lines++;
                    if (string > 0){
                        lines += (string / (w-2));
                    }
                    if (i != (content.size() - 1))
                        string = 0;
                }else{
                    string++;
                }
            }
        if (string % (w-2))
            lines++;

        // Ограничение прокрутки пада 
        int max = (lines - (h - 1));


        if (max < -1)
            max = -1;

        WINDOW* pad = newpad(lines, w - 2);

        mvwprintw(pad, 0, 0, "%s", content.c_str());

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
                    return 0;
            }
        }

        delwin(pad);
        return 0;
    };

// int dialog_menu(WINDOW* win, const std::string& title, const std::vector<std::string>& items, int& choice) {
//     box(win, 0, 0);
//     mvwprintw(win, 0, 2, "%s", title.c_str());

//     int index = 0;
//     while (true) {
//         for (size_t i = 0; i < items.size(); i++) {
//             if ((int)i == index)
//                 wattron(win, A_REVERSE);
//             mvwprintw(win, 2 + i, 2, "%s", items[i].c_str());
//             wattroff(win, A_REVERSE);
//         }
//         wrefresh(win);

//         switch(wgetch(win)) {
//             case KEY_UP:   index = (index == 0) ? items.size() - 1 : index - 1; break;
//             case KEY_DOWN: index = (index + 1) % items.size(); break;
//             case 10: choice = index; return 0;
//         }
//     }
// }

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
                    return 0;
                case 27: // ESC → выход без выбора
                    return 1;
            }
        }
    }

}