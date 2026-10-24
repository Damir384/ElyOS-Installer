#include "steps_common.hpp"
#include "utils/logger.hpp"

namespace core::steps {

    std::vector<Step> get_initial_steps() {
        return {
            {"Экран приветствия", welcome_step},
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

    StepResult disk_select_step(InstallContext& ctx) {
        if (!ctx.ui) {
            return StepResult::FATAL_ERROR;
        }

        auto disks = utils::get_disks();

        std::string selected_disk_path;
        StepResult res = ctx.ui->show_disks_menu(disks, selected_disk_path);

        if (res == StepResult::NEXT) {
            ctx.target_disk = selected_disk_path;
            Logger::instance().log(LogLevel::INFO, "Target disk selected by user: " + ctx.target_disk);
        }

        return res;
    }

} // namespace core::steps