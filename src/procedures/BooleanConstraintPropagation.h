/**
 * @file BooleanConstraintPropagation.h
 * @author Anton Reinhard
 * @brief Boolean Constraint Propagation procedure header
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Procedure.h"

#include <vector>

namespace preppy::procedures {

   /**
    * @brief Class implementing Boolean Constraint Propagation (BCP) as a procedure
    * 
    */
   class BooleanConstraintPropagation
      : public preppy::procedures::Procedure {
   public:

      /**
       * @brief Construct a new Boolean Constraint Propagation procedure
       * 
       */
      BooleanConstraintPropagation();

      /**
       * @brief Returns all literals that can be derived from the formula using boolean constraint propagation \n If the formula is detected to be unsatisfiable in this process {0} is returned
       * 
       * @param formula The formula to get the propagated literals from
       * @return cnf::Literals The derived literals, or {0} if the formula became unsatisfiable
       */
      cnf::Literals getBcp(cnf::CNF& formula);

      /**
       * @brief Applies boolean constraint propagation on the formula with a single literal
       * 
       * @param formula The formula to apply the literal to
       * @param literal The literal to apply
       */
      void applySingleLiteral(cnf::CNF& formula, const int literal) const;

      /**
       * @brief Applies BCP to the formula with a single literal and adds it to the formula to keep models intact
       * 
       * @param formula The formula to apply the literal to
       * @param literal The literal to apply
       */
      void applySingleLiteralEq(cnf::CNF& formula, const int literal) const;

      /**
       * @brief Applies boolean constraint propagation on the formula with all given literals
       * 
       * @param formula The formula to apply the literals to
       * @param literals The literals to apply
       */
      void applyLiterals(cnf::CNF& formula, const cnf::Literals& literals) const;

      /**
       * @brief Applies bcp on the formula with all given literals and adds them to the formula as givens to keep models intact
       * 
       * @param formula The formula to apply the literals to
       * @param literals The literals to apply
       */
      void applyLiteralsEq(cnf::CNF& formula, const cnf::Literals& literals) const;

   protected:

      /**
       * @brief Apply the BCP procedure to a formula
       * 
       * @param formula The formula to apply to
       * @return bool True on success
       */
      bool impl(cnf::CNF& formula) override;

   private:

   };

}
