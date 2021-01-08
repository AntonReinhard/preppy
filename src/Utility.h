/**
 * @file Utility.h
 * @author Anton Reinhard
 * @brief Utility functions header
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include "Logger.h"
#include "definitions.h"
#include "solvers/Solver.h"

#include <string>
#include <sstream>
#include <chrono>
#include <memory>
#include <vector>
#include <map>

namespace preppy::util {

   /**
    * @brief A class for all sorts of static utility functions
    * 
    */
   class Utility {
   public:

      /**
       * @brief Calls all the different init functions
       * 
       * @return bool True if everything was initialized successfully
       */
      static bool init();

      /**
       * @brief Cleans up everything that needs cleanup
       * 
       */
      static void cleanup();

      /**
       * @brief Parses the command line
       * 
       * @param argc argc from main
       * @param argv argv from main
       * @return bool Returns false if an error was encountered and the program should exit
       */
      static bool parseCommandLine(const int argc, char** argv);

      /**
       * @brief Takes a duration and formats it as a string as "[ss.mmmm]"
       * 
       * @param time The time to format
       * @return std::string The formatted string
       */
      static std::string timeToString(clock::duration time);

      /**
       * @brief Get a string containing current real time and date
       * 
       * @return std::string The string containing current real time and date
       */
      static std::string getDateTime();

      /**
       * @brief Initializes the Logger
       * 
       * @param logLevel Log level to use for the logger
       */
      static void initializeLogger(const log::LOG_LEVEL logLevel);

      /**
       * @brief Deletes the Logger
       * 
       */
      static void deleteLogger();

      /**
       * @brief For logging program output, will always be logged unless logging level = NOTHING
       * 
       * @tparam Ts Any types that can be piped into an ostream
       * @param args Arguments that will be piped into the output in order
       */
      template<class... Ts>
      static void logOutput(Ts const&... args) {
         std::ostringstream os;

         using expander = int[];
         (void) expander{0,
            (void(os<<args), 0)...
         };

         if (nullptr != logger) {
            logger->logOutput(os.str());
         }
      }

      /**
       * @brief Log something to the global logger at debug level
       * 
       * @tparam Ts Any types that can be piped into an ostream
       * @param args Arguments that will be piped into the output in order
       */
      template<class... Ts>
      static void logDebug(Ts const&... args) {
         std::ostringstream os;
            
         using expander = int[];
         (void) expander{0, 
            (void(os<<args), 0)...
         };
            
         if (nullptr != logger) {
            logger->logDebug(os.str());
         }
      }

      /**
       * @brief Log something to the global logger at info level
       * 
       * @tparam Ts Any types that can be piped into an ostream
       * @param args Arguments that will be piped into the output in order
       */
      template<class... Ts>
      static void logInfo(Ts const&... args) {
         std::ostringstream os;
            
         using expander = int[];
         (void) expander{0, 
            (void(os<<args), 0)...
         };
            
         if (nullptr != logger) {
            logger->logInfo(os.str());
         }
      }

      /**
       * @brief Log something to the global logger at warning level
       * 
       * @tparam Ts Any types that can be piped into an ostream
       * @param args Arguments that will be piped into the output in order
       */
      template<class... Ts>
      static void logWarning(Ts const&... args) {
         std::ostringstream os;
            
         using expander = int[];
         (void) expander{0, 
            (void(os<<args), 0)...
         };
            
         if (nullptr != logger) {
            logger->logWarning(os.str());
         }
      }

      /**
       * @brief Log something to the global logger at error level
       * 
       * @tparam Ts Any types that can be piped into an ostream
       * @param args Arguments that will be piped into the output in order
       */
      template<class... Ts>
      static void logError(Ts const&... args) {
         std::ostringstream os;
          
         using expander = int[];
         (void) expander{0, 
            (void(os<<args), 0)...
         };
            
         if (nullptr != logger) {
            logger->logError(os.str());
         }
      }

      /**
       * @brief Checks if a file or directory exists at the given path
       * 
       * @param path The path to the file to check
       * @return bool True if the file exists
       */
      static bool fileExists(const std::string& path);

      /**
       * @brief Checks whether the given path refers to a directory
       * 
       * @param path The path to check
       * @return bool True if the path refers to a directory
       */
      static bool isDirectory(const std::string& path);

      /**
       * @brief Checks whether the given path refers to a file
       * 
       * @param path The path to check
       * @return bool True if the path refers to a file
       */
      static bool isFile(const std::string& path);

      /**
       * @brief Initializes Signal Handling
       * 
       */
      static void initializeSignalHandling();

      /**
       * @brief The Signal Handler used to catch signals
       * 
       * @param signal The signal that was raised
       */
      static void SignalHandler(int signal);

      /**
       * @brief Executes the given command. Returns stdout output on successful execution
       * 
       * @param command The command to execute
       * @return std::string The stdout output of the executed command
       */
      static std::string systemCall(const std::string& command);

      /**
       * @brief Splits a string into tokens using a given delimiter
       * 
       * @param str The string to split
       * @param delim The delimiter to use
       * @return std::vector<std::string> A vector containing the substrings
       */
      static std::vector<std::string> tokenizeString(const std::string& str, const std::string& delim);

      /**
       * @brief Starts a timer with the given name
       * 
       * @param name The name of the timer
       */
      static void startTimer(const std::string& name);

      /**
       * @brief Stops the timer with the given name and returns how long it was running
       * 
       * @param name The name of the timer
       * @return util::clock::duration The duration the timer was running
       */
      static util::clock::duration stopTimer(const std::string& name);

      /**
       * @brief Converts a chrono::duration to a readable string, using appropriate units between microseconds and hours
       * 
       * @param duration The duration to convert
       * @return std::string A readable string
       */
      static std::string durationToString(const util::clock::duration& duration);

      /**
       * @brief Gets currently used solver
       * 
       * @return std::shared_ptr<solvers::Solver> The solver currently in use
       */
      static std::shared_ptr<solvers::Solver> getSolver();

      /**
       * @brief Converts a vector of literals to a vector of variables, effectively using abs() on every literal
       * 
       * @param literals The vector of literals to convert
       * @return cnf::Variables The vector of Variables
       */
      static cnf::Variables literalsToVariables(const cnf::Literals& literals);

   protected:

      /**
       * @brief Sets the given solver as the main solver
       * 
       * @param solverName The solver to use
       */
      static void setSolver(const std::string& solverName);

   private:

      /**
       * @brief Unique Pointer to the logger
       * 
       */
      static std::unique_ptr<log::Logger> logger;

      /**
       * @brief Shared Pointer to the solver
       * 
       */
      static std::shared_ptr<solvers::Solver> solver;

      /**
       * @brief Timeout for processing
       * 
       */
      static clock::duration solvingTimeout;

      /**
       * @brief The log level used by everything
       * 
       */
      static log::LOG_LEVEL GLOBAL_LOG_LEVEL;

      /**
       * @brief The timer start points for startTimer and stopTimer
       * 
       */
      static std::map<std::string, util::clock::time_point> timerStartPoints;

   };

}
