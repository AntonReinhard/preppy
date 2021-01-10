/**
 * @file Clause.h
 * @author Anton Reinhard
 * @brief Clause header
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "definitions.h"

#include <string>
#include <vector>
#include <initializer_list>

namespace preppy::cnf {

   /**
    * @brief Representing a Clause, consisting of literals represented by numbers. Negative number means the literal is negated.
    * A unsatisfied clause is represented by {0}
    * 
    */
   class Clause {
   public:

      /**
       * @brief Construct empty Clause
       * 
       */
      Clause() = default;

      /**
       * @brief Constructs clause with given literals
       * 
       * @param l An initializer list of literals
       */
      Clause(std::initializer_list<Literals::value_type> l);

      /**
       * @brief Construct this Clause from a line in a cnf file
       * 
       * @param line The line in the cnf file
       */
      explicit Clause(const std::string& line);

      /**
       * @brief Simple helper function to check whether this clause contains a given literal
       * 
       * @param literal The literal to check whether it's part of the clause
       * @return bool True if the literal is part of the clause
       */
      bool containsLiteral(int literal) const;

      /**
       * @brief Sets the given literal to true, removes it from the clause if it's negated
       * 
       * @param literal The literal to set
       * @return bool True if the clause is satisfied afterwards
       */
      bool setLiteral(int literal);

      /**
       * @brief Returns this clause when the given literals are applied as facts to it
       * 
       * @param literals The literals to apply
       * @return cnf::Clause The resulting clause, {0} if unsatisfied
       */
      cnf::Clause getPartialClause(const cnf::Literals& literals) const;

      /**
       * @brief Returns true if the clause is satisfied when setting the given literals
       * 
       * @param literals The literals to set
       * @return bool True iff the clause is satisfied with the given literals
       */
      bool isSatisfied(const cnf::Literals& literals) const;

      /**
       * @brief Gets the complementary clause of this clause, meaning every literal is negated
       * 
       * @return cnf::Clause The complementary clause
       */
      cnf::Clause getComplement() const;

      /**
       * @brief Renames the literals in the clause from old to new
       * 
       * @param oldVar Old/Current name of the variable
       * @param newVar New name to give to the variable
       */
      void renameVariable(const unsigned oldVar, const unsigned newVar);

      /**
       * @brief Get this clause as a human readable string
       * 
       * @return std::string The clause as readable string
       */
      std::string toString() const;

      /**
       * @brief Get a string representing this clause in cnf format
       * 
       * @return std::string The clause as cnf string
       */
      std::string toCNFLine() const;

      /**
       * @brief Get the maximum occurring variable in this clause
       * 
       * @return unsigned The maximum variable
       */
      unsigned getMaxVariable() const;

#pragma region vectorfunctions
   //Put function definitions here for compiler optimizations and inlining

   Literals::iterator begin() noexcept {
      return this->literals.begin();
   }

   Literals::const_iterator begin() const noexcept {
      return this->literals.begin();
   }

   Literals::iterator end() noexcept {
      return this->literals.end();
   }

   Literals::const_iterator end() const noexcept {
      return this->literals.end();
   }
   
   Literals::reference front() {
      return this->literals.front();
   }

   Literals::const_reference front() const {
      return this->literals.front();
   }

   Literals::reference back() {
      return this->literals.back();
   }
   
   Literals::const_reference back() const {
      return this->literals.back();
   }
   
   Literals::reference operator[](Literals::size_type n) {
      return this->literals[n];
   }

   Literals::const_reference operator[](Literals::size_type n) const {
      return this->literals[n];
   }
   
   Literals::reference at(const Literals::size_type n) {
      return this->literals.at(n);
   }

   Literals::const_reference at(const Literals::size_type n) const {
      return this->literals.at(n);
   }

   void push_back(const Literals::value_type& val) {
      this->literals.push_back(val);
   }

   void push_back(Literals::value_type&& val) {
      this->literals.push_back(val);
   }

   void pop_back() {
      this->literals.pop_back();
   }

   Literals::iterator erase(Literals::const_iterator position) {
      return this->literals.erase(position);
   }

   Literals::iterator erase(Literals::const_iterator first, Literals::const_iterator last) {
      return this->literals.erase(first, last);
   }

   Literals::iterator insert(Literals::const_iterator position, const Literals::value_type& val) {
      return this->literals.insert(position, val);
   }

   Literals::iterator insert(Literals::const_iterator position, Literals::size_type n, const Literals::value_type& val) {
      return this->literals.insert(position, n, val);
   }

   Literals::iterator insert(Literals::const_iterator position, Literals::value_type&& val) {
      return this->literals.insert(position, val);
   }

   Literals::iterator insert(Literals::const_iterator position, std::initializer_list<Literals::value_type> il) {
      return this->literals.insert(position, il);
   }

   void clear() noexcept {
      this->literals.clear();
   }

   void reserve(Literals::size_type n) {
      this->literals.reserve(n);
   }

   Literals::size_type capacity() const noexcept {
      return this->literals.capacity();
   }

   void resize(Literals::size_type n) {
      this->literals.resize(n);
   }

   void resize(Literals::size_type n, const Literals::value_type& val) {
      this->literals.resize(n, val);
   }

   Literals::size_type size() const noexcept {
      return this->literals.size();
   }

   Literals::size_type max_size() const noexcept {
      return this->literals.max_size();
   }

#pragma endregion vectorfunctions

   protected:


   private:

      /**
       * @brief The literals in this Clause \n Exposed through the standard vector functions
       * 
       */
      Literals literals;

   };

}
