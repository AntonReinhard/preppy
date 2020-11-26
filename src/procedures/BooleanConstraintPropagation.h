#pragma once

#include "Procedures.h"

namespace preppy::procedures {

   class BooleanConstraintPropagation
      : public Procedure {
   public:

      BooleanConstraintPropagation() = default;

      bool apply(cnf::CNF& formula) override;

      // Returns all literals that can be derived from the formula using boolean constraint propagation
      // If the formula is detected to be unsatisfiable in this process {0} is returned
      cnf::Literals getBcp(cnf::CNF& formula) const;

      // Applies boolean constraint propagation on the formula with the single literal
      void applySingleLiteral(cnf::CNF& formula, const int literal) const;

      // Applies bcp on the formula with a signle literal and adds it to the formula to keep models intact
      void applySingleLiteralEq(cnf::CNF& formula, const int literal) const;

      // Applies boolean constraint propagation on the formula with all given literals
      void applyLiterals(cnf::CNF& formula, const cnf::Literals& literals) const;

      // Applies bcp on the formula with all given literals and adds them to the formula as givens to keep models intact
      void applyLiteralsEq(cnf::CNF& formula, const cnf::Literals& literals) const;

   protected:


   private:


   };

}
