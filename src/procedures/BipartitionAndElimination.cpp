#include "BipartitionAndElimination.h"

namespace preppy::procedures {

   bool BipartitionAndElimination::apply(cnf::CNF& formula) {
      Variables outputVariables = this->bipartition(formula);

      this->eliminate(formula, outputVariables);

      return true;
   }

   Variables BipartitionAndElimination::bipartition(const cnf::CNF& formula) const {

      return {{}, {}};
   }

   void BipartitionAndElimination::eliminate(cnf::CNF& formula, Variables variables) const {

   }

}
