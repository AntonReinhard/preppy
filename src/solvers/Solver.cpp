/**
 * @file Solver.cpp
 * @author Anton Reinhard
 * @brief Solver base class implementation
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Solver.h"
#include "../Utility.h"

namespace preppy::solvers {

   Solver::Solver(std::string name)
      : solvername(std::move(name)) {
      util::Utility::logDebug("Solver \"", this->solvername, "\" has been initialized");
   }

   const std::string& Solver::getName() {
      return this->solvername;
   }

   bool Solver::executeSolver(const std::string& command, cnf::CNF& formula, std::string& solution) {
      util::Utility::startTimer("writeFile");

      // Write formula to file
      if (!formula.writeToFile(this->inFileName, true)) {
         util::Utility::logError("Couldn't write formula to file ", this->inFileName);
         return false;
      }
      util::Utility::logDebug("Writing formula to file took ", util::Utility::durationToString(util::Utility::stopTimer("writeFile")));

      util::Utility::logDebug("Executing \"", command, "\"");

      solution = util::Utility::systemCall(command);

      if (solution.empty()) {
         util::Utility::logError("Couldn't read solution from solver \"", this->getName(), "\"");
         return false;
      }

      // Remove left-over files
      if (remove(this->inFileName.c_str())) {
         util::Utility::logInfo("Couldn't delete cnf file \"", this->inFileName, "\"");
      }

      return true;
   }

}
