/**
 * @file Procedures.cpp
 * @author Anton Reinhard
 * @brief Procedures implementation
 * @version 0.1
 * @date 2021-01-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Procedure.h"
#include "../Utility.h"

namespace preppy::procedures {

   Procedure::Procedure(const std::string& name, cnf::EQUIVALENCE_TYPE eqType) 
      : name(name)
      , eqType(eqType) {
      
   }

   bool Procedure::apply(cnf::CNF& formula) {
      this->setApplied(formula);

      util::Utility::logDebug("Applying Procedure ", this->getName(), " with Equivalence Type ", cnf::equivalenceTypeToString(this->getEqType()));
      util::Utility::startTimer(this->getName());
      
      bool success = this->impl(formula);

      util::Utility::logDebug("Procedure ", this->getName(), " finished after ", util::Utility::durationToString(util::Utility::stopTimer(this->getName())));

      return success;
   }

   std::string Procedure::getName() const {
      return this->name;
   }

   cnf::EQUIVALENCE_TYPE Procedure::getEqType() const {
      return this->eqType;
   }

   void Procedure::setApplied(cnf::CNF& formula) const {
      formula.addProcedure(this->name);
      formula.setEquivalence(this->eqType);
   }

}
