/**
 * @file OccurrenceSimplification.cpp
 * @author Anton Reinhard
 * @brief Occurrence Simplification Procedure implementation
 * @version 0.1
 * @date 2021-01-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "OccurrenceSimplification.h"
#include "BooleanConstraintPropagation.h"

#include <map>
#include <vector>
#include <algorithm>

namespace preppy::procedures {

   OccurrenceSimplification::OccurrenceSimplification()
      : Procedure("Occurrence Simplification", cnf::EQUIVALENCE_TYPE::EQUIVALENT) {

   }

   bool OccurrenceSimplification::impl(cnf::CNF& formula) {
      cnf::Literals literals = this->getIncidenceSortedLiterals(formula);

      while (!literals.empty()) {
         // get most common literal from the list and remove it from the list
         const int literal = literals[0];
         literals.erase(literals.begin());

         this->applySingleLiteral(formula, literal);
      }

      //remove any clauses that became empty
      formula.erase(
         std::remove_if(formula.begin(), formula.end(), [](const std::unique_ptr<cnf::Clause>& clause){
            return clause->size() == 0;
         }), 
         formula.end()
      );

      return false;
   }

   void OccurrenceSimplification::applySingleLiteral(cnf::CNF& formula, int literal) {
      procedures::BooleanConstraintPropagation bcp;

      for (auto& clause : formula) {
         // unit clauses can't have their literals removed by this anyways -> skip
         if (clause->size() == 1) {
            continue;
         }
         // consider only clauses that contain the literal
         if (!clause->containsLiteral(literal)) {
            continue;
         }

         // make possible new clause
         cnf::Clause newClause = *clause;
         newClause.erase(std::find(newClause.begin(), newClause.end(), literal));
         
         formula.push_back(std::make_unique<cnf::Clause>(newClause.getComplement()));
         formula.push_back(std::make_unique<cnf::Clause>(std::initializer_list<int>({literal})));
         cnf::Literals bcpLiterals = bcp.getBcp(formula);
         // remove the two clauses again
         formula.pop_back();
         formula.pop_back();

         if (!bcpLiterals.empty() && bcpLiterals[0] == 0) {
            // if it became unsatisfiable the literal can be removed from the clause
            clause->erase(std::find(clause->begin(), clause->end(), literal));
         }
      }
   }

   cnf::Literals OccurrenceSimplification::getIncidenceSortedLiterals(cnf::CNF& formula) const {
      // map from the literal onto its number of occurrences in the formula
      std::map<int, int> literalOccurenceMap;
      
      // the sorted list of literals to return at the end
      cnf::Literals literals;
      literals.reserve(formula.getVariables() * 2);

      // Add every literal positive and negative to the map and to list of literals
      for (auto var = 1; var <= formula.getVariables(); ++var) {
         literalOccurenceMap[var] = 0;
         literalOccurenceMap[-var] = 0;
         literals.emplace_back(var);
         literals.emplace_back(-var);
      }

      // Collect their number of occurrences
      for (const auto& clause : formula) {
         for (const auto& literal : *clause) {
            ++literalOccurenceMap[literal];
         }
      }

      // remove any literals that don't appear at all
      literals.erase(
         std::remove_if(literals.begin(), literals.end(), [&](int l) {
            return literalOccurenceMap[l] == 0;
         }),
         literals.end()
      );

      // sort literals by occurrence
      std::sort(literals.begin(), literals.end(), [&](int a, int b) {
         return literalOccurenceMap[a] > literalOccurenceMap[b];
      });

      return literals;
   }

}
