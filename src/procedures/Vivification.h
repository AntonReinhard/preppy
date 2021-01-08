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
#include "Procedures.h"

namespace preppy::procedures {

   /**
    * @brief Class implementing Vivification as a procedure
    * 
    */
   class Vivification
      : public preppy::procedures::Procedure {

   public:
      /**
       * @brief Apply the Vivification procedure to a formula
       * 
       * @param formula The formula to apply to
       * @return bool True on success
       */
      bool apply(cnf::CNF &formula) override;

   protected:


   private:

   };

} // namespace preppy::procedures
