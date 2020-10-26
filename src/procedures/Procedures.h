#pragma once

#include "../CNF.h"

namespace preppy::procedures {

   class Procedure {
   public:

      virtual ~Procedure() = default;

      // Takes a reference to the formula and works on it
      virtual bool apply(cnf::CNF& formula) = 0;

   protected:


   private:


   };

}
