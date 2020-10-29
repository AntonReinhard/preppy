#include "BackboneSimplification.h"
#include "BooleanConstraintPropagation.h"
#include "../Model.h"
#include "../CNF.h"
#include "../Clause.h"
#include "../Utility.h"

#include <algorithm>
#include <cmath>

namespace preppy::procedures {

   BackboneSimplification::BackboneSimplification(std::shared_ptr<solvers::Solver> solver) 
      : solver(solver) {

   }

   bool BackboneSimplification::apply(cnf::CNF& formula) {
      literals backbone = this->getBackbone(formula);

      const uint64_t variablesWithoutBackbone = formula.getVariables() - backbone.size();
      util::Utility::logInfo("Expected number of variables after removing backbone: ", variablesWithoutBackbone);

      /*
      for (const auto& lit : backbone) {
         // Simply Adding the literals as known to the formula
         cnf::Clause newClause;
         newClause.push_back(lit);
         formula.emplace_back(newClause);
      }
      */

      BooleanConstraintPropagation bcp;
      bcp.applyLiterals(formula, backbone);

      //every non-backbone variable that disappears in this process can have any value -> halving number of possible models
      util::Utility::logInfo("Actual number of variables after removing backbone:   ", formula.getVariables());

      const uint64_t independentRemovedVariabels = variablesWithoutBackbone - formula.getVariables();

      util::Utility::logInfo("That means a factor of ", std::pow(2, independentRemovedVariabels));

      formula.compress();

      return true;
   }

   literals BackboneSimplification::getBackbone(cnf::CNF& formula) {
      util::Utility::logDebug("Computing Backbone");

      util::Utility::startTimer("backbone calculation");

      // Compute a model
      cnf::Model startingModel = this->solver->getModel(formula);

      if (startingModel.empty()) {
         // If the formula is unsatisfiable, the backbone is empty
         return {};
      }

      // Convert Model to a literalSet, this will stay uncompressed the entire time
      literals remainingLiterals;
      for (size_t i = 1; i < startingModel.size(); ++i) {
         if (startingModel[i]) {
            remainingLiterals.push_back(i);
         }
         else {
            remainingLiterals.push_back(-i);
         }
      }

      // create copy of the formula to work on
      cnf::CNF workingFormula = formula;
      literals backbone;

      size_t i = 0;

      BooleanConstraintPropagation bcp;

      // Main loop
      while (!remainingLiterals.empty()) {
         ++i;
         //util::Utility::startTimer("loop");

         // currently used literal, compress
         const int currentLiteral = /*workingFormula.compress*/(remainingLiterals[0]);

         cnf::Clause newLiteralClause;
         newLiteralClause.push_back(-currentLiteral);     // add negated literal

         workingFormula.push_back(newLiteralClause);
         cnf::Model model = this->solver->getModel(workingFormula);
         
         if (model.empty()) {
            // if there's no model then the literal is in the backbone
            // use the uncompressed remaining Literal for the backbone
            backbone.push_back(remainingLiterals[0]);
            workingFormula.erase(workingFormula.end());                    // remove last clause again
            
            bcp.applySingleLiteral(workingFormula, currentLiteral);        // can propagate the literal we learned
            //workingFormula.setLiteralBackpropagated(currentLiteral);

            remainingLiterals.erase(remainingLiterals.begin());      // remove the literal that was tried
         }
         else {
            //workingFormula.decompress(model);
            
            remainingLiterals.erase(remainingLiterals.begin());      // remove the literal that was tried

            remainingLiterals.erase(
               std::remove_if(remainingLiterals.begin(), remainingLiterals.end(), 
                  [&](int lit) {
                     return model[std::abs(lit)] != (lit > 0);
                  }
               ),
               remainingLiterals.end()
            );

            workingFormula.erase(workingFormula.end());           // remove last clause again

            if (remainingLiterals.empty()) {                      // just in case remaining literals has been emptied by remove_if
               break;
            }
         }

         //util::Utility::logWarning("Loop iteration ", i, " took ", util::Utility::durationToString(util::Utility::stopTimer("loop")));
      }

      util::Utility::logInfo("Backbone computation took ", util::Utility::durationToString(util::Utility::stopTimer("backbone calculation")));

      return backbone;
   }

}
