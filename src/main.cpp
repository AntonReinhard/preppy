/**
 * @file main.cpp
 * @author Anton Reinhard
 * @brief main file
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "CNF.h"
#include "Clause.h"
#include "Utility.h"
#include "CommandLineParser.h"
#include "procedures/ProcedureManager.h"
#include "procedures/BackboneSimplification.h"
#include "procedures/OccurrenceSimplification.h"
#include "procedures/Vivification.h"

#include <string>

using namespace preppy;

int main(const int argc, char** argv) {
   // initialization
   util::Arguments args = util::Utility::parseCommandLine(argc, argv);
   util::Utility::init();

   // read formula
   cnf::CNF formula;
   if (!formula.readFromFile(args.fileIn)) {
      return 1;
   }

   // setup procedures and apply
   procedures::ProcedureManager manager(args.iterations);
   manager.addOneTimeProcedure(std::make_unique<procedures::Vivification>());
   manager.apply(formula);

   // output result
   if (args.fileOut.empty()) {
      formula.writeToFile("output/", args.force);
   }
   else {
      formula.writeToFile(args.fileOut, args.force);
   }

   // cleanup
   util::Utility::cleanup();

   return 0;
}
