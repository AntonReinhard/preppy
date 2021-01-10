/**
 * @file BooleanConstraintPropagation.cpp
 * @author Anton Reinhard
 * @brief Boolean Constraint Propagation procedure implementation
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "BooleanConstraintPropagation.h"
#include "../CNF.h"
#include "../definitions.h"

#include <algorithm>

namespace preppy::procedures {

   BooleanConstraintPropagation::BooleanConstraintPropagation()
      : Procedure("Boolean Constraint Propagation", cnf::EQUIVALENCE_TYPE::SAT_EQUIVALENT) {

   }

   bool BooleanConstraintPropagation::impl(cnf::CNF& formula) {
      // Unit propagation -> find unit clauses
      cnf::Literals units = this->getBcp(formula);

      // apply
      this->applyLiterals(formula, units);

      return true;
   }

   cnf::Literals BooleanConstraintPropagation::getBcp(cnf::CNF& formula) {
      this->reset();
      this->initWatchedLiterals(formula);

      // Unit propagation -> find unit clauses
      cnf::Literals units;
      for (const auto& clause : formula) {
         if (clause.size() == 1) {
            units.push_back(clause[0]);
         }
      }
      
      if (units.empty()) {
         return {};
      }

      for (size_t i = 0; i < units.size(); ++i) {
         auto literal = units[i];
         size_t index = std::abs(2 * literal) + (literal < 0);  // use < here because the negated literal needs to be checked now
         std::vector<cnf::Clause*>& watched = watchedLiterals[index];

         for (auto& clause : watched) {
            // check if the clause is satisfied
            if (clause->isSatisfied(units)) {
               continue;
            }

            auto partialClause = clause->getPartialClause(units);
            if (partialClause.size() == 1) {
               // formula became unsatisfiable
               if (partialClause[0] == 0) {
                  return {0};
               }
               // new unit clause -> add to units
               // do nothing else since if we come across the same clause again it will be satisfied and not get here
               units.push_back(partialClause[0]);
            }
            else {
               // there's 2 or more literals
               // choose a new literal to watch
               // we always watch the first two, so without the one currently being propagated, start watching the second now
               size_t newIndex = std::abs(2 * partialClause[1]) + (partialClause[1] > 0);
               watchedLiterals[newIndex].push_back(clause);
            }
         }
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

   void BooleanConstraintPropagation::applyLiterals(cnf::CNF& formula, const cnf::Literals& literals) const {
      for (const auto& literal : literals) {
         this->applySingleLiteral(formula, literal);
      }
   }

   void BooleanConstraintPropagation::applyLiteralsEq(cnf::CNF& formula, const cnf::Literals& literals) const {
      for (const auto& literal : literals) {
         this->applySingleLiteralEq(formula, literal);
      }
   }

   void BooleanConstraintPropagation::reset() {
      this->watchedLiterals.clear();
   }

   void BooleanConstraintPropagation::initWatchedLiterals(cnf::CNF& formula) {
      // 2l points is for literal -l, index 2l + 1 for literal l
      this->watchedLiterals.resize(2 * (formula.getMaxVariable() + 2));

      for (auto& clause : formula) {
         if (clause.size() <= 1) {
            // nothing to be done
            continue;
         }

         //if there's at least 2 literals, add the first two literals of the clause to the watched literals
         size_t index = std::abs(2 * clause[0]) + (clause[0] > 0);
         watchedLiterals[index].push_back(&clause);
         index = std::abs(2 * clause[1]) + (clause[1] > 0);
         watchedLiterals[index].push_back(&clause);
      }
   }

}
