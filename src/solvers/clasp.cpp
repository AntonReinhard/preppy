#include "clasp.h"
#include "../Utility.h"

#include <string>
#include <sstream>
#include <fstream>

namespace preppy::solvers {

   clasp::clasp(const util::clock::duration& timeout)
      : Solver("clasp")
      , timeout(timeout) {
   }

   bool clasp::isSatisfiable(cnf::CNF& formula) {
      util::Utility::logDebug("Satisfiability check using solver ", this->getName());

      // create command as string
      std::stringstream syscall;
      syscall << this->getName()
         << " 1 -q" // only search for one model -> SAT test, and quiet option cause we don't need the model itself
         << " --time-limit=" << std::chrono::duration_cast<std::chrono::seconds>(this->timeout).count()
         << " " << this->inFileName
         << " > " << this->outFileName << " 2>&1"; // redirect both stdout and stderr into outFile

      // Execute Solver      
      std::string solutionString;
      if (!this->executeSolver(syscall.str(), formula, solutionString)) {
         return false;
      }

      // Parse result
      if (solutionString.find("s SATISFIABLE") != std::string::npos) {
         util::Utility::logDebug("Formula is satisfiable");
         return true;
      }
      else if (solutionString.find("s UNSATISFIABLE") != std::string::npos) {
         util::Utility::logDebug("Formula is unsatisfiable");
         return false;
      }
      else if (solutionString.find("INTERRUPTED by signal")) {
         util::Utility::logDebug("Formula timed out or was otherwise interrupted");
         return false;
      }
      
      util::Utility::logWarning("Unexpected solution from solver");
      return false;
   }

   cnf::Model clasp::getModel(cnf::CNF& formula) {
      util::Utility::logDebug("Getting model using solver ", this->getName());

      // create command as string
      std::stringstream syscall;
      syscall << this->getName()
         << " 1"                                   // search for any one model and output it
         << " --time-limit=0"                      // no timeout, we need to find a model
         << " " << this->inFileName
         << " > " << this->outFileName << " 2>&1"; // redirect both stdout and stderr into outFile

      // Execute Solver
      std::string solutionString;
      if (!this->executeSolver(syscall.str(), formula, solutionString)) {
         return cnf::Model(); // if something fails return empty model, executeSolver will print the error messages
      }

      // Parse result
      if (solutionString.find("s UNSATISFIABLE") != std::string::npos) {
         util::Utility::logDebug("Formula is unsatisfiable");
         return cnf::Model();    // return empty model if the formula was unsatisfiable
      }
      else if (solutionString.find("INTERRUPTED by signal") != std::string::npos) {
         util::Utility::logDebug("Formula timed out or was otherwise interrupted");
         return cnf::Model();
      }
      else if (solutionString.find("s SATISFIABLE") != std::string::npos) {
         // If there was a result, read the model
         std::istringstream ss(solutionString);
         cnf::Model result;
         result.reserve(formula.getMaxVariable());
         result.readFromStream(ss);
         return result;
      }

      util::Utility::logWarning("Unexpected solution from solver");
      return cnf::Model();
   }

}
