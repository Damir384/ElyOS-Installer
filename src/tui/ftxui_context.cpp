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
// Экран приветствия
// -------------------------------------------------------------
StepResult FTXUIContext::show_welcome() {
    using namespace ftxui;
    StepResult outcome = StepResult::CANCEL;

    auto btn_next = Button("Proceed >", [&] { outcome = StepResult::NEXT; m_screen->Exit(); });
    auto btn_exit = Button("Exit [ESC]", [&] { outcome = StepResult::CANCEL; m_screen->Exit(); });
    
    int default_btn = 1;
    auto buttons = Container::Horizontal({btn_exit, btn_next}, &default_btn);

    auto content = vbox({
        text("Welcome to the ElyOS installer!") | center,
        separatorEmpty(),
        text("This wizard will prepare the storage devices and deploy a base Arch image.") | center,
        text("Use the arrow keys to navigate, Enter to select, and ESC to cancel.") | dim | center
    }) | center;

    auto component = Renderer(buttons, [&] {
        return build_layout("Welcome", content, buttons);
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
// Универсальное меню
// -------------------------------------------------------------
StepResult FTXUIContext::show_menu(const std::string& title, const std::vector<std::string>& options, int& out_selected_index) {
    using namespace ftxui;
    StepResult outcome = StepResult::CANCEL;

    // 1. Индекс выбора
    int selected = out_selected_index;
    if (selected < 0 || selected >= static_cast<int>(options.size())) {
        selected = 0;
    }

    auto radiobox = Radiobox(&options, &selected);

    // 2. Кнопки управления
    auto btn_back = Button("< Cancel", [&] { 
        outcome = StepResult::BACK; 
        m_screen->Exit(); 
    });

    auto btn_next = Button("Proceed >", [&] { 
        out_selected_index = selected;
        outcome = StepResult::NEXT; 
        m_screen->Exit(); 
    });

    int default_btn = 1;
    auto buttons = Container::Horizontal({btn_back, btn_next}, &default_btn);

    auto main_container = Container::Vertical({
        radiobox,
        buttons
    });

    auto component = Renderer(main_container, [&] {
        auto content = vbox({
            radiobox->Render() | frame | size(HEIGHT, LESS_THAN, 8)
        });

        return build_layout(title, content, buttons);
    });

    // 5. Перехват ESC
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

StepResult FTXUIContext::show_info(const std::string &title, const std::string &source_text)
{
    using namespace ftxui;
    StepResult outcome = StepResult::CANCEL;

    auto btn_back = Button("< Cancel", [&] { 
        outcome = StepResult::BACK; 
        m_screen->Exit(); 
    });

    auto btn_next = Button("Proceed >", [&] { 
        outcome = StepResult::NEXT; 
        m_screen->Exit(); 
    });

    int default_btn = 1;
    auto buttons = Container::Horizontal({btn_back, btn_next}, &default_btn);

    auto content = vbox({
        text(source_text) | center
    }) | center;

    auto component = Renderer(buttons, [&] {
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
// Поле ввода
// -------------------------------------------------------------
StepResult FTXUIContext::show_input(const std::string& title, const std::string& placeholder, std::string& out_value) {
    using namespace ftxui;
    StepResult outcome = StepResult::CANCEL;

    std::string buffer = out_value;
    auto input_comp = Input(&buffer, placeholder);

    auto btn_back = Button("< Cancel", [&] { 
        outcome = StepResult::BACK; 
        m_screen->Exit(); 
    });

    auto btn_next = Button("Proceed >", [&] { 
        out_value = buffer;
        outcome = StepResult::NEXT; 
        m_screen->Exit(); 
    });

    int default_btn = 1;
    auto buttons = Container::Horizontal({btn_back, btn_next}, &default_btn);
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
// Диалог подтверждения
// -------------------------------------------------------------
StepResult FTXUIContext::show_confirmation(const std::string& prompt) {
    using namespace ftxui;
    StepResult outcome = StepResult::BACK;

    auto btn_no = Button("Нет (< Cancel)", [&] { outcome = StepResult::BACK; m_screen->Exit(); });
    auto btn_yes = Button("Да (Proceed >)", [&] { outcome = StepResult::NEXT; m_screen->Exit(); });

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

void FTXUIContext::show_warning(const std::string &title, const std::string &message) {
    
}
