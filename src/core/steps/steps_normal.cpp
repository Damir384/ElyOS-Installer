#include "steps_normal.hpp"
#include "utils/logger.hpp"
#include "utils/fs_utils.hpp"
#include <nlohmann/json.hpp>
#include <vector>

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
        nlohmann::json list;

        if(utils::check_file_readable(ctx.partition_profiles_path, err_msg)==0) {
            try {
                list = nlohmann::json::parse(utils::read_file(ctx.partition_profiles_path));
                list = list.at("disk_layouts");
                err = 0;
            }
            catch(std::exception& e) {
                err_msg = e.what();
            }
        }
        
        if (err) {
            Logger::instance().log(LogLevel::WARNING, "Cannot open the partition profiles. Using the emergency list. Error message: "+err_msg);
            ctx.ui->show_warning("Partitions not found", "Cannot open the partition profiles.\nUsing the emergency list.\n"+err_msg);

                list = nlohmann::json::array({
    nlohmann::json::parse(R"({
        "description": "512 MB EFI boot partition and root partition.",
        "partition_table": "gpt",
        "partitions": [
            {
                "flags": ["boot", "esp"],
                "fs": "fat32",
                "mount_point": "/boot",
                "name": "EFI System",
                "size": "512M"
            },
            {
                "flags": [],
                "fs": "ext4",
                "mount_point": "/",
                "name": "Root",
                "size": "auto"
            }
        ],
        "profile": "linux_uefi"
    })")});
        }
        
        std::vector<std::string> disk_layouts;
        for(const auto& layout : list) {
            disk_layouts.push_back(layout.at("profile").get<std::string>() + " " + layout.at("description").get<std::string>());
        }

        int selected = 0;

        auto res = ctx.ui->show_menu("Select partitioning scheme", disk_layouts, selected);

        if (res==StepResult::NEXT) {
            Logger::instance().log(LogLevel::INFO, "User select: " + list.at(selected).at("profile").get<std::string>());
            Logger::instance().log(LogLevel::DEBUG, "Profile layout parameters: "+list.at(selected).dump());
        }

        return res;
    }

    StepResult test_normal_step(InstallContext& ctx) {
        return StepResult::NEXT;
    }
}