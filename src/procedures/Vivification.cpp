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
            auto bcpLiterals = bcp.getBcp(formula);

            while (currentClause.size() != 0) {
                // if new Clause is satisfied -> break
                if (newClause.size() != 0 && newClause[0] == 0) {
                    break;
                }

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
                cnf::CNF tempFormula(formula);
                tempFormula.joinFormula(newFormula);
                tempFormula.push_back(complementaryNewClause);

                bcpLiterals = bcp.getBcp(tempFormula);

                if (!bcpLiterals.empty() && bcpLiterals[0] == 0) {
                    newClause.insert(newClause.begin(), 0);
                }
            }

            if (newClause[0] != 0) {
                newFormula.push_back(newClause);
            }
        }

        formula = newFormula;

        return true;
    }

}
