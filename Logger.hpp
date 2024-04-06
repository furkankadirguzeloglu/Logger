#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <map>
#include <iomanip>
#include <sstream>
#pragma warning(disable : 4996)
#pragma warning(disable : 6031)

enum class LogLevel { LOG_NORMAL, LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_CRITICAL, LOG_TRACE, LOG_VERBOSE, LOG_AUDIT, LOG_FATAL };

struct LogColor {
    std::string colorCode;
    std::string prefix;
};

class Logger {
public:
    Logger() : logLevel(LogLevel::LOG_NORMAL), writeToConsole(true), writeToFile(false), fileName("log.txt") {
        colorMap[LogLevel::LOG_NORMAL] = { "#ffffff", "" };
        colorMap[LogLevel::LOG_DEBUG] = { "#0356fc", "[DEBUG]: " };
        colorMap[LogLevel::LOG_INFO] = { "#00ff00", "[INFO]: " };
        colorMap[LogLevel::LOG_WARNING] = { "#ffff00", "[WARNING]: " };
        colorMap[LogLevel::LOG_ERROR] = { "#ff0000", "[ERROR]: " };
        colorMap[LogLevel::LOG_CRITICAL] = { "#ff00ff", "[CRITICAL]: " };
        colorMap[LogLevel::LOG_TRACE] = { "#808080", "[TRACE]: " };
        colorMap[LogLevel::LOG_VERBOSE] = { "#800080", "[VERBOSE]: " };
        colorMap[LogLevel::LOG_AUDIT] = { "#008080", "[AUDIT]: " };
        colorMap[LogLevel::LOG_FATAL] = { "#ff6347", "[FATAL]: " };

    }

    void setLogLevel(LogLevel level) {
        logLevel = level;
    }

    void setWriteToConsole(bool value) {
        writeToConsole = value;
    }

    void setWriteToFile(bool value) {
        writeToFile = value;
    }

    void setFileName(const std::string& name) {
        fileName = name;
    }

    void setLogColor(LogLevel level, const std::string& color) {
        colorMap[level].colorCode = color;
    }

    void setLogColorRGB(LogLevel level, int r, int g, int b) {
        std::stringstream ss;
        ss << '#' << std::setw(2) << std::setfill('0') << std::hex << r
            << std::setw(2) << std::setfill('0') << std::hex << g
            << std::setw(2) << std::setfill('0') << std::hex << b;
        colorMap[level].colorCode = ss.str();
    }

    void writeLog(const std::string& message, LogLevel level = LogLevel::LOG_NORMAL) {
        std::lock_guard<std::mutex> lock(mutex);
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::tm* ptm = std::localtime(&time);
        char buffer[32];
        if (ptm != nullptr) {
            if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ptm)) {
                std::string timestamp(buffer);
                if (shouldLog(level)) {
                    if (writeToConsole)
                        outputToConsole(message, level, timestamp);
                    if (writeToFile)
                        outputToFile(message, level, timestamp);
                }
            }
            else {
                throw std::runtime_error("Failed to format timestamp.");
            }
        }
    }

private:
    LogLevel logLevel;
    bool writeToConsole;
    bool writeToFile;
    std::string fileName;
    std::mutex mutex;
    std::map<LogLevel, LogColor> colorMap;

    std::string hexToAnsi(const std::string& color) {
        std::stringstream ss;
        ss << "\033[38;2;";
        if (color[0] == '#') {
            int r, g, b;
            sscanf(color.c_str(), "#%02x%02x%02x", &r, &g, &b);
            ss << r << ';' << g << ';' << b << 'm';
        }
        else {
            ss << color;
        }
        return ss.str();
    }

    void outputToConsole(const std::string& message, LogLevel level = LogLevel::LOG_NORMAL, const std::string& timestamp = "") {
        if (colorMap.find(level) != colorMap.end()) {
            std::cout << hexToAnsi(colorMap[level].colorCode);
        }

        std::cout << "[" << timestamp << "] " << colorMap[level].prefix << message << "\033[0m" << std::endl;
    }

    void outputToFile(const std::string& message, LogLevel level = LogLevel::LOG_NORMAL, const std::string& timestamp = "") {
        std::ofstream file(fileName, std::ios_base::app);
        if (file.is_open()) {
            file << "[" << timestamp << "] " << colorMap[level].prefix << message << std::endl;
            file.close();
        }
        else {
            throw std::runtime_error("Unable to open log file!");
        }
    }

    bool shouldLog(LogLevel level) {
        return level >= logLevel || level == LogLevel::LOG_DEBUG;
    }
};