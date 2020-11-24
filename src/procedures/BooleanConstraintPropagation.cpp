#include "BooleanConstraintPropagation.h"

#include <algorithm>

namespace preppy::procedures {

   bool BooleanConstraintPropagation::apply(cnf::CNF& formula) {
      // Unit propagation -> find unit clauses
      cnf::Literals units;
      for (const auto& clause : formula) {
         if (clause.size() == 1) {
            units.push_back(clause[0]);
         }
      }
      
      if (units.empty()) {
         return false;
      }

      this->applyLiterals(formula, units);
      this->apply(formula);   // new unit clauses could have been made -> try again
      // this is not very efficient

      return true;
   }

   cnf::Literals BooleanConstraintPropagation::getBcp(cnf::CNF& formula) const {
      cnf::CNF workingFormula{formula};

      // saves pointers to clauses, each clause is in here exactly twice (except for unit clauses)
      // index 2l points is for literal -l, index 2l + 1 for literal l
      std::vector<std::vector<cnf::Clause*>> watchedLiterals(2*(formula.getMaxVariable()+1));
      // Unit propagation -> find unit clauses
      cnf::Literals units;
      for (auto& clause : formula) {
         if (clause.size() == 1) {
            units.push_back(clause[0]);
            continue;
         }
         if (clause.size() == 0) {
            continue;
         }

         //if there's at least 2 literals, add the first two literals of the clause to the watched literals
         size_t index = std::abs(clause[0]) + (clause[0] > 0);
         watchedLiterals.at(index).push_back(&clause);
         index = std::abs(clause[1]) + (clause[1] > 0);
         watchedLiterals.at(index).push_back(&clause);
      }
      
      if (units.empty()) {
         return {};
      }

      for (auto i = 0; i < units.size(); ++i) {
         auto literal = units[i];
         size_t index = std::abs(literal) + (literal > 0);
         std::vector<cnf::Clause*>& watched = watchedLiterals.at(index);
      }


      return units;
   }

   void BooleanConstraintPropagation::applySingleLiteral(cnf::CNF& formula, const int literal) const {
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

   void BooleanConstraintPropagation::applySingleLiteralEq(cnf::CNF& formula, const int literal) const {
      this->applySingleLiteral(formula, literal);
      formula.push_back(cnf::Clause{literal});
   }

   void BooleanConstraintPropagation::applyLiterals(cnf::CNF& formula, const literals& literals) const {
      for (const auto& literal : literals) {
         this->applySingleLiteral(formula, literal);
      }
   }

   void BooleanConstraintPropagation::applyLiteralsEq(cnf::CNF& formula, const literals& literals) const {
      for (const auto& literal : literals) {
         this->applySingleLiteralEq(formula, literal);
      }
   }

}
