/**
 * @file Procedures.h
 * @author Anton Reinhard
 * @brief Procedures abstract definition
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "../CNF.h"

namespace preppy::procedures {

   /**
    * @brief Interface of every procedure
    * 
    */
   class Procedure {
   public:

      /**
       * @brief Destroy a procedure object
       * 
       */
      virtual ~Procedure() = default;

      /**
       * @brief Takes a reference to the formula and works on it
       * 
       * @param formula The formula to apply the procedure to
       * @return bool True on success, False if some error occurred
       */
      virtual bool apply(cnf::CNF& formula) = 0;

   protected:


   private:


   };

}
