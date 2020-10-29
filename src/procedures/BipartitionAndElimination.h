#pragma once

#include "Procedures.h"
#include "../CNF.h"

#include <vector>
#include <tuple>

namespace preppy::procedures {

   typedef std::vector<unsigned> Variables;

   class BipartitionAndElimination
      : public Procedure {
   public:

      BipartitionAndElimination() = default;

      bool apply(cnf::CNF& formula) override;

   protected:

      // Returns a list of variables in the formula that are defined in terms of the other variables in the formula
      // Greedily calculated, so not necessarily all are found
      Variables bipartition(const cnf::CNF& formula) const;

      // Eliminates the given variables from the formula
      void eliminate(cnf::CNF& formula, Variables variables) const;

   private:


   };

}
