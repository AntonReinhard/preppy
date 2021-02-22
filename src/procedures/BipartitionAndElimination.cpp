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
#include "Vivification.h"
#include "OccurrenceSimplification.h"
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

      for (std::size_t i = 1; i < appearances.size(); ++i) {
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

      for (std::size_t i = 0; i < sortedAppearances.size(); ++i) {
         cnf::Variables definitionSet = inputVariables;
         for (std::size_t j = i+1; j < sortedAppearances.size(); ++j) {
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
      cnf::CNF workingFormula = formula;

      procedures::Vivification vivification;
      procedures::OccurrenceSimplification occurrenceSimplification;

      bool iterate = true;

      while (iterate) {

         // line 4
         cnf::Variables Eliminate = variables;
         variables.clear();
         iterate = false;

         // line 5
         vivification.apply(workingFormula);

         // line 6
         while (!Eliminate.empty()) {
            // line 7
            this->sort(workingFormula, Eliminate);
            unsigned x = variables[0];

            // line 8
            variables.erase(variables.begin());

            // line 9
            occurrenceSimplification.applySingleLiteral(workingFormula, static_cast<int>(x));
            occurrenceSimplification.applySingleLiteral(workingFormula, -static_cast<int>(x));

            // line 10
            int occurrencesPos = 0;
            int occurrencesNeg = 0;

            for (const auto& clause : formula) {
               if (std::find(clause->begin(), clause->end(), x) != clause->end()) {
                  occurrencesPos++;
               }
               if (std::find(clause->begin(), clause->end(), -x) != clause->end()) {
                  occurrencesNeg++;
               }
            }

            if (occurrencesNeg * occurrencesPos > maxNumberRes) {
               // line 11  -> possibly postpone elimination
               variables.emplace_back(x);
            }
            // line 12
            else {
               // line 13
               



            }
         }
      }


      formula = workingFormula;
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

   void BipartitionAndElimination::sort(const cnf::CNF& formula, cnf::Variables& variables) {
      std::unordered_map<int, int> occurences;
      for (int var : variables) {
         occurences[var] = 0;
         occurences[-var] = 0;
      }

      for (const auto& clause : formula) {
         for (int var : variables) {
            if (std::find(clause->begin(), clause->end(), var) != clause->end()) {
               occurences[var]++;
            }
            if (std::find(clause->begin(), clause->end(), -var) != clause->end()) {
               occurences[-var]++;
            }
         }
      }

      std::sort(variables.begin(), variables.end(), [&occurences](int var1, int var2){
         return occurences[var1] * occurences[-var1] < occurences[var2] * occurences[-var2];
      });
   }

}
