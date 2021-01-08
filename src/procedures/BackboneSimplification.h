/**
 * @file BackboneSimplification.h
 * @author Anton Reinhard
 * @brief Backbone Simplification header
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Procedures.h"
#include "../solvers/Solver.h"
#include "../definitions.h"

#include <set>
#include <memory>

namespace preppy::procedures {

   /**
    * @brief Class implementing Backbone Simplification as a procedure
    * 
    */
   class BackboneSimplification
      : public Procedure {
   public:

      /**
       * @brief Construct a new Backbone Simplification procedure
       * 
       * @param solver The solver to use for this
       */
      BackboneSimplification(std::shared_ptr<solvers::Solver> solver);

      /**
       * @brief Apply Backbone Simplification to a formula
       * 
       * @param formula The formula to apply to
       * @return bool True on success
       */
      bool apply(cnf::CNF& formula) override;

      /**
       * @brief Computes a backbone
       * 
       * @param formula The formula to get the backbone for 
       * @return cnf::Literals The literals of the backbone
       */
      cnf::Literals getBackbone(const cnf::CNF& formula) const;

   protected:

   
   private:

      /**
       * @brief The solver to use
       * 
       */
      std::shared_ptr<solvers::Solver> solver;

   };

}
