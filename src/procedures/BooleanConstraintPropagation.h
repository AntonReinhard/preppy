#pragma once

#include "Procedures.h"

namespace preppy::procedures {

   // A ordered set of literals for quick lookup
   typedef std::vector<int> literals;

   class BooleanConstraintPropagation
      : public Procedure {
   public:

      BooleanConstraintPropagation() = default;

      bool apply(cnf::CNF& formula) override;

      // Applies boolean constraint propagation on the formula with the single literal
      void applySingleLiteral(cnf::CNF& formula, const int literal);

      // Applies bcp on the formula with a signle literal and adds it to the formula to keep models intact
      void applySingleLiteralEq(cnf::CNF& formula, const int literal);

      // Applies boolean constraint propagation on the formula with all given literals
      void applyLiterals(cnf::CNF& formula, const literals& literals);

      // Applies bcp on the formula with all given literals and adds them to the formula as givens to keep models intact
      void applyLiteralsEq(cnf::CNF& formula, const literals& literals);

   protected:


   private:


   };

}
