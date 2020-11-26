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
        
        this->logDebug("Logger created at " + util::Utility::getDateTime() + " with logging level \"" + logLevelToString(logLevel) + "\"");

        this->logLogo();
    }

    Logger::~Logger() {
        this->logOutput("Execution finished");
    }

    void Logger::logLogo() {
        std::vector<std::string> logoLines = {
            std::string("preppy v") + util::VERSION,
            "Copyright (C) Anton Reinhard",
            "License: The MIT License <https://opensource.org/licenses/MIT>"
        };

        for (const auto& line : logoLines) {
            this->logOutput(line);
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

    void Logger::logOutput(const std::string& message) {
        if (logLevel == NOTHING) {
            return;
        }
        std::ostringstream outputss;
        std::istringstream inputss(message);
        std::string line;

        // take lines apart so every line has the time and log level at the start in the log
        while (std::getline(inputss, line)) {
            if (line.empty()) { // discard emtpty lines, e.g. in case there's a newline at the end of message
                continue;
            }
            outputss << this->getCurrentTimeString() << " ";
            outputss << line << std::endl;
        }

        // write everything to cout
        std::cout << outputss.str() << std::flush;
    }

    void Logger::log(const std::string& message, const LOG_LEVEL logLevel) {
        const std::lock_guard<std::mutex> lock(this->mutex);
        
        if (logLevel <= this->logLevel) {
            std::ostringstream outputss;
            std::istringstream inputss(message);
            std::string line;

            // take lines apart so every line has the time and log level at the start in the log
            while (std::getline(inputss, line)) {
                if (line.empty()) { // discard emtpty lines, e.g. in case there's a newline at the end of message
                    continue;
                }
                outputss << this->getCurrentTimeString() << " " << logLineStart(logLevel);
                outputss << line << std::endl;
            }

            // write everything to cout
            std::cout << outputss.str() << std::flush;
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
        case NOTHING:
            return "Nothing";
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
        case NOTHING:   // should never happen, as nothing will be logged with this loglevel
        default:
            return "UNKNOWN: ";
        }
    }

}
