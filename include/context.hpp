#pragma once
#include <string>
#include <vector>

class InstallerContext {
public:
    std::string selectedDisk;
    std::string partitionScheme;
    std::string graphics;
    std::string audio;
    std::vector<std::string> extraPackages;
    
    // Можешь добавить методы сериализации в файл позже
};
