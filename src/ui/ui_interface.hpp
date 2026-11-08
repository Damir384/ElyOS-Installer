#pragma once

#include "core/install_context.hpp"
#include <string>
#include <vector>
#include <map>

class UIContext {
public:
    virtual ~UIContext() = default;

    virtual void init_screen() = 0;
    virtual void shutdown_screen() = 0;
    virtual void set_mode_name(const std::string& mode_name) = 0;
    
    virtual StepResult show_welcome() = 0;

    virtual StepResult show_menu(
        const std::string& title,
        const std::vector<std::string>& options,
        int& out_selected_index
    ) = 0;

    virtual StepResult show_info(
        const std::string& title,
        const std::string& text
    ) = 0;

    virtual StepResult show_input(
        const std::string& title, 
        const std::string& placeholder, 
        std::string& out_value
    ) = 0;

    virtual StepResult show_confirmation(const std::string& prompt) = 0;
};