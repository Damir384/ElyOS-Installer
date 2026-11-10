#pragma once

#include "core/install_context.hpp"
#include <string>
#include <vector>
#include <map>

/**
 * @brief Abstract UI interface delegate decoupling core logic from presentation layers.
 * @details Implemented by frontends (TUI, GUI, or headless CLI). Interactive methods
 * return a StepResult navigation state.
 */
class UIContext {
public:
    virtual ~UIContext() = default;

    /**
     * @brief Initializes display canvas (e.g., switches terminal to alternate screen buffer).
     */
    virtual void init_screen() = 0;

    /**
     * @brief Releases display resources and restores default terminal state.
     */
    virtual void shutdown_screen() = 0;

    /**
     * @brief Configures display header title reflecting current operational mode.
     * @param [in] mode_name Name of the mode (e.g., "Normal Mode").
     */
    virtual void set_mode_name(const std::string& mode_name) = 0;

    /**
     * @brief Renders the welcome view.
     * @return StepResult NEXT on proceed, CANCEL on exit or Escape key.
     */
    virtual StepResult show_welcome() = 0;

    /**
     * @brief Displays an interactive single-choice selection menu (Radiobox list).
     * @param [in] title Window header text.
     * @param [in] options Vector of selectable string options.
     * @param [in,out] out_selected_index Currently selected zero-based index.
     * @return StepResult NEXT on confirmation, BACK on cancel, CANCEL on Escape.
     */
    virtual StepResult show_menu(
        const std::string& title,
        const std::vector<std::string>& options,
        int& out_selected_index
    ) = 0;

    /**
     * @brief Displays a read-only text viewer screen.
     * @param [in] title Header label.
     * @param [in] text Body content to display.
     * @return StepResult Navigation status (NEXT, BACK, CANCEL).
     */
    virtual StepResult show_info(
        const std::string& title,
        const std::string& text
    ) = 0;

    /**
     * @brief Displays a single-line text input field.
     * @param [in] title Header label.
     * @param [in] placeholder Ghost text inside empty field.
     * @param [in,out] out_value Target string receiving user input.
     * @return StepResult NEXT on confirmation, BACK/CANCEL otherwise.
     */
    virtual StepResult show_input(
        const std::string& title, 
        const std::string& placeholder, 
        std::string& out_value
    ) = 0;

    /**
     * @brief Displays a modal confirmation dialog (Yes / No).
     * @param [in] prompt Warning description or question.
     * @return StepResult NEXT on confirm, BACK on decline, CANCEL on Escape.
     */
    virtual StepResult show_confirmation(const std::string& prompt) = 0;

    /**
     * @brief Displays a blocking warning modal dialog with an OK confirmation.
     * @details Does not return navigation results; dismisses cleanly on acknowledge.
     * @param [in] title Warning box header.
     * @param [in] message Explanatory text body.
     */
    virtual void show_warning(const std::string& title, const std::string& message) = 0;
};