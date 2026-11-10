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

/**
 * @brief Thread-safe singleton file logger.
 * @details Provides file-based logging formatted with timestamps (HH:MM:SS) 
 * and log levels. Implements threshold-based message filtering.
 * 
 * @code
 * Logger::instance().init("installer.log");
 * Logger::instance().set_min_level(LogLevel::DEBUG);
 * Logger::instance().log(LogLevel::INFO, "Subsystem initialized.");
 * @endcode
 */
class Logger {
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /**
     * @brief Retrieves the global singleton instance.
     * @details Uses Meyers' Singleton pattern guaranteed thread-safe since C++11.
     * @return Logger& Reference to the logger instance.
     */
    static Logger& instance();

    /**
     * @brief Initializes the log file stream in append mode.
     * @param [in] logFilePath Path to the target log file.
     */
    void init(const std::string& logFilePath);

    /**
     * @brief Sets the minimum severity threshold for log output.
     * @details Messages below this severity level are discarded before formatting.
     * @param [in] level The minimum LogLevel to record (DEBUG, INFO, WARNING, ERROR).
     */
    void set_min_level(LogLevel level);

    /**
     * @brief Writes a formatted message to the active log file.
     * @details Protected by a mutex. Flushes stream output on every call.
     * @param [in] level Severity level of the event.
     * @param [in] message String payload to write.
     */
    void log(LogLevel level, std::string_view message);

private:
    Logger() = default;
    ~Logger();

    std::ofstream m_fileStream;
    std::mutex m_mutex;
    bool m_initialized = false;
    LogLevel m_min_level;
};
