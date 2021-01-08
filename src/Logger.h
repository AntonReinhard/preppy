/**
 * @file Logger.h
 * @author Anton Reinhard
 * @brief Logger header
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include "definitions.h"

#include <chrono>
#include <iostream>
#include <string>
#include <fstream>
#include <mutex>
#include <memory>

namespace preppy::log {

    /**
     * @brief The logger of the program, used to log output, errors, warnings, info and debug information
     * 
     */
    class Logger {
    public:

        /**
         * @brief Construct a new Logger
         * 
         * @param logLevel The logging level for the logger to use. Everything with a logging level lower than this will not be logged
         */
        Logger(LOG_LEVEL logLevel);

        /**
         * @brief Destroy the Logger object
         * 
         */
        ~Logger();

        /**
         * @brief Output program information
         * 
         */
        void logLogo();

        /**
         * @brief Get the current time as string
         * 
         * @return std::string Current program time as a string
         */
        std::string getCurrentTimeString();

        /**
         * @brief Get the current time
         * 
         * @return util::clock::duration The current time
         */
        util::clock::duration getCurrentTime();

        /**
         * @brief Log a given message with log level debug
         * 
         * @param message The message to log
         */
        void logDebug(const std::string& message);

        /**
         * @brief Log a given message with log level info
         * 
         * @param message The message to log
         */
        void logInfo(const std::string& message);

        /**
         * @brief Log a given message with log level warning
         * 
         * @param message The message to log
         */
        void logWarning(const std::string& message);

        /**
         * @brief Log a given message with log level error
         * 
         * @param message The message to log
         */
        void logError(const std::string& message);

        /**
         * @brief Log program output, will only be discarded if log level was set to NOTHING
         * 
         * @param message The message to log
         */
        void logOutput(const std::string& message);

    protected:

        /**
         * @brief Log a message with given log level
         * 
         * @param message The message to log
         * @param logLevel The log level to use
         */
        void log(const std::string& message, LOG_LEVEL logLevel);

        /**
         * @brief Returns the log level enum value name as a string
         * 
         * @param logLevel The log level to convert to string
         * @return std::string The name of the log level
         */
        static std::string logLevelToString(LOG_LEVEL logLevel);

        /**
         * @brief Returns the line start, e.g. "DEBUG:   "
         * 
         * @param logLevel The log level to convert to a line start
         * @return std::string The line start
         */
        static std::string logLineStart(LOG_LEVEL logLevel);

    private:

        /**
         * @brief The time point this logger was created
         * 
         */
        util::clock::time_point start;

        /**
         * @brief The logging level of this logger
         * 
         */
        LOG_LEVEL logLevel;

        /**
         * @brief Mutex that is locked while writing logs for thread safety
         * 
         */
        std::mutex mutex;

    };

}
