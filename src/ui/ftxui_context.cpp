#include "ftxui_context.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

FTXUIContext::FTXUIContext() 
    : m_app_name(ELY_APP_NAME), m_app_version(ELY_VERSION), m_mode_name("") {}

void FTXUIContext::init_screen() {
    m_screen = std::make_unique<ftxui::ScreenInteractive>(ftxui::ScreenInteractive::Fullscreen());
}

void FTXUIContext::shutdown_screen() {
    m_screen.reset();
}

void FTXUIContext::set_mode_name(const std::string& mode_name) {
    m_mode_name = mode_name;
}

// Единый шаблонный рендерер (Шапка + Фиксированный бокс 76x20 + borderDouble)
ftxui::Element FTXUIContext::build_layout(const std::string& step_title, ftxui::Element content, ftxui::Component buttons_container) {
    using namespace ftxui;

    // 1. Формируем строку статуса слева сверху
    std::string header_text = m_app_name + " v" + m_app_version;
    if (!m_mode_name.empty()) {
        header_text += " [" + m_mode_name + "]";
    }

    auto header = text(header_text) | bold | color(Color::Cyan);

    // 2. Тело фиксированного окна с двойной рамкой
    auto window_box = vbox({
        text(" " + step_title + " ") | bold | color(Color::Yellow),
        separator(),
        content | flex,
        separator(),
        buttons_container->Render() | center
    }) | borderDouble 
       | size(WIDTH, EQUAL, 76) 
       | size(HEIGHT, EQUAL, 20);

    return vbox({
        header,
        window_box
    }) | center;
}

// -------------------------------------------------------------
// 1. Экран приветствия (Welcome)
// -------------------------------------------------------------
StepResult FTXUIContext::show_welcome() {
    using namespace ftxui;
    StepResult outcome = StepResult::CANCEL;

    auto btn_next = Button("Дальше >", [&] { outcome = StepResult::NEXT; m_screen->Exit(); });
    auto btn_exit = Button("Выход [ESC]", [&] { outcome = StepResult::CANCEL; m_screen->Exit(); });
    
    auto buttons = Container::Horizontal({btn_exit, btn_next});

    auto content = vbox({
        text("Добро пожаловать в установщик операционной системы ElyOS!") | center,
        separatorEmpty(),
        text("Автономия — это не изоляция, а независимость.") | italic | center,
        separatorEmpty(),
        text("Данный мастер подготовит накопители и развернет базовый образ Arch.") | center,
        text("Используйте стрелки для навигации, Enter для выбора и ESC для отмены.") | dim | center
    }) | center;

    auto component = Renderer(buttons, [&] {
        return build_layout("Приветствие", content, buttons);
    });

    // Перехват клавиши ESC
    component |= CatchEvent([&](Event event) {
        if (event == Event::Escape) {
            outcome = StepResult::CANCEL;
            m_screen->Exit();
            return true;
        }
        return false;
    });

    m_screen->Loop(component);
    return outcome;
}

// -------------------------------------------------------------
// 2. Меню выбора диска (Disks Menu)
// -------------------------------------------------------------
StepResult FTXUIContext::show_disks_menu(const std::map<std::string, std::string>& disks_map, std::string& out_selected_path) {
    using namespace ftxui;
    StepResult outcome = StepResult::CANCEL;

    std::vector<std::string> entries;
    std::vector<std::string> paths;

    for (const auto& [path, info] : disks_map) {
        paths.push_back(path);
        entries.push_back(path + " (" + info + ")");
    }

    if (entries.empty()) {
        entries.push_back("Накопители не обнаружены!");
    }

    int selected = 0;
    auto radiobox = Radiobox(&entries, &selected);

    auto btn_back = Button("< Назад", [&] { outcome = StepResult::BACK; m_screen->Exit(); });
    auto btn_next = Button("Дальше >", [&] { 
        if (!paths.empty()) {
            out_selected_path = paths[selected];
            outcome = StepResult::NEXT; 
            m_screen->Exit();
        }
    });

    auto buttons = Container::Horizontal({btn_back, btn_next});
    auto main_container = Container::Vertical({radiobox, buttons});

    auto content = vbox({
        text("Выберите целевой диск для разметки и установки:") | color(Color::White),
        separatorEmpty(),
        radiobox->Render() | frame | size(HEIGHT, LESS_THAN, 8)
    });

    auto component = Renderer(main_container, [&] {
        return build_layout("Выбор накопителя", content, buttons);
    });

    component |= CatchEvent([&](Event event) {
        if (event == Event::Escape) {
            outcome = StepResult::CANCEL;
            m_screen->Exit();
            return true;
        }
        return false;
    });

    m_screen->Loop(component);
    return outcome;
}

