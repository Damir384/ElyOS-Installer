#include "steps_normal.hpp"
#include "utils/logger.hpp"
#include "utils/fs_utils.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <stdexcept>

namespace {
    static void validate_disk_layouts(const nlohmann::json& layouts)
    {
        using json = nlohmann::json;

        if (!layouts.is_array())
            throw std::runtime_error("disk_layouts: expected array");

        for (std::size_t i = 0; i < layouts.size(); ++i) {
            const auto& layout = layouts[i];
            const std::string path = "disk_layouts[" + std::to_string(i) + "]";

            if (!layout.is_object())
                throw std::runtime_error(path + ": expected object");

            for (const char* key : {"description", "partition_table", "profile"}) {
                if (!layout.contains(key) || !layout.at(key).is_string())
                    throw std::runtime_error(path + "." + key + ": missing or not a string");
            }

            if (!layout.contains("partitions") || !layout.at("partitions").is_array())
                throw std::runtime_error(path + ".partitions: missing or not an array");

            const auto& partitions = layout.at("partitions");
            for (std::size_t j = 0; j < partitions.size(); ++j) {
                const auto& partition = partitions[j];
                const std::string partition_path =
                    path + ".partitions[" + std::to_string(j) + "]";

                if (!partition.is_object())
                    throw std::runtime_error(partition_path + ": expected object");

                for (const char* key : {"fs", "mount_point", "name", "size"}) {
                    if (!partition.contains(key) || !partition.at(key).is_string())
                        throw std::runtime_error(
                            partition_path + "." + key + ": missing or not a string");
                }

                if (!partition.contains("flags") || !partition.at("flags").is_array())
                    throw std::runtime_error(partition_path + ".flags: missing or not an array");

                for (std::size_t k = 0; k < partition.at("flags").size(); ++k) {
                    if (!partition.at("flags")[k].is_string())
                        throw std::runtime_error(
                            partition_path + ".flags[" + std::to_string(k) +
                            "]: expected string");
                }
            }
        }
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
        nlohmann::json list;

        if(utils::check_file_readable(ctx.partition_profiles_path, err_msg)==0) {
            try {
                list = nlohmann::json::parse(utils::read_file(ctx.partition_profiles_path));
                list = list.at("disk_layouts");
                validate_disk_layouts(list);
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

        // TODO: записать всё в partition plan но без реальных разделов.

        return res;
    }

    StepResult test_normal_step(InstallContext& ctx) {
        return StepResult::NEXT;
    }

}