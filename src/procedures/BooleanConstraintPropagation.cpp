#include "BooleanConstraintPropagation.h"

#include <algorithm>

namespace preppy::procedures {

   bool BooleanConstraintPropagation::apply(cnf::CNF& formula) {
      // TODO

      return false;
   }

   void BooleanConstraintPropagation::applySingleLiteral(cnf::CNF& formula, const int literal) {
      // set literals on all clauses and remove the ones that are satisfied
      formula.erase(
         std::remove_if(formula.begin(), formula.end(),
            [&](cnf::Clause& clause){
               return clause.setLiteral(literal);
            }
         ),
         formula.end()
      );
      formula.setDirtyBitsTrue();
   }

   void BooleanConstraintPropagation::applyLiterals(cnf::CNF& formula, const literals& literals) {
      for (const auto& literal : literals) {
         this->applySingleLiteral(formula, literal);
      }
   }

}