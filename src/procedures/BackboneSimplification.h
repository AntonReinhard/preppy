#pragma once

#include "Procedures.h"
#include "../solvers/Solver.h"
#include "../definitions.h"

#include <set>
#include <memory>

namespace preppy::procedures {

   class BackboneSimplification
      : public Procedure {
   public:

      BackboneSimplification(std::shared_ptr<solvers::Solver> solver);

      bool apply(cnf::CNF& formula) override;

      // computes the backbone of the given formula
      cnf::Literals getBackbone(const cnf::CNF& formula) const;

   protected:

   
   private:

      std::shared_ptr<solvers::Solver> solver;

   };

}
