#include "logger.hpp"
#include "times.hpp"

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    if (m_fileStream.is_open()) {
        m_fileStream.close();
    }
}

void Logger::init(const std::string& logFilePath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_fileStream.is_open()) {
        m_fileStream.close();
    }
    m_fileStream.open(logFilePath, std::ios::out | std::ios::app);
    m_initialized = m_fileStream.is_open();
}

void Logger::log(LogLevel level, std::string_view message) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_initialized || !m_fileStream.is_open()) {
        return;
    }

    std::string timeStr = utils::time_to_string(utils::get_time());
    std::string_view lvlStr = log_level_to_string(level);

    m_fileStream << "[" << timeStr << "] [" << lvlStr << "] " << message << "\n";
    m_fileStream.flush();
}
