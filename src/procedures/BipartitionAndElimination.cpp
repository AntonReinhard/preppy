/**
 * @file BipartitionAndElimination.cpp
 * @author Anton Reinhard
 * @brief Bipartition and Elimination Procedure implementation
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "BipartitionAndElimination.h"
#include "BackboneSimplification.h"
#include "BooleanConstraintPropagation.h"
#include "../solvers/clasp.h"
#include "../definitions.h"

#include <algorithm>

namespace preppy::procedures {

   BipartitionAndElimination::BipartitionAndElimination() 
      : Procedure("Bipartition and Elimination", cnf::EQUIVALENCE_TYPE::NUMBER_EQUIVALENT) {

   }

   bool BipartitionAndElimination::impl(cnf::CNF& formula) {
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
      
      cnf::Variables backboneVariables;
      for (const auto& l : backbone) {
         backboneVariables.push_back(std::abs(l));
      }
      
      cnf::Variables outputVariables = util::Utility::literalsToVariables(backbone);
      cnf::Variables inputVariables;

      cnf::Variables appearances = workingFormula.countVariables();
      std::vector<std::pair<unsigned, unsigned>> sortedAppearances;     // first = variable, second = number of appearances

      for (size_t i = 1; i < appearances.size(); ++i) {
         //if it's in the backbone we don't need to add
         //so it's only added if it's not in the backbone
         if (std::find(backboneVariables.begin(), backboneVariables.end(), i) == backboneVariables.end()) { 
            sortedAppearances.emplace_back(i, appearances[i]);
         }
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
         for (size_t j = i+1; j < sortedAppearances.size(); ++j) {
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
      
      if (std::find(variables.begin(), variables.end(), x) != variables.end()) {     // if x is in the variables then it is defined
         return true;
      }

      // We need two copies to work on
      cnf::CNF workingFormula = formula;
      cnf::CNF copyFormula = formula;

      unsigned xPrime = 0;

      util::Utility::logDebug("Renaming variables");
      // rename every variable *not* in variables to an unassigned variable name in copyFormula
      unsigned newVariableName = copyFormula.getMaxVariable() + 1;
      for (unsigned var = 1; var <= workingFormula.getMaxVariable(); ++var) {
         if (std::find(variables.begin(), variables.end(), var) != variables.end()) {     // don't rename if it's found
            continue;
         }
         if (x == var) {   // remember what x is renamed to
            xPrime = newVariableName;
         }
         copyFormula.renameVariable(var, newVariableName);
         ++newVariableName;
      }

      util::Utility::logDebug("Joining Formulas");
      // add clauses from the copy to working Formula
      workingFormula.joinFormula(copyFormula);

      workingFormula.push_back(std::make_unique<cnf::Clause>(std::initializer_list<int>({int(x)})));
      workingFormula.push_back(std::make_unique<cnf::Clause>(std::initializer_list<int>({-int(xPrime)})));

      return !util::Utility::getSolver()->isSatisfiable(workingFormula);
   }
}
