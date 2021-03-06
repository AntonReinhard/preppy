/**
 * @file Vivification.h
 * @author Anton Reinhard
 * @brief Vivification Procedure header
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Procedure.h"

namespace preppy::procedures {

   /**
    * @brief Class implementing Vivification as a procedure
    * 
    * @details For the original algorithm see: https://www.cril.univ-artois.fr/KC/documents/lagniez-marquis-aaai14.pdf
    * Application of this procedure will result in an equivalent output fourmla
    */
   class Vivification
      : public preppy::procedures::Procedure {
   public:

      /**
       * @brief Construct a new Vivification procedure
       * 
       */
      Vivification();

   protected:

      /**
       * @brief Apply the Vivification procedure to a formula
       * 
       * @param formula The formula to apply to
       * @return bool True on success
       */
      bool impl(cnf::CNF& formula) override;

   private:

   };

}
