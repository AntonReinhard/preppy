/**
 * @file clasp.h
 * @author Anton Reinhard
 * @brief clasp solver header
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Solver.h"
#include "../Utility.h"
#include "../Model.h"

#include <chrono>

namespace preppy::solvers {

   /**
    * @brief Implementation of the clasp solver
    * 
    */
   class clasp
      : public Solver {
   public:

      /**
       * @brief Construct a new clasp solver
       * 
       * @param timeout Timeout for getting a solution
       */
      clasp(const util::clock::duration& timeout);

      /**
       * @brief Destroy the clasp object
       * 
       */
      ~clasp() = default;

      /**
       * @brief Checks whether a formula is satisfiable using clasp
       * 
       * @param formula The formula to check
       * @return bool True if satisfiable, False if unsatisfiable, error or timeout
       */
      bool isSatisfiable(cnf::CNF& formula) override;
   
      /**
       * @brief Gets a model for the formula using clasp
       * 
       * @param formula The formula to get a model for
       * @return cnf::Model A single model for the formula, or empty if the formula is unsatisfiable, or on timeout or error
       */
      cnf::Model getModel(cnf::CNF& formula) override;

   protected:


   private:

      /**
       * @brief Timeout for getting a solution
       * 
       */
      util::clock::duration timeout;

   };

}