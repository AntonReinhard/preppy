#pragma once
#include "definitions.h"

#include <chrono>
#include <iostream>
#include <string>
#include <fstream>
#include <mutex>
#include <memory>

namespace preppy::log {

    class Logger {
    public:

        Logger(LOG_LEVEL logLevel);

        ~Logger();

        void logLogo();

        std::string getCurrentTimeString();

        util::clock::duration getCurrentTime();

        void logDebug(const std::string& message);

        void logInfo(const std::string& message);

        void logWarning(const std::string& message);

        void logError(const std::string& message);

    protected:

        // Only for stuff from the logger itself
        void logLoggerInfo(const std::string& message);

        void log(const std::string& message, LOG_LEVEL logLevel);

        // Returns the enum value name
        static std::string logLevelToString(LOG_LEVEL logLevel);

        // Returns the line start, e.g. "DEBUG:   "
        static std::string logLineStart(LOG_LEVEL logLevel);

    private:

        util::clock::time_point start;

        LOG_LEVEL logLevel;

        // mutex that is locked while writing to the log file
        std::mutex mutex;

    };

}
