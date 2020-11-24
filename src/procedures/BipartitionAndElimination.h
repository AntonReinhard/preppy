#pragma once

#include "Procedures.h"
#include "../CNF.h"
#include "../definitions.h"

#include <vector>
#include <tuple>

namespace preppy::procedures {

   class BipartitionAndElimination
      : public Procedure {
   public:

      BipartitionAndElimination();

      bool apply(cnf::CNF& formula) override;

      // Returns a list of variables in the formula that are defined in terms of the other variables in the formula
      // Greedily calculated, so not necessarily all are found
      cnf::Variables bipartition(const cnf::CNF& formula) const;

   protected:

      // Eliminates the given variables from the formula
      void eliminate(cnf::CNF& formula, cnf::Variables variables) const;

      // Tests whether variable x is defined in terms of variables in formula
      bool isDefined(const unsigned x, const cnf::CNF& formula, const cnf::Variables& variables/*, unsigned maxC*/) const;

   private:


   };

}
