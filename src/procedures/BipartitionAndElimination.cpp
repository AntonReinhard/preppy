#include "BipartitionAndElimination.h"
#include "../solvers/clasp.h"
#include "BackboneSimplification.h"
#include "BooleanConstraintPropagation.h"
#include "../definitions.h"

#include <algorithm>

namespace preppy::procedures {

   bool BipartitionAndElimination::apply(cnf::CNF& formula) {
      cnf::Variables outputVariables = this->bipartition(formula);

      this->eliminate(formula, outputVariables);

      return true;
   }

   cnf::Variables BipartitionAndElimination::bipartition(const cnf::CNF& formula) const {
      // copy formula so we can simplify it as we go
      cnf::CNF workingFormula = formula;
      
      BackboneSimplification bs(util::Utility::getSolver());
      BooleanConstraintPropagation bcp;
      cnf::Literals backbone = bs.getBackbone(workingFormula);
      bcp.applyLiteralsEq(workingFormula, backbone);
      
      cnf::Variables outputVariables = util::Utility::literalsToVariables(backbone);
      cnf::Variables inputVariables{};

      cnf::Variables appearances = workingFormula.countVariables();
      std::vector<std::pair<unsigned, unsigned>> sortedAppearances;     // first = variable, second = number of appearances

      for (size_t i = 1; i < appearances.size(); ++i) {
         sortedAppearances.emplace_back(i, appearances[i]);
      }

      std::sort(
         sortedAppearances.begin(), 
         sortedAppearances.end(), 
         [](std::pair<unsigned, unsigned> a, std::pair<unsigned, unsigned> b){
            return a.second < b.second;
         }
      );

      util::Utility::logDebug("Sorted");

      for (size_t i = 0; i < sortedAppearances.size(); ++i) {
         cnf::Variables definitionSet = inputVariables;
         for (size_t j = i; j < sortedAppearances.size(); ++j) {
            definitionSet.push_back(sortedAppearances[j].first);
         }
         
         bool isDefined = this->isDefined(sortedAppearances[i].first, workingFormula, definitionSet);

         if (isDefined) {
            outputVariables.push_back(sortedAppearances[i].first);
         }
         else {
            inputVariables.push_back(sortedAppearances[i].first);
         }
      }

      return outputVariables;
   }

   void BipartitionAndElimination::eliminate(cnf::CNF& formula, cnf::Variables variables) const {

   }

   bool BipartitionAndElimination::isDefined(const unsigned x, const cnf::CNF& formula, const cnf::Variables& variables/*, unsigned maxC*/) const {
      // generate formula to check for satisfiability

      util::Utility::logDebug("Checking definedness of ", x);

      // We need two copies to work on
      cnf::CNF workingFormula = formula;
      cnf::CNF copyFormula = formula;

      unsigned xPrime = 0;

      util::Utility::logDebug("Renaming variables");
      // rename every variable *not* in variables to an unassigned variable name in copyFormula
      unsigned newVariableName = copyFormula.getMaxVariable() + 1;
      for (unsigned var = 1; var < workingFormula.getMaxVariable(); ++var) {
         if (std::find(variables.begin(), variables.end(), var) != variables.end()) {     // don't rename if it's found
            continue;
         }
         if (x == var) {   // remember what x is renamed to
            xPrime = var;
         }
         copyFormula.renameVariable(var, newVariableName);
         ++newVariableName;
      }

      util::Utility::logDebug("Joining Formulas");
      // add clauses from the copy to working Formula
      workingFormula.joinFormula(copyFormula);

      workingFormula.push_back(cnf::Clause{int(x)});
      workingFormula.push_back(cnf::Clause{-int(xPrime)});

      return !util::Utility::getSolver()->isSatisfiable(workingFormula);
   }

}