// -------------------------------------------------------------
// 3. Универсальное меню (например, выбор режима)
// -------------------------------------------------------------
StepResult FTXUIContext::show_menu(const std::string& title, const std::vector<std::string>& options, int& out_selected_index) {
    using namespace ftxui;
    StepResult outcome = StepResult::CANCEL;

    int selected = out_selected_index;
    auto radiobox = Radiobox(&options, &selected);

    auto btn_back = Button("< Назад", [&] { outcome = StepResult::BACK; m_screen->Exit(); });
    auto btn_next = Button("Дальше >", [&] { 
        out_selected_index = selected;
        outcome = StepResult::NEXT; 
        m_screen->Exit(); 
    });

    auto buttons = Container::Horizontal({btn_back, btn_next});
    auto main_container = Container::Vertical({radiobox, buttons});

    auto content = vbox({
        radiobox->Render() | frame
    });

    auto component = Renderer(main_container, [&] {
        return build_layout(title, content, buttons);
    });

    component |= CatchEvent([&](Event event) {
        if (event == Event::Escape) {
            outcome = StepResult::CANCEL;
            m_screen->Exit();
            return true;
        }
        return false;
    });

    m_screen->Loop(component);
    return outcome;
}

// -------------------------------------------------------------
// 4. Поле ввода (Input Box)
// -------------------------------------------------------------
StepResult FTXUIContext::show_input(const std::string& title, const std::string& placeholder, std::string& out_value) {
    using namespace ftxui;
    StepResult outcome = StepResult::CANCEL;

    std::string buffer = out_value;
    auto input_comp = Input(&buffer, placeholder);

    auto btn_back = Button("< Назад", [&] { outcome = StepResult::BACK; m_screen->Exit(); });
    auto btn_next = Button("Дальше >", [&] { 
        out_value = buffer;
        outcome = StepResult::NEXT; 
        m_screen->Exit(); 
    });

    auto buttons = Container::Horizontal({btn_back, btn_next});
    auto main_container = Container::Vertical({input_comp, buttons});

    auto content = vbox({
        text("Введите требуемое значение:") | dim,
        separatorEmpty(),
        input_comp->Render() | borderLight
    });

    auto component = Renderer(main_container, [&] {
        return build_layout(title, content, buttons);
    });

    component |= CatchEvent([&](Event event) {
        if (event == Event::Escape) {
            outcome = StepResult::CANCEL;
            m_screen->Exit();
            return true;
        }
        return false;
    });

    m_screen->Loop(component);
    return outcome;
}

// -------------------------------------------------------------
// 5. Диалог подтверждения (Confirmation)
// -------------------------------------------------------------
StepResult FTXUIContext::show_confirmation(const std::string& prompt) {
    using namespace ftxui;
    StepResult outcome = StepResult::BACK;

    auto btn_no = Button("Нет (< Назад)", [&] { outcome = StepResult::BACK; m_screen->Exit(); });
    auto btn_yes = Button("Да (Дальше >)", [&] { outcome = StepResult::NEXT; m_screen->Exit(); });

    auto buttons = Container::Horizontal({btn_no, btn_yes});

    auto content = vbox({
        text("ВНИМАНИЕ!") | bold | color(Color::Red) | center,
        separatorEmpty(),
        text(prompt) | center,
        separatorEmpty(),
        text("Действие необратимо. Убедитесь в правильности выбора.") | dim | center
    }) | center;

    auto component = Renderer(buttons, [&] {
        return build_layout("Подтверждение действия", content, buttons);
    });

    component |= CatchEvent([&](Event event) {
        if (event == Event::Escape) {
            outcome = StepResult::CANCEL;
            m_screen->Exit();
            return true;
        }
        return false;
    });

    m_screen->Loop(component);
    return outcome;
}