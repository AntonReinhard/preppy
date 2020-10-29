#include "Utility.h"
#include "definitions.h"

#include <sys/stat.h>
#include <unistd.h>
#include <iomanip>
#include <string>
#include <chrono>
#include <sstream>
#include <thread>
#include <random>
#include <filesystem>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

namespace preppy::util {

   std::unique_ptr<log::Logger> Utility::logger = nullptr;

   log::LOG_LEVEL Utility::GLOBAL_LOG_LEVEL = log::WARNING;

   std::map<std::string, util::clock::time_point> Utility::timerStartPoints;

   bool Utility::init() {
      // always initialize the logger first
      Utility::initializeLogger(Utility::GLOBAL_LOG_LEVEL);
      Utility::initializeSignalHandling();

      return true;
   }

   void Utility::cleanup() {
      logDebug("Cleaning up...");

      // logger should always be the last thing to be deleted
      Utility::logger.reset();
   }

   bool Utility::parseCommandLine(const int argc, char** argv) {
      if (argc <= 1) {
         std::cout << "Usage: " << argv[0] << " [input cnf file] [options]" << std::endl;
         return false;
      }

      if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
         std::cout << "Usage: " << argv[0] << " [input cnf file] [options]" << std::endl;
         std::cout << "Options:\n"
            << "    -h        Print this help\n"
            << "    -v <n>    Sets log level to <n>, 0=NOTHING, 1=ERROR, 2=WARNING (default), 3=INFO, 4=DEBUG"
            << std::endl;
         return false;
      }

      // check that first parameter is an actual file
      if (!util::Utility::fileExists(argv[1])) {
         std::cout << "File \"" << argv[1] << "\" doesn't exist" << std::endl;
         return false;
      }

      for (auto i = 2; i < argc; ++i) {
         if (strcmp(argv[i], "-v") == 0) {
            if (i == argc - 1) { // there has to be a next option
               std::cout << "-v option expects a log level" << std::endl;
               return false;
            }
            ++i;
            std::stringstream ss(argv[i]);
            int tempLogLevel;
            if (tempLogLevel < 0 || tempLogLevel > log::LOG_LEVEL::DEBUG){
               std::cout << "Invalid log level: " << tempLogLevel << std::endl;
               return false;
            }
            ss >> tempLogLevel;
            util::Utility::GLOBAL_LOG_LEVEL = log::LOG_LEVEL(tempLogLevel);
         }
      }

      return true;
   }

   std::string Utility::timeToString(const clock::duration time) {
      std::ostringstream ss;

      ss << "["
         << std::setw(3) << std::setfill(' ') << std::chrono::duration_cast<std::chrono::seconds>(time).count() << "."
         << std::setw(3) << std::setfill('0') << std::chrono::duration_cast<std::chrono::milliseconds>(time).count() % 1000 
         << "]";

      return ss.str();
   }

   std::string Utility::getDateTime()
   {
      auto now = std::chrono::system_clock::now();
      auto in_time_t = std::chrono::system_clock::to_time_t(now);

      std::stringstream ss;
      ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%X");
      return ss.str();
   }

   void Utility::initializeLogger(const log::LOG_LEVEL logLevel) {
      if (logger == nullptr) {
         logger = std::make_unique<log::Logger>(logLevel);
      }
      else {
         std::cerr << "A logger was already initialized!" << std::endl;
      }
   }

   void Utility::deleteLogger() {
      logger.reset();
   }

   bool Utility::fileExists(const std::string& path) {
      return std::filesystem::exists(path);
   }

   void Utility::initializeSignalHandling() {
      logDebug("Initializing signal handling");
      // Configures signal handling.
      struct sigaction sig_int_handler;
      sig_int_handler.sa_handler = SignalHandler;
      sigemptyset(&sig_int_handler.sa_mask);
      sig_int_handler.sa_flags = 0;
      sigaction(SIGINT, &sig_int_handler, NULL);
   }

   void Utility::SignalHandler(int signal) {
      Utility::logError("Caught signal ", signal, ", terminating...");
      Utility::cleanup();
      exit(0);
   }

   bool Utility::systemCall(const std::string& command) {
      system(command.c_str());
      
      return true;
   }

   std::vector<std::string> Utility::tokenizeString(const std::string& str, const std::string& delim) {
      size_t start;
      size_t end = 0;

      std::vector<std::string> out;
      while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
         end = str.find(delim, start);
         out.push_back(str.substr(start, end - start));
      }

      return out;
   }

   void Utility::startTimer(const std::string& name) {
      if (Utility::timerStartPoints.find(name) != Utility::timerStartPoints.end()) {
         util::Utility::logWarning("Timer ", name, " already started, overriding");
         Utility::timerStartPoints.erase(name);
      }
      Utility::timerStartPoints.insert(std::pair<std::string, util::clock::time_point>(name, util::clock::now()));
   }

   util::clock::duration Utility::stopTimer(const std::string& name) {
      if (Utility::timerStartPoints.find(name) == Utility::timerStartPoints.end()) {
         util::Utility::logWarning("Can't stop timer ", name, " because it doesn't exist");
         return util::clock::duration(0);
      }
      auto ret = util::clock::now() - Utility::timerStartPoints[name];
      Utility::timerStartPoints.erase(name);
      return ret;
   }

   std::string Utility::durationToString(const util::clock::duration& duration) {
      std::stringstream ss;
      ss << std::setfill('0');
      
      const auto hrs = std::chrono::duration_cast<std::chrono::hours>(duration);
      const auto mins = std::chrono::duration_cast<std::chrono::minutes>(duration - hrs);
      const auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration - hrs - mins);
      const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration - hrs - mins - secs);
      const auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration - hrs - mins - secs - ms);
      const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration - hrs - mins - secs - ms - us);
      
      if (hrs.count() > 0) {
         ss << hrs.count()
            << ":" << std::setw(2) << mins.count()
            << ":" << std::setw(2) << secs.count();
      }
      else if (mins.count() > 0) {
         ss << mins.count()
            << ":" << std::setw(2) << secs.count()
            << "." << std::setw(3) << ms.count();
      }
      else if (secs.count() > 0) {
         ss << secs.count()
            << "." << std::setw(3) << ms.count() << "s";
      }
      else if (ms.count() > 0) {
         ss << ms.count()
            << "." << std::setw(3) << us.count() << "ms";
      }
      else {
         ss << us.count() 
            << "." << std::setw(3) << ns.count() << "us";
      }

      return ss.str();
   }

}
