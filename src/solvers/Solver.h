#pragma once

#include "../CNF.h"
#include "../Model.h"

namespace preppy::solvers {

   class Solver {
   public:

      // Constructs the solver with a name. The name should also be the systemcall
      Solver(std::string name);

      virtual ~Solver() = default;

      // Returns true if the formula is satisfiable
      virtual bool isSatisfiable(cnf::CNF& formula) = 0;

      // Returns a single model for the formula if it exists, or an empty one if not
      virtual cnf::Model getModel(cnf::CNF& formula) = 0;

      // Returns the name of this solver
      const std::string& getName();

   protected:

      // Helper function executing a solver with the given command, putting the result in the string solution
      bool executeSolver(const std::string& command, cnf::CNF& formula, std::string& solution);

      const std::string inFileName = "in.cnf";

   private:

      std::string solvername;

   };

}
