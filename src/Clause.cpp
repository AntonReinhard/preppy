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

   bool Clause::setLiteral(const int literal) {
      for (const auto& lit : *this) {
         if (lit == literal) {
            return true;
         }
      }
      this->erase(
         std::remove_if(this->begin(), this->end(),
            [&](const int lit) {
               return lit == -literal;
            }
         ),
         this->end()
      );
      return false;
   }

   cnf::Clause Clause::getPartialClause(const cnf::Literals& literals) const {
      cnf::Clause result(*this);
      for (const auto& lit : literals) {
         if (result.setLiteral(lit)) {
            return {0};
         }
      }
      return result;
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

#pragma region vectorfunctions

   Literals::iterator Clause::begin() noexcept {
      return this->literals.begin();
   }

   Literals::const_iterator Clause::begin() const noexcept {
      return this->literals.begin();
   }

   Literals::iterator Clause::end() noexcept {
      return this->literals.end();
   }

   Literals::const_iterator Clause::end() const noexcept {
      return this->literals.end();
   }
   
   Literals::reference Clause::front() {
      return this->literals.front();
   }

   Literals::const_reference Clause::front() const {
      return this->literals.front();
   }

   Literals::reference Clause::back() {
      return this->literals.back();
   }
   
   Literals::const_reference Clause::back() const {
      return this->literals.back();
   }
   
   Literals::reference Clause::operator[](Literals::size_type n) {
      return this->literals[n];
   }

   Literals::const_reference Clause::operator[](Literals::size_type n) const {
      return this->literals[n];
   }
   
   Literals::reference Clause::at(const Literals::size_type n) {
      return this->literals.at(n);
   }

   Literals::const_reference Clause::at(const Literals::size_type n) const {
      return this->literals.at(n);
   }

   void Clause::push_back(const Literals::value_type& val) {
      this->literals.push_back(val);
   }

   void Clause::push_back(Literals::value_type&& val) {
      this->literals.push_back(val);
   }

   void Clause::pop_back() {
      this->literals.pop_back();
   }

   Literals::iterator Clause::erase(Literals::const_iterator position) {
      return this->literals.erase(position);
   }

   Literals::iterator Clause::erase(Literals::const_iterator first, Literals::const_iterator last) {
      return this->literals.erase(first, last);
   }

   Literals::iterator Clause::insert(Literals::const_iterator position, const Literals::value_type& val) {
      return this->literals.insert(position, val);
   }

   Literals::iterator Clause::insert(Literals::const_iterator position, Literals::size_type n, const Literals::value_type& val) {
      return this->literals.insert(position, n, val);
   }

   Literals::iterator Clause::insert(Literals::const_iterator position, Literals::value_type&& val) {
      return this->literals.insert(position, val);
   }

   Literals::iterator Clause::insert(Literals::const_iterator position, std::initializer_list<Literals::value_type> il) {
      return this->literals.insert(position, il);
   }

   void Clause::clear() noexcept {
      this->literals.clear();
   }

   void Clause::reserve(Literals::size_type n) {
      this->literals.reserve(n);
   }

   Literals::size_type Clause::capacity() const noexcept {
      return this->literals.capacity();
   }

   void Clause::resize(Literals::size_type n) {
      this->literals.resize(n);
   }

   void Clause::resize(Literals::size_type n, const Literals::value_type& val) {
      this->literals.resize(n, val);
   }

   Literals::size_type Clause::size() const noexcept {
      return this->literals.size();
   }

   Literals::size_type Clause::max_size() const noexcept {
      return this->literals.max_size();
   }

#pragma endregion vectorfunctions

}
