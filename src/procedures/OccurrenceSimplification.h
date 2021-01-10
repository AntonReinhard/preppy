/**
 * @file OccurrenceSimplification.h
 * @author Anton Reinhard
 * @brief Occurrence Simplification Procedure header
 * @version 0.1
 * @date 2021-01-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Procedure.h"

namespace preppy::procedures {

   /**
    * @brief Occurrence Simplification Algorithm
    * 
    * @details For the original algorithm see: https://www.cril.univ-artois.fr/KC/documents/lagniez-marquis-aaai14.pdf
    * Application of this procedure will result in an equivalent output fourmla
    */
   class OccurrenceSimplification
      : public preppy::procedures::Procedure {
   public:

      /**
       * @brief Construct a new Occurrence Simplification procedure
       * 
       */
      OccurrenceSimplification();

   protected:

      /**
       * @brief Apply the Occurrence Simplification procedure to a formula
       * 
       * @param formula The formula to apply to
       * @return bool True on success
       */
      bool impl(cnf::CNF& formula) override;

   private:

      /**
       * @brief Get the Literals in the given formula sorted by their number of occurrences in the formula
       * 
       * @param formula The formula to get the literals from
       * @return cnf::Literals The literals, sorted by their number of occurrence. Most common to least common
       */
      cnf::Literals getIncidenceSortedLiterals(cnf::CNF& formula) const;

   };

}
