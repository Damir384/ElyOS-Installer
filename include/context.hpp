#pragma once
#include <string>
#include <vector>

class InstallerContext {
public:
    // Singleton access
    static InstallerContext& get() {
        static InstallerContext instance;
        return instance;
    }

    // Config data
    std::string selectedDisk;
    std::string partitionScheme;
    std::string graphics;
    std::string audio;
    std::vector<std::string> extraPackages;

private:
    InstallerContext() = default;
};
