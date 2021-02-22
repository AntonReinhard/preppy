/**
 * @file SelfSubsumingResolution.cpp
 * @author Anton Reinhard
 * @brief Self Subsuming Resolution Implementation
 * @version 0.5
 * @date 2021-02-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "SelfSubsumingResolution.h"

namespace preppy::procedures {

   SelfSubsumingResolution::SelfSubsumingResolution() 
      : Procedure("SelfSubsumingResolution", cnf::EQUIVALENCE_TYPE::NUMBER_EQUIVALENT) {

   }

   bool SelfSubsumingResolution::impl(cnf::CNF& formula) {
      
   }

}
