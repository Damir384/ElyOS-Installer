#include "steps_common.hpp"
#include "utils/logger.hpp"
#include "core/install_context.hpp"

namespace core::steps {

    std::vector<Step> get_initial_steps() {
        return {
            {"Welcome", welcome_step},
            {"Information", information_step},
            {"Selection mode", select_mode}
            // Пример: {"Новый шаг", template_step}
        };
    }

    // =========================================================================
    // ЭТАЛОННЫЙ ШАБЛОН ШАГА (Копировать отсюда)
    // =========================================================================
    StepResult template_step(InstallContext& ctx) {
        // 1. Проверка предусловий (Defensive programming)
        if (!ctx.ui) {
            Logger::instance().log(LogLevel::ERROR, "UI Context is missing in step execution.");
            return StepResult::FATAL_ERROR;
        }

        // 2. Выполнение системной логики (если требуется до показа UI)
        // Пример: auto data = utils::some_action();

        // 3. Вызов UI и передача управления пользователю
        // Пример вызова подтверждения:
        // StepResult res = ctx.ui->show_confirmation("Вы уверены?");

        // 4. Обработка полезной нагрузки при подтверждении
        // if (res == StepResult::NEXT) {
        //     ctx.some_field = user_value;
        // }

        // 5. Возврат статуса навигации в StateMachine
        return StepResult::NEXT;
    }
    StepResult welcome_step(InstallContext& ctx) {
        if (!ctx.ui) {
            return StepResult::FATAL_ERROR;
        }

        return ctx.ui->show_welcome();
    }

    StepResult information_step(InstallContext& ctx) {
        if (!ctx.ui) {
            Logger::instance().log(LogLevel::ERROR, "UI Context is missing in step execution.");
            return StepResult::FATAL_ERROR;
        }

        std::string title = "Information";
        std::string text = R"(This installer is intended for installing the original Arch Linux image. Optionally, you can install the ElyOS family DE packages and themes. Navigation:
arrow keys - navigate between buttons
Space - select
Enter - confirm
ESC - exit
mouse control is also supported
)";
        StepResult res = ctx.ui->show_info(title, text);

        return res;
    }

    StepResult select_mode(InstallContext &ctx)
    {
        if (!ctx.ui) {
            Logger::instance().log(LogLevel::ERROR, "UI Context is missing in step execution.");
            return StepResult::FATAL_ERROR;
        }

        std::vector<std::string> modes = {"Normal", "Advanced"};
        int selected = 0;


        auto res = ctx.ui->show_menu("Select install mode", modes, selected);
        if (res == StepResult::NEXT){
            ctx.mode = modes[selected];
            Logger::instance().log(LogLevel::DEBUG, "Selected mode: "+ctx.mode);
            return StepResult::RESET;
        }
        return res;
    }

} // namespace core::steps