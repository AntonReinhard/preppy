#pragma once

#include "Procedures.h"
#include "../solvers/Solver.h"

#include <set>
#include <memory>

namespace preppy::procedures {

   // A ordered set of literals for quick lookup
   typedef std::vector<int> literals;

   class BackboneSimplification
      : public Procedure {
   public:

      BackboneSimplification(std::shared_ptr<solvers::Solver> solver);

      bool apply(cnf::CNF& formula) override;

      // computes the backbone of the given formula
      literals getBackbone(cnf::CNF& formula);

   protected:

   
   private:

      std::shared_ptr<solvers::Solver> solver;

   };

}
