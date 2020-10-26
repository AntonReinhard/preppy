#pragma once

#include "Solver.h"
#include "../Utility.h"
#include "../Model.h"

#include <chrono>

namespace preppy::solvers {

   class clasp
      : public Solver {
   public:

      clasp(const util::clock::duration& timeout);

      ~clasp() = default;

      bool isSatisfiable(cnf::CNF& formula) override;
   
      cnf::Model getModel(cnf::CNF& formula) override;

   protected:


   private:

      util::clock::duration timeout;

   };

}