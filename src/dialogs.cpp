#include "dialogs.hpp"

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
    int dialog_text(WINDOW* win, std::string title, std::string content){
        // Создаём рамку в окне win
        box(win, 0, 0);

        // Выводим заголовок title в окно win
        mvwprintw(win, 0, 1, "%s", title.c_str());

        // Выводим изменения
        wrefresh(win);

        // Расположение окна win
        int h, w = 0;

        // Размер окна win
        int wh, ww = 0;

        getmaxyx(win, h, w);
        getbegyx(win, wh, ww);

        // Общая высота пада
        int pad_h = 1;

        for (size_t i = 0; i < content.size(); i++){
            if (content[i] == '\n')
                pad_h++;
        }

        // Общее количество строк выводимых в паде
        int text_lines = ((content.size() - pad_h) / (w - 2)) + pad_h;

        // Ограничение прокрутки пада 
        int max = (text_lines - h + 1);

        if (max < -1)
            max = -1;

        WINDOW* pad = newpad(text_lines, w - 2);

        mvwprintw(pad, 0, 0, "%s", content.c_str());

        // Скрол пада
        int scroll = 0;

        // Цикл обновления пада и логика управления выводом
        while (true) {
            prefresh(pad, scroll, 0, wh+1, ww+1, wh+h-1, ww+w);
            int key = wgetch(win);
            if (key == KEY_UP && scroll >= 1)
                scroll--;
            else if (key == KEY_DOWN && scroll <= max)
                scroll++;
            else if (key == KEY_HOME && scroll)
                scroll = 0;
            else if (key == 10 || key == 27){
                delwin(pad);
                return 0;
            }
        }

        delwin(pad);
        return 0;
    };
}
