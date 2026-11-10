#include "steps_normal.hpp"
#include "utils/logger.hpp"
#include <vector>

    namespace core::steps {
    std::vector<Step> get_normal_installation_steps() {
        using namespace core::steps;
        return {
            // {"Тестовый шаг общего режима", template_step},
            {"Выбор диска", disk_select_step}
        };
    }

    StepResult disk_select_step(InstallContext& ctx) {
        if (!ctx.ui) {
            return StepResult::FATAL_ERROR;
        }

        auto disks = utils::get_disks();

        std::vector<std::string> values;
        std::vector<std::string> keys;

        for(const auto& [path, name] : disks) {
            values.push_back(name+" ["+path+"]");
            keys.push_back(path);
        }

        int choice;
        StepResult res = ctx.ui->show_menu("disk selection", values, choice);

        if (res == StepResult::NEXT) {
            ctx.target_disk = disks[keys[choice]];
            Logger::instance().log(LogLevel::INFO, "Target disk selected by user: " + ctx.target_disk);
        }

        return res;
    }

    StepResult test_normal_step(InstallContext& ctx) {
        return StepResult::NEXT;
    }
}