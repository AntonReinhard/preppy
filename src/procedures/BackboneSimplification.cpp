#include "BackboneSimplification.h"
#include "../Model.h"
#include "../CNF.h"
#include "../Clause.h"
#include "../Utility.h"

#include <algorithm>

namespace preppy::procedures {

   BackboneSimplification::BackboneSimplification(std::shared_ptr<solvers::Solver> solver) 
      : solver(solver) {

   }

   bool BackboneSimplification::apply(cnf::CNF& formula) {
      //TODO

      return true;
   }

   literals BackboneSimplification::getBackbone(cnf::CNF& formula) {
      util::Utility::logDebug("Computing Backbone");

      // Compute a model
      cnf::Model startingModel = this->solver->getModel(formula);

      if (startingModel.empty()) {
         // If the formula is unsatisfiable, the backbone is empty
         return {};
      }

      // Convert Model to a literalSet
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

      // Main loop
      while (!remainingLiterals.empty()) {
         cnf::Clause newLiteralClause;
         newLiteralClause.push_back(-(remainingLiterals[0]));     // add negated literal

         workingFormula.push_back(newLiteralClause);
         cnf::Model model = this->solver->getModel(workingFormula);
         
         if (model.empty()) {
            // if there's no model then the literal is in the backbone
            backbone.push_back(remainingLiterals[0]);
            workingFormula.erase(workingFormula.end());           // remove last clause again
            
            cnf::Clause learnedClause;
            learnedClause.push_back(remainingLiterals[0]);
            workingFormula.push_back(learnedClause);              // we have learnt this
         }
         else {
            std::remove_if(remainingLiterals.begin(), remainingLiterals.end(), 
               [&](int lit) {
                  return model[std::abs(lit)] == (lit > 0);
               }
            );
            workingFormula.erase(workingFormula.end());           // remove last clause again
         }

         remainingLiterals.erase(remainingLiterals.begin());      // remove the literal that was tried
      }

      return backbone;
   }

}
