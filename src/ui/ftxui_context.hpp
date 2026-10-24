#pragma once

#include "ui_interface.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

class FTXUIContext : public UIContext {
public:
    FTXUIContext();
    ~FTXUIContext() override = default;

    void init_screen() override;
    void shutdown_screen() override;
    void set_mode_name(const std::string& mode_name) override;

    StepResult show_welcome() override;
    StepResult show_disks_menu(const std::map<std::string, std::string>& disks_map, std::string& out_selected_path) override;
    StepResult show_menu(const std::string& title, const std::vector<std::string>& options, int& out_selected_index) override;
    StepResult show_input(const std::string& title, const std::string& placeholder, std::string& out_value) override;
    StepResult show_confirmation(const std::string& prompt) override;

private:
    std::unique_ptr<ftxui::ScreenInteractive> m_screen;
    std::string m_app_name;
    std::string m_app_version;
    std::string m_mode_name;

    // Внутренний генератор единого стиля окна
    ftxui::Element build_layout(const std::string& step_title, ftxui::Element content, ftxui::Component buttons_container);
};