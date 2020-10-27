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

      // Applies boolean constraint propagation on the formula with all given literals
      void applyLiterals(cnf::CNF& formula, const literals& literals);

   protected:


   private:


   };

}
