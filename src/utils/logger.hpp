#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <mutex>

enum class LogLevel : int {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
};

inline constexpr std::string_view log_level_to_string(LogLevel level) noexcept {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARN";
        case LogLevel::ERROR:   return "ERROR";
    }
    return "UNKNOWN";
}

class Logger {
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& instance();

    void init(const std::string& logFilePath);
    void set_min_level(LogLevel level) { m_min_level = level; }
    void log(LogLevel level, std::string_view message);

private:
    Logger() = default;
    ~Logger();

    std::ofstream m_fileStream;
    std::mutex m_mutex;
    bool m_initialized = false;
    LogLevel m_min_level = LogLevel::INFO; // По умолчанию без флага -d дебаг не пишется
};
