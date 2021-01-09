/**
 * @file CommandLineParser.cpp
 * @author Anton Reinhard
 * @brief Commandline Parser implementation
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "CommandLineParser.h"
#include "Utility.h"

#include <sstream>
#include <argp.h>

namespace preppy::util {

   Arguments CommandLineParser::args;

   CommandLineParser::CommandLineParser() {
      argp_program_version_hook = CommandLineParser::printVersion;
      CommandLineParser::args.logLevel = log::WARNING;
      CommandLineParser::args.force = false;
   }

   void CommandLineParser::parse(int argc, char **argv) {
      argp parser = { CommandLineParser::options, CommandLineParser::parseOption, CommandLineParser::argsDoc, CommandLineParser::doc };
      argp_parse(&parser, argc, argv, 0, 0, &CommandLineParser::args);

      if (!util::Utility::fileExists(CommandLineParser::args.fileIn)) {
         //input file does not exist
         std::cout << "Input file '" << args.fileIn << "' does not exist." << std::endl;
         exit(0);
      }
   }

   error_t CommandLineParser::parseOption(int key, char *arg, struct argp_state *state) {
      // input will contain the Argument struct
      Arguments *arguments = static_cast<Arguments*>(state->input);

      switch (key) {
      case 'v':      // --verbose <LOG_LEVEL>
      {
         std::istringstream ss(arg);
         int i;
         ss >> i;
         if (ss.fail() || i < 0 || i > log::DEBUG) {
            return 1;
         }
         arguments->logLevel = static_cast<log::LOG_LEVEL>(i);
         break;
      }
      case 'o':      // --output <FILE or DIRECTORY>
         arguments->fileOut = arg;
         break;
      case 'f':      // --force
         arguments->force = true;
         break;
      case ARGP_KEY_ARG:
         if (state->arg_num >= 1) {
            argp_usage(state);
         }
         arguments->fileIn = arg;
         break;
      case ARGP_KEY_END:
         if (state->arg_num < 1) {
            argp_usage(state);
         }
         break;
      default:
         return ARGP_ERR_UNKNOWN;
      }
      return 0;
   }

   Arguments CommandLineParser::getArguments() const {
      return CommandLineParser::args;
   }

}
