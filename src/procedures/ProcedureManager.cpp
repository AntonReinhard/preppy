/**
 * @file ProcedureManager.cpp
 * @author Anton Reinhard
 * @brief Procedure Manager implementation
 * @version 0.1
 * @date 2021-01-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "ProcedureManager.h"
#include "Procedure.h"
#include "../Utility.h"
#include "../definitions.h"

namespace preppy::procedures {

   ProcedureManager::ProcedureManager(const unsigned iterations, const std::pair<unsigned, unsigned>& minLiteralAndClauseReduction)
      : iterations(iterations)
      , minLiteralReduction(minLiteralAndClauseReduction.first)
      , minClauseReduction(minLiteralAndClauseReduction.second) {

   }

   void ProcedureManager::apply(cnf::CNF& formula) {
      util::Utility::startTimer("mainProcessing");

      // Remember state before doing anything
      unsigned clausesStart = formula.getClauses();
      unsigned literalsStart = formula.getLiterals();

      util::Utility::logOutput("The formula has ", clausesStart, " clauses and ", literalsStart, " literals");
      
      // execute one time procedures in order
      for (const auto& procedure : this->oneTimeProcedures) {
         util::Utility::logOutput("Applying one-time procedure ", procedure->getName(), "...");
         procedure->apply(formula);
      }

      unsigned clausesFixpointStart = formula.getClauses();
      unsigned literalsFixpointStart = formula.getLiterals();

      util::Utility::logOutput("After one-time procedures the formula has ", clausesFixpointStart, " clauses and ", literalsFixpointStart, " literals");

      // loop iterative procedures
      bool fixpointReached = false;
      unsigned i = 0;
      while (i < this->iterations && !fixpointReached) {
         ++i;
         util::Utility::logOutput("Iteration ", i, "...");

         clausesFixpointStart = formula.getClauses();
         literalsFixpointStart = formula.getLiterals();

         for (const auto& procedure : this->iterativeProcedures) {
            util::Utility::logInfo("Applying iterative procedure ", procedure->getName(), " in iteration ", i, "...");
            procedure->apply(formula);
         }

         // check for fixpoint (this assumes that procedures can't *add* clauses or literals)
         fixpointReached = true;
         if (clausesFixpointStart - formula.getClauses() > this->minClauseReduction) {
            fixpointReached = false;
         }
         if (literalsFixpointStart - formula.getLiterals() > this->minLiteralReduction) {
            fixpointReached = false;
         }
      }

      if (fixpointReached) {
         util::Utility::logInfo("Fixpoint reached");
      }
      else {
         util::Utility::logInfo("No Fixpoint reached");
      }

      if (1 == i) {
         util::Utility::logOutput("After ", i, " iteration of iterative procedures the formula has ", clausesStart, " clauses and ", literalsStart, " literals");
      }
      else {
         util::Utility::logOutput("After ", i, " iterations of iterative procedures the formula has ", clausesStart, " clauses and ", literalsStart, " literals");
      }

      auto timeTaken = util::Utility::stopTimer("mainProcessing");
      formula.addProcessingTime(timeTaken);
   }

   void ProcedureManager::addOneTimeProcedure(std::unique_ptr<procedures::Procedure>&& procedure) {
      util::Utility::logInfo("Adding one-time Procedure ", procedure->getName(), " of type ", cnf::equivalenceTypeToString(procedure->getEqType()));
      this->oneTimeProcedures.push_back(std::move(procedure));
   }

   void ProcedureManager::addIterativeProcedure(std::unique_ptr<procedures::Procedure>&& procedure) {
      util::Utility::logInfo("Adding iterative Procedure ", procedure->getName(), " of type ", cnf::equivalenceTypeToString(procedure->getEqType()));
      this->iterativeProcedures.push_back(std::move(procedure));
   }

}
