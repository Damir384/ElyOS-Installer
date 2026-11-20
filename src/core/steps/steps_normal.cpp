#include "steps_normal.hpp"
#include "utils/logger.hpp"
#include "utils/fs_utils.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <iostream>

void traverse(const nlohmann::json& j) {
    if (j.is_object()) {
        for (const auto& [key, val] : j.items()) {
            std::cout << "Ключ: " << key << '\n';
            traverse(val);
        }
    } else if (j.is_array()) {
        for (const auto& item : j) {
            traverse(item);
        }
    } else {
        // Базовый случай: достигнуто примитивное значение (строка, число, bool)
        std::cout << "Значение: " << j << '\n';
    }
}


    namespace core::steps {
    std::vector<Step> get_normal_installation_steps() {
        using namespace core::steps;
        return {
            // {"Тестовый шаг общего режима", template_step},
            {"Выбор диска", disk_select_step},
            {"Выбор схемы разметки", select_partitioning_scheme},
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
            ctx.target_disk = keys[choice];
            Logger::instance().log(LogLevel::INFO, "Target disk selected by user: " + ctx.target_disk);
        }

        return res;
    }

    StepResult select_partitioning_scheme(InstallContext& ctx) {
        if (!ctx.ui) {
            return StepResult::FATAL_ERROR;
        }

        std::string err_msg;
        int err = -1;

        if(utils::check_file_readable(ctx.partition_profiles_path, err_msg)==0) {
            auto list = nlohmann::json::parse(utils::read_file(ctx.partition_profiles_path));
            traverse(list);
            err = 0;
        }
        
        if (err) {
            Logger::instance().log(LogLevel::WARNING, "Cannot open the partition profiles. Using the emergency list."+err_msg);
            ctx.ui->show_warning("Partitions not found", "Cannot open the partition profiles.\nUsing the emergency list.\n"+err_msg);
        }

        return StepResult::NEXT;
    }

    StepResult test_normal_step(InstallContext& ctx) {
        return StepResult::NEXT;
    }
}