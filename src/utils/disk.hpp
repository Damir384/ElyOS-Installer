#pragma once
#include <string>
#include <map>

namespace utils {

    /**
     * @brief Inspects host block storage devices via lsblk JSON streaming.
     * @details Filters out loop, zram, and 0-byte card reader nodes, parsing device
     * node paths, human-readable model names, and capacities in gigabytes.
     * 
     * @return std::map<std::string, std::string> Key-value mapping:
     *         key is the device path (e.g., "/dev/sda"),
     *         value is the formatted model name and size description.
     */
    std::map<std::string, std::string> get_disks();

} // namespace utils