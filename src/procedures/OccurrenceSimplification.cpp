/**
 * @file OccurrenceSimplification.cpp
 * @author Anton Reinhard
 * @brief Occurrence Simplification Procedure implementation
 * @version 0.1
 * @date 2021-01-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "OccurrenceSimplification.h"

namespace preppy::procedures {

   OccurrenceSimplification::OccurrenceSimplification()
      : Procedure("Occurrence Simplification", cnf::EQUIVALENCE_TYPE::EQUIVALENT) {

   }

   bool OccurrenceSimplification::impl(cnf::CNF& formula) {

      return false;
   }

}
