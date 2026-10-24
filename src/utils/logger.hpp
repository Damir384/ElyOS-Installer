#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <mutex>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
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
    void log(LogLevel level, std::string_view message);

private:
    Logger() = default;
    ~Logger();

    std::ofstream m_fileStream;
    std::mutex m_mutex;
    bool m_initialized = false;
};
