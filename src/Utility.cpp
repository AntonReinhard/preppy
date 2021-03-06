/**
 * @file Utility.cpp
 * @author Anton Reinhard
 * @brief Utility functions implementation
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Utility.h"
#include "CommandLineParser.h"
#include "definitions.h"
#include "solvers/clasp.h"

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
#include <argp.h>

namespace preppy::util {

   std::unique_ptr<log::Logger> Utility::logger = nullptr;

   std::shared_ptr<solvers::Solver> Utility::solver = nullptr;
   clock::duration Utility::solvingTimeout = std::chrono::seconds(5);

   log::LOG_LEVEL Utility::GLOBAL_LOG_LEVEL = log::LOG_LEVEL::WARNING;

   std::map<std::string, util::clock::time_point> Utility::timerStartPoints;

   bool Utility::init() {
      // always initialize the logger first
      Utility::initializeLogger(Utility::GLOBAL_LOG_LEVEL);
      Utility::initializeSignalHandling();

      Utility::solver = std::make_shared<solvers::clasp>(solvingTimeout);

      return true;
   }

   void Utility::cleanup() {
      logDebug("Cleaning up...");

      // logger should always be the last thing to be deleted
      Utility::logger.reset();
   }

   Arguments Utility::parseCommandLine(const int argc, char** argv) {
      CommandLineParser clp;
      clp.parse(argc, argv);

      Arguments args = clp.getArguments();

      util::Utility::GLOBAL_LOG_LEVEL = args.logLevel;

      return args;
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

   bool Utility::isDirectory(const std::string& path) {
      return std::filesystem::is_directory(path);
   }

   bool Utility::isFile(const std::string& path) {
      return std::filesystem::is_regular_file(path);
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

   std::string Utility::systemCall(const std::string& command) {    
      std::array<char, 128> buffer;
      std::string result;
      std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
      if (!pipe) {
         Utility::logError("System call \"", command, "\" failed");
         return std::string();
      }
      while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
         result += buffer.data();
      }
      return result;
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

   std::shared_ptr<solvers::Solver> Utility::getSolver() {
      return Utility::solver;
   }

   cnf::Variables Utility::literalsToVariables(const cnf::Literals& literals) {
      cnf::Variables vars;
      for (const auto& lit : literals) {
         vars.push_back(std::abs(lit));
      }
      return vars;
   }

   void Utility::setSolver(const std::string& solverName) {
      if ("clasp" == solverName) {
         Utility::logDebug("Using solver \"", solverName, "\"");
         Utility::solver.reset(new solvers::clasp(solvingTimeout));
      }
      else {
         Utility::logError("Can't set unknown solver \"", solverName, "\"");
      }
   }

}
