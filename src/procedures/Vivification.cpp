#include "Vivification.h"
#include "BooleanConstraintPropagation.h"

#include <algorithm>

namespace preppy::procedures {

    bool Vivification::apply(cnf::CNF& formula) {
        cnf::CNF newFormula;
        
        procedures::BooleanConstraintPropagation bcp;

        while (formula.size() != 0) {
            // take first clause and delete it from the formula
            cnf::Clause currentClause = formula[0];
            formula.erase(formula.begin());

            cnf::Clause newClause;

            cnf::CNF tempFormula = formula;
            tempFormula.joinFormula(newFormula);
            // take bcp of formula ∪ newFormula
            auto bcpLiterals = bcp.getBcp(tempFormula);

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

                // make complementary clause (consisting of all negated literals)
                cnf::Clause complementaryNewClause;
                for (const auto& lit : newClause) {
                    complementaryNewClause.push_back(-lit);
                }

                // take bcp of formula ∪ newFormula
                tempFormula.push_back(complementaryNewClause);
                bcpLiterals = bcp.getBcp(tempFormula);
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

        return true;
    }

}
