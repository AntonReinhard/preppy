/**
 * @file CommandLineParser.h
 * @author Anton Reinhard
 * @brief Commandline Parser header
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "definitions.h"

#ifndef WIN32
#include <argp.h>
#else
// alternate definitions in case of windows compilation
enum error_t {
   SUCCESS = 0,
   ARGP_ERROR_UNKNOWN = 1
};

struct argp_option {
   std::string name;          // long version of the option
   char abbr;                 // short version of the option, or identifier
   std::string hint;          // hint for writing the command template
   int type;                  // alias or no
   std::string description;   // long description of the option
};

enum {
   OPTION_SINGLE = 0,
   OPTION_ALIAS = 1
};

#endif

namespace preppy::util {

   /**
    * @brief Arguments struct keeping all program arguments and options together
    * 
    */
   struct Arguments {
      std::string fileIn;
      std::string fileOut;
      log::LOG_LEVEL logLevel;
      unsigned iterations;
      bool force;
   };

   /**
    * @brief The Commandline Parser for the program, parsing and storing arguments and options
    * 
    */
   class CommandLineParser {
   public:

      /**
       * @brief Constructor initializing program arguments and options
       * 
       */
      CommandLineParser();

      /**
       * @brief Call with the main arguments to parse. Will exit if a unexpected options or arguments are encountered
       * 
       * @param argc The number of commandline arguments
       * @param argv The commandline arguments
       */
      void parse(int argc, char **argv);

      /**
       * @brief Get the Arguments object
       * 
       * @return Arguments The parsed arguments from the command line
       */
      Arguments getArguments() const;

   protected:

#ifndef WIN32
      /**
       * @brief Argp parse_opt function implementation
       * 
       * @param key The key of the option to parse
       * @param arg argument given with the option
       * @param state argp state, input contains Arguments struct
       * @return error_t Error code on exit, 0 on success
       */
      static error_t parseOption(int key, char *arg, struct argp_state *state);

      /**
       * @brief Function printing version information
       * 
       * @param stream Argument from argp, not used
       * @param state Argument from argp, not used
       */
      static void printVersion(FILE *stream, struct argp_state *state) {
         for (const auto& str : preppy::util::logoLines) {
            std::cout << str << std::endl;
         }
      }
#endif

   private:

      /**
       * @brief Documentation string of the program
       * 
       */
      const char *doc = "preppy -- A Preprocessor for SAT Model Counting";

      /**
       * @brief Description of program arguments
       * 
       */
      const char *argsDoc = "<INPUT FILE>";

#ifndef WIN32
      /**
       * @brief Description of the available options for this program
       * 
       */
      const argp_option options[6] = {
         {"verbose",       'v', "<LOG_LEVEL>",        0, "Set log level of the application, 0 = NOTHING, 1 = ERROR, 2 = WARNING (default), 3 = INFO, 4 = DEBUG"},
         {"logging",       'l', 0,                    OPTION_ALIAS},
         {"output",        'o', "<FILE/DIRECTORY>",   0, "Set a output file or directory. If a directory is set the output file name will be <inputfilestem>out.cnf"},
         {"force",         'f', 0,                    0, "Override output files if they already exist"},
         {"iterations",    'i', "<n>",                0, "How many iterations to use iterative procedures (default: 10)"},
         { 0 }
      };
#endif
      
      /**
       * @brief The globally set arguments
       * 
       */
      static Arguments args;

   };

}
