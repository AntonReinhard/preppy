/**
 * @file Vivification.h
 * @author Anton Reinhard
 * @brief Vivification Procedure implementation
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Vivification.h"
#include "BooleanConstraintPropagation.h"
#include "../Utility.h"

#include <algorithm>

namespace preppy::procedures {

   Vivification::Vivification()
      : Procedure("Vivification", cnf::EQUIVALENCE_TYPE::EQUIVALENT) {

   }

   bool Vivification::impl(cnf::CNF &formula) {
      cnf::CNF newFormula = formula.getMetadataCopy();

      procedures::BooleanConstraintPropagation bcp;

      int totalBcpCalls = 0;
      util::clock::duration bcpDuration(0);

      while (formula.size() != 0) {
         // take first clause and delete it from the formula
         cnf::Clause currentClause = formula[0];
         formula.erase(formula.begin());

         cnf::Clause newClause;

         cnf::CNF tempFormula = formula;
         tempFormula.joinFormula(newFormula);
         // take bcp of formula ∪ newFormula

         util::Utility::startTimer("getBcp");
         auto bcpLiterals = bcp.getBcp(tempFormula);
         ++totalBcpCalls;
         bcpDuration += util::Utility::stopTimer("getBcp");

         bool satisfied = false;

         while (currentClause.size() != 0) {
            // choose literal from currentClause such that its negation is not part of bcpLiterals
            int l = 0;
            for (size_t i = 0; i < currentClause.size(); ++i) {
               int lit = currentClause[i];
               if (std::find(bcpLiterals.begin(), bcpLiterals.end(), -lit) == bcpLiterals.end()) {
                  // -l wasn't found -> choose this l
                  l = lit;
                  currentClause.erase(currentClause.begin() + i);
                  break;
               }
            }

            if (l == 0) {
               // if none was found break
               break;
            }

            // add l to newClause
            newClause.push_back(l);

            // get complementary clause (consisting of all negated literals)
            cnf::Clause complementaryNewClause = newClause.getComplement();

            // take bcp of formula ∪ newFormula
            tempFormula.push_back(complementaryNewClause);

            util::Utility::startTimer("getBcp");
            bcpLiterals = bcp.getBcp(tempFormula);
            ++totalBcpCalls;
            bcpDuration += util::Utility::stopTimer("getBcp");

            //delete complementaryNewClause again
            tempFormula.erase(tempFormula.end() - 1);

            if (!bcpLiterals.empty() && bcpLiterals[0] == 0) {
               // if new Clause is satisfied -> break
               satisfied = true;
               break;
            }
         }

         if (!satisfied) {
            newFormula.push_back(newClause);
         }
      }

      formula = newFormula;

      util::Utility::logInfo("Total getBcp Calls: ", totalBcpCalls);
      util::Utility::logInfo("Total getBcp Time:  ", util::Utility::durationToString(bcpDuration));

      return true;
   }

}
