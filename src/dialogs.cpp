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
     * 
     * @param error
     * принимает на вход строку типа string и выводит её как содержимое окна с возможностью прокрутки
     */
    int dialog_text(WINDOW* win, std::string & title, std::string & content, std::string & error){
        // Создаём рамку в окне win
        box(win, 0, 0);

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

        // Выводим заголовок title в окно win
        mvwprintw(win, 0, 1, "%d", lines);

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
}
