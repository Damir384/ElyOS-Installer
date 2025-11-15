#pragma once
#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

class Logger {
public:
    static Logger& get();                 // Singleton access
    void log(LogLevel level, const std::string& msg);

private:
    Logger();                             // constructor hidden
    ~Logger();
    std::ofstream file;
    std::mutex mtx;

    std::string levelToString(LogLevel level);
    std::string currentTime();
};
