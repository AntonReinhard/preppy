#pragma once
#include "Logger.h"
#include "definitions.h"

#include <string>
#include <sstream>
#include <chrono>
#include <memory>
#include <vector>
#include <map>

namespace preppy::util {

   class Utility {
   public:

      // Calls all the different init functions
      static bool init();

      // Cleans up anything that needs cleanup
      static void cleanup();

      // Takes a duration and formats it as a string as "[dddd, hh:mm:ss.mmmm]"
      static std::string timeToString(clock::duration time);

      // Returns a string containing current real time and date
      static std::string getDateTime();

      // Initializes the Logger
      static void initializeLogger(const log::LOG_LEVEL logLevel);

      // Deletes the Logger
      static void deleteLogger();

      // Log something to the global logger
      template<class... Ts>
      static void logDebug(Ts const&... args) {
         std::ostringstream os;
            
         using expander = int[];
         (void) expander{0, 
            (void(os<<args), 0)...
         };
            
         if (logger != nullptr) {
            logger->logDebug(os.str());
         }
      }

      // Log something to the global logger
      template<class... Ts>
      static void logInfo(Ts const&... args) {
         std::ostringstream os;
            
         using expander = int[];
         (void) expander{0, 
            (void(os<<args), 0)...
         };
            
         if (logger != nullptr) {
            logger->logInfo(os.str());
         }
      }

      // Log something to the global logger
      template<class... Ts>
      static void logWarning(Ts const&... args) {
         std::ostringstream os;
            
         using expander = int[];
         (void) expander{0, 
            (void(os<<args), 0)...
         };
            
         if (logger != nullptr) {
            logger->logWarning(os.str());
         }
      }

      // Log something to the global logger
      template<class... Ts>
      static void logError(Ts const&... args) {
         std::ostringstream os;
          
         using expander = int[];
         (void) expander{0, 
            (void(os<<args), 0)...
         };
            
         if (logger != nullptr) {
            logger->logError(os.str());
         }
      }

      // Checks if a file exists
      static bool fileExists(const std::string& path);

      static void initializeSignalHandling();

      static void SignalHandler(int signal);

      // Executes the given command. Returns true on successfull execution
      static bool systemCall(const std::string& command);

      static std::vector<std::string> tokenizeString(const std::string& str, const std::string& delim);

      // Starts a timer with the given name
      static void startTimer(const std::string& name);

      // Stops the timer with the given name and returns how long it was running
      static util::clock::duration stopTimer(const std::string& name);

      // Converts a chrono::duration to a readable string
      static std::string durationToString(const util::clock::duration& duration);

   protected:

   private:

      static std::unique_ptr<log::Logger> logger;

      static log::LOG_LEVEL GLOBAL_LOG_LEVEL;

      static std::map<std::string, util::clock::time_point> timerStartPoints;

   };

}
