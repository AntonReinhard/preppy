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

   Clause Clause::resolve(const Clause& other, int var) const {
      // make sure var is always positive
      var = std::abs(var);
      if (0 == var) {
         // if no var is given, find a variable to resolve on
         int foundPairs;
         for (const int& v : *this) {
            for (const int& vOther : other) {
               if (v == -vOther) {
                  if (foundPairs > 0) {
                     // if there's more than one pair to resolve on the resolvent will be a tautology anyways
                     return {};
                  }
                  var = std::abs(v);
                  foundPairs++;
                  break;
               }
            }
         }
         if (0 == foundPairs) {
            // can't resolve
            return {0};
         }
      }

      // we'll just assume that if a var was given that it's okay to resolve on
      // create new clause adding all of the given clauses' variables except for var
      Clause resolvent;
      resolvent.reserve(this->size() + other.size());
      for (const int& v : *this) {
         if (std::abs(v) == var) {
            continue;
         }
         resolvent.push_back(v);
      }
      for (const int& v : other) {
         if (std::abs(v) == var) {
            continue;
         }
         resolvent.push_back(v);
      }

      return resolvent;
   }
}
