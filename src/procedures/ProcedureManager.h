/**
 * @file ProcedureManager.h
 * @author Anton Reinhard
 * @brief Procedure Manager header
 * @version 0.1
 * @date 2021-01-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Procedure.h"

namespace preppy::procedures {

   /**
    * @brief A Manager class to collect procedures that will be applied to a formula
    * 
    */
   class ProcedureManager {
   public:

      /**
       * @brief Construct a new empty Procedure Manager
       * 
       * @param iterations The number of iterations to execute the iterative procedures
       * @param minLiteralAndClauseReduction The minimum literals and clauses that have to be reduced in one iteration 
       * for it to not be considered a fixpoint. If in any iteration both number of reduced literals and clauses are
       * undershot the loop is stopped. First value in the pair is literal reduction, second is clause reduction
       */
      ProcedureManager(unsigned iterations, const std::pair<unsigned, unsigned>& minLiteralAndClauseReduction = {1, 1});

      /**
       * @brief Applies the procedures that were added to the manager
       * 
       * @param formula 
       */
      void apply(cnf::CNF& formula);

      /**
       * @brief Adds a procedure to the list of procedures that will be executed once before the iteratively applied procedures.
       * The order in which procedures are added is the order in which procedures will be applied when calling apply()
       * 
       * @param procedure The procedure to add
       */
      void addOneTimeProcedure(std::unique_ptr<procedures::Procedure>&& procedure);

      /**
       * @brief Adds a procedure to the list of procedures that will be executed in a loop for a set number of iterations or
       * until a fixpoint is reached. The order in which procedures are added is the order in which procedures
       * will be applied when calling apply()
       * 
       * @param procedure The procedure to add
       */
      void addIterativeProcedure(std::unique_ptr<procedures::Procedure>&& procedure);

   protected:


   private:

      /**
       * @brief Procedures that will be applied once at the start, in the order they are added
       * 
       */
      std::vector<std::unique_ptr<procedures::Procedure>> oneTimeProcedures;

      /**
       * @brief Procedures that will be applied in a loop for however many iterations are set.
       * The order of application is the order in which the procedures were added.
       * If none of the procedures had an effect on the formula in an entire iteration, the loop
       * is stopped, regardless of how many iterations would be left.
       * 
       */
      std::vector<std::unique_ptr<procedures::Procedure>> iterativeProcedures;

      /**
       * @brief How often to execute the iterative procedures. If set to zero procedures will be executed
       * until a fixpoint is reached
       * 
       */
      unsigned iterations;

      /**
       * @brief Minimum reduction of literals for the state to not be considered a fixpoint.
       * Default 1
       * @note 0 means that a fixpoint is not reached, even when 0 literals were removed
       */
      unsigned minLiteralReduction;

      /**
       * @brief Minimum reduction of clauses for the state to not be considered a fixpoint.
       * Default 1
       * @note 0 means that a fixpoint is not reached, even when 0 clauses were removed
       */
      unsigned minClauseReduction;

   };

}
