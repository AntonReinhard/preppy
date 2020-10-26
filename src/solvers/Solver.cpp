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

      if (!util::Utility::systemCall(command)) {
         util::Utility::logError("Couldn't execute solver ", this->getName(), ", exiting");
         return false;
      }

      // Read solution
      std::ifstream solutionFile(this->outFileName);
      if (!solutionFile.is_open()) {
         util::Utility::logError("Couldn't open cnf solution file ", this->outFileName);
         return false;
      }

      std::stringstream solutionSS;
      solutionSS << solutionFile.rdbuf();
      solution = solutionSS.str();

      solutionFile.close();

      // Remove left-over files
      if (remove(this->inFileName.c_str())) {
         util::Utility::logInfo("Couldn't delete cnf file \"", this->inFileName, "\"");
      }
      if (remove(this->outFileName.c_str())) {
         util::Utility::logInfo("Couldn't delete solutions file\"", this->outFileName, "\"");
      }

      return true;
   }

}
