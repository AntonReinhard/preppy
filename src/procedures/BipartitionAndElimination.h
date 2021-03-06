/**
 * @file BipartitionAndElimination.h
 * @author Anton Reinhard
 * @brief Bipartition and Elimination Procedure header
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Procedure.h"
#include "../CNF.h"
#include "../definitions.h"

#include <vector>
#include <tuple>

namespace preppy::procedures {

   /**
    * @brief Class implementing Bipartition and Elimination as a procedure
    * 
    * @details For the original algorithm see: https://www.cril.univ-artois.fr/kc/documents/definability4mc-fullproof.pdf
    * Application of this procedure will result in an output formula that has the same number of models as the input
    */
   class BipartitionAndElimination
      : public Procedure {
   public:

      /**
       * @brief Construct a new Bipartition and Elimination procedure
       * 
       */
      BipartitionAndElimination();

      /**
       * @brief Returns a list of variables in the formula that are defined in terms of the other variables in the formula greedily calculated, so not necessarily all are found
       * 
       * @param formula The formula to get a bipartition from
       * @return cnf::Variables The variables defined in terms of the rest in the formula
       */
      
      cnf::Variables bipartition(const cnf::CNF& formula) const;

   protected:

      /**
       * @brief Apply the Bipartition and Elimination procedure to a formula
       * 
       * @param formula The formula to apply to
       * @return bool True on success
       */
      bool impl(cnf::CNF& formula) override;

      /**
       * @brief Eliminates the given variables from the formula
       * 
       * @param formula The formula to eliminate the variables from
       * @param variables The variables to eliminate, provided by the bipartition algorithm
       */
      void eliminate(cnf::CNF& formula, cnf::Variables variables) const;

      /**
       * @brief Tests whether variable x is defined in terms of variables in formula
       * 
       * @param x The variable to check for definedness
       * @param formula The formula to check on
       * @param variables The given variables that x can be defined by
       * @return bool True if x is defined in terms of variables, False if not or on timeout/error
       */
      bool isDefined(const unsigned x, const cnf::CNF& formula, const cnf::Variables& variables) const;

      /**
       * @brief Sorts the given variables such that the number of clauses that a variable appears in as negative
       * literal and number of clauses that it appears in as a positive literal multiplied sorted lowest to highest
       * 
       * @param formula The formula to count clauses in
       * @param variables The variables to sort
       */
      static void sort(const cnf::CNF& formula, cnf::Variables& variables);

   private:

      /**
       * @brief max#Res from the paper to decide whether a certain variable x will be retained
       * 
       */
      constexpr static int maxNumberRes = 500;

   };

}
