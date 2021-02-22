/**
 * @file SelfSubsumingResolution.h
 * @author Anton Reinhard
 * @brief Self Subsuming Resolution Procedure header
 * @version 0.5
 * @date 2021-02-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Procedure.h"

namespace preppy::procedures {

   /**
    * @brief Self Subsuming Resolution procedure definition
    * 
    */
   class SelfSubsumingResolution
      : public Procedure {
   public:

      /**
       * @brief Construct a new SelfSubsumingResolution object
       * 
       */
      SelfSubsumingResolution();

   protected:

      bool impl(cnf::CNF& formula) override;

   private:


   };

}
