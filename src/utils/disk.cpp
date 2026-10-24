#include "disk.hpp"
#include "console.hpp"
#include <nlohmann/json.hpp>
#include "logger.hpp"
#include <array>
#include <string_view>
#include <algorithm>

constexpr std::array<std::string_view, 3> BLACKLIST_PREFIXES = {
    "zram", 
    "loop", 
    "ram"
};

bool is_blacklisted(std::string_view device_name) {
    return std::any_of(BLACKLIST_PREFIXES.begin(), BLACKLIST_PREFIXES.end(),
        [&device_name](std::string_view prefix) {
            return device_name.rfind(prefix, 0) == 0;
        }
    );
}

namespace utils {

std::map<std::string, std::string> get_disks() {
    std::map<std::string, std::string> disks_map;
    
    auto res = utils::ProcessRunner::execute({}, "lsblk", {"-J", "-b", "-o", "NAME,PATH,SIZE,TYPE,FSTYPE,MODEL"});
    Logger::instance().log(LogLevel::DEBUG, res.output);
    
    try {
        auto disks = nlohmann::json::parse(res.output);
        
        for (const auto& disk : disks["blockdevices"]) {
            std::string type = disk.value("type", "");
            std::string name = disk.value("name", "");
            std::string path = disk.value("path", "");
            uint64_t size = disk.value("size", (uint64_t)0);

            if (type != "disk" || size == 0 || is_blacklisted(name)) {
                continue;
            }

            std::string model = "Generic Storage Device";
            if (disk.contains("model") && !disk["model"].is_null() && disk["model"].is_string()) {
                std::string raw_model = disk["model"].get<std::string>();
                if (!raw_model.empty()) {
                    model = raw_model;
                }
            }

            if (!path.empty()) {
                uint64_t size_gb = size / (1024 * 1024 * 1024);
                disks_map[path] = model + " (" + std::to_string(size_gb) + " GB)";
            }
        }
    } catch (const nlohmann::json::parse_error& e) {
        Logger::instance().log(LogLevel::ERROR, "Failed to parse lsblk JSON: " + std::string(e.what()));
    }

    return disks_map;
}

} // namespace utils