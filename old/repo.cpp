#include <ncurses.h>
#include <string>
#include <vector>
#include <algorithm>
#include <locale.h>

// -------------------------------
// Структура пакета
// -------------------------------
struct Package {
    std::string name;
    bool selected = false;
};

// -------------------------------
// Основная программа
// -------------------------------
int main() {
    setlocale(LC_ALL, "");  // чтобы русские буквы работали

    // --- Пакеты вручную ---
    std::vector<Package> all_packages = {
        {"base"}, {"bash"}, {"vim"}, {"zsh"}, {"gcc"},
        {"make"}, {"git"}, {"nano"}, {"neovim"}, {"htop"},
        {"grub"}, {"networkmanager"}, {"curl"}, {"wget"}, {"python"},
        {"cmake"}, {"gdb"}, {"alsa-utils"}, {"xorg"}, {"xfce4"}
    };

    std::vector<int> visible_indices;     // для фильтрации
    std::vector<std::string> selected_packages; // итоговый список

    std::string search_query;
    int search_cursor = 0;
    int list_cursor = 0;
    int scroll_offset = 0;

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    int height = LINES - 2;
    int width = COLS - 4;
    WINDOW* win = newwin(height, width, 1, 2);
	keypad(win, TRUE); // чтобы стрелки, F1-F12 и т.п. работали
	box(win, 0, 0);
	wrefresh(win);

    bool done = false;

    auto update_filter = [&]() {
        visible_indices.clear();
        for (int i = 0; i < (int)all_packages.size(); i++) {
            if (search_query.empty() ||
                all_packages[i].name.find(search_query) != std::string::npos)
                visible_indices.push_back(i);
        }
        if (list_cursor >= (int)visible_indices.size())
            list_cursor = visible_indices.empty() ? 0 : visible_indices.size() - 1;
        scroll_offset = 0;
    };

    update_filter();

    while (!done) {
        // --- рисуем всё ---
	werase(win);
	box(win, 0, 0);

	// 1. Поле поиска
	mvwprintw(win, 1, 2, "Поиск: %s", search_query.c_str());

	// 2. Разделительная линия
	mvwhline(win, 2, 1, '-', width - 2);

	// 3. Отрисовка списка пакетов
	int list_area_height = height - 6;
	for (int i = 0; i < list_area_height && i + scroll_offset < (int)visible_indices.size(); i++) {
    	int pkg_index = visible_indices[i + scroll_offset];
    	auto &pkg = all_packages[pkg_index];
    	bool highlighted = (i + scroll_offset == list_cursor);

    	if (highlighted) wattron(win, A_REVERSE);
    	mvwprintw(win, 3 + i, 2, "[%c] %s", pkg.selected ? 'x' : ' ', pkg.name.c_str());
    	if (highlighted) wattroff(win, A_REVERSE);
	}

	// 4. Список выбранных
	mvwhline(win, height - 3, 1, '-', width - 2);
	std::string selected_str;
	for (auto &pkg : all_packages)
    	if (pkg.selected)
        	selected_str += pkg.name + " ";
	mvwprintw(win, height - 2, 2, "Выбрано: %s", selected_str.c_str());

	// 5. <-- перемещаем курсор В САМОМ КОНЦЕ!
	wmove(win, 1, 9 + search_cursor);

	// 6. Обновляем экран
	wrefresh(win);


        int ch = wgetch(win);

        switch (ch) {
            case KEY_LEFT:
                if (search_cursor > 0) search_cursor--;
                break;

            case KEY_RIGHT:
                if (search_cursor < (int)search_query.size()) search_cursor++;
                break;

            case KEY_BACKSPACE:
            case 127:
                if (search_cursor > 0) {
                    search_query.erase(search_cursor - 1, 1);
                    search_cursor--;
                    update_filter();
                }
                break;

            case KEY_UP:
                if (list_cursor > 0) list_cursor--;
                if (list_cursor < scroll_offset) scroll_offset--;
                break;

            case KEY_DOWN:
                if (list_cursor + 1 < (int)visible_indices.size()) list_cursor++;
                if (list_cursor >= scroll_offset + list_area_height) scroll_offset++;
                break;

            case ' ': {
                if (!visible_indices.empty()) {
                    int idx = visible_indices[list_cursor];
                    all_packages[idx].selected = !all_packages[idx].selected;
                }
                break;
            }

            case '\n':
                done = true;
                break;

            default:
                if (isprint(ch)) {
                    search_query.insert(search_cursor, 1, (char)ch);
                    search_cursor++;
                    update_filter();
                }
                break;
        }
    }

    endwin();

    // --- Формируем итоговый список выбранных ---
    for (auto &pkg : all_packages)
        if (pkg.selected)
            selected_packages.push_back(pkg.name);

    // --- Вывод результатов ---
    printf("Вы выбрали %zu пакетов:\n", selected_packages.size());
    for (auto &name : selected_packages)
        printf(" - %s\n", name.c_str());

    return 0;
}
