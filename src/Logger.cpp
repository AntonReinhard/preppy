#include "Logger.h"
#include "definitions.h"
#include "Utility.h"

#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdarg>

namespace preppy::log {

    Logger::Logger(const LOG_LEVEL logLevel)
        : logLevel(logLevel) {
        this->start = util::clock::now();
        
        this->logLoggerInfo("Logger created at " + util::Utility::getDateTime() + " with logging level \"" + logLevelToString(logLevel) + "\"");

        this->logLogo();
    }

    Logger::~Logger() {
    }

    void Logger::logLogo() {
        std::vector<std::string> logoLines = {
            "",
            "PREPPY",
            ""
        };

        for (const auto& line : logoLines) {
            this->logLoggerInfo(line);
        }
    }

    std::string Logger::getCurrentTimeString() {
        return util::Utility::timeToString(this->getCurrentTime());
    }

    util::clock::duration Logger::getCurrentTime() {
        return util::clock::now() - this->start;
    }

    void Logger::logDebug(const std::string& message) {
        this->log(message, LOG_LEVEL::DEBUG);
    }

    void Logger::logInfo(const std::string& message) {
        this->log(message, LOG_LEVEL::INFO);
    }

    void Logger::logWarning(const std::string& message) {
        this->log(message, LOG_LEVEL::WARNING);
    }

    void Logger::logError(const std::string& message) {
        this->log(message, LOG_LEVEL::ERROR);
    }

    void Logger::logLoggerInfo(const std::string& message) {
        std::ostringstream ss;
        ss << this->getCurrentTimeString() << " ";
        ss << message;
        
        std::cerr << ss.str() << std::endl;
    }

    void Logger::log(const std::string& message, const LOG_LEVEL logLevel) {
        const std::lock_guard<std::mutex> lock(mutex);
        
        if (logLevel >= this->logLevel) {
            std::ostringstream ss;
            ss << this->getCurrentTimeString() << " " << logLineStart(logLevel);
            ss << message;
            
            std::cerr << ss.str() << std::endl;
        }
    }

    std::string Logger::logLevelToString(const LOG_LEVEL logLevel) {
        switch (logLevel) {
        case DEBUG:
            return "Debug";
        case INFO:
            return "Info";
        case WARNING:
            return "Warning";
        case ERROR:
            return "Error";
        default:
            return "Unknown";
        }
    } 
    
    std::string Logger::logLineStart(const LOG_LEVEL logLevel) {
        switch (logLevel) {
        case DEBUG:
            return "DEBUG:   ";
        case INFO:
            return "INFO:    ";
        case WARNING:
            return "WARNING: ";
        case ERROR:
            return "ERROR:   ";
        default:
            return "UNKNOWN: ";
        }
    }

}
