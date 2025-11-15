#include "../../include/logger.hpp"
#include <ctime>
#include <iomanip>

Logger& Logger::get() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    file.open("/tmp/elyos_install.log", std::ios::app); // можно изменить путь
}

Logger::~Logger() {
    if (file.is_open())
        file.close();
}

std::string Logger::levelToString(LogLevel level) {
    switch(level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::DEBUG: return "DEBUG";
    }
    return "UNKNOWN";
}

std::string Logger::currentTime() {
    std::time_t now = std::time(nullptr);
    std::tm* tm = std::localtime(&now);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
    return buf;
}

void Logger::log(LogLevel level, const std::string& msg) {
    std::lock_guard<std::mutex> lock(mtx);
    if (file.is_open()) {
        file << "[" << currentTime() << "][" << levelToString(level) << "] "
             << msg << std::endl;
    }
}
