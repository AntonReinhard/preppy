/**
 * @file Clause.cpp
 * @author Anton Reinhard
 * @brief Clause implementation
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Clause.h"
#include "Utility.h"
#include "definitions.h"

#include <string>
#include <sstream>
#include <vector>
#include <initializer_list>
#include <algorithm>

namespace preppy::cnf {

   Clause::Clause(std::initializer_list<preppy::cnf::Literals::value_type> l) 
      : literals(l) {
   }

   Clause::Clause(const std::string& line) {
      std::istringstream ss(line);
      
      while (ss.rdbuf()->in_avail() != 0) {  //while the buffer is not empty
         int literal;
         ss >> literal;
         if (0 == literal) {
            break;
         }
         this->push_back(literal);
      }
      if (ss.rdbuf()->in_avail() != 0) {
         util::Utility::logWarning("Leftover characters in line after parsing clause: ", ss.str());
      }
   }

   bool Clause::containsLiteral(const int literal) const {
      return std::find(this->begin(), this->end(), literal) != this->end();
   }

   bool Clause::setLiteral(const int literal) {
      // if the literal is found -> the clause is satisfied
      if (std::find(this->begin(), this->end(), literal) != this->end()) {
         this->literals = {};
         return true;
      }

      // if the negated literal is found -> remove that literal
      auto litPos = std::find(this->begin(), this->end(), -literal);
      if (litPos != this->end()) {
         this->erase(litPos);
      }

      // if the last literal was just set to false this clause is now unsatisfied
      if (this->size() == 0) {
         this->literals = {0};
      }

      return false;
   }

   cnf::Clause Clause::getPartialClause(const cnf::Literals& literals) const {
      cnf::Clause result(*this);
      for (const auto& lit : literals) {
         if (result.setLiteral(lit)) {
            return result;
         }
      }
      return result;
   }

   bool Clause::isSatisfied(const cnf::Literals& literals) const {
      for (const auto& lit : this->literals) {
         if (std::find(literals.begin(), literals.end(), lit) != literals.end()) {
            return true;
         }
      }
      return false;
   }

   cnf::Clause Clause::getComplement() const {
      cnf::Clause complementaryNewClause;
      complementaryNewClause.reserve(this->size());
      for (const auto &lit : this->literals) {
         complementaryNewClause.push_back(-lit);
      }
      return complementaryNewClause;
   }

   void Clause::renameVariable(const unsigned oldVar, const unsigned newVar) {
      for (auto& lit : *this) {
         if (std::abs(lit) == oldVar) {
            lit = (lit < 0) ? -newVar : newVar;
         }
      }
   }

   std::string Clause::toString() const {
      std::ostringstream ss;
      bool first = true;
      for (auto literal : *this) {
         if (!first) {
            ss << " ";
         }
         first = false;
         ss << literal;
      }
      return ss.str();
   }

   std::string Clause::toCNFLine() const {
      std::ostringstream ss;
      for (auto literal : *this) {
         ss << literal << " ";
      }
      ss << "0\n";
      return ss.str();
   }

   unsigned Clause::getMaxVariable() const {
      unsigned max = 0;
      for (const auto& v : *this) {
         if (std::abs(v) > max) {
            max = std::abs(v);
         }
      }
      return max;
   }

}
