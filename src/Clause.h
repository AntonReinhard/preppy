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
       * @brief Sets the given literal to true, removes it from the clause if it's negated
       * 
       * @param literal The literal to set
       * @return bool True if the clause is satisfied afterwards
       */
      bool setLiteral(const int literal);

      /**
       * @brief Returns this clause when the given literals are applied as facts to it
       * 
       * @param literals The literals to apply
       * @return cnf::Clause The resulting clause, {0} if satisfied
       */
      cnf::Clause getPartialClause(const cnf::Literals& literals) const;

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

      // exposing vector member functions for literals
      // access
      Literals::iterator begin() noexcept;
      Literals::const_iterator begin() const noexcept;
      Literals::iterator end() noexcept;
      Literals::const_iterator end() const noexcept;
      Literals::reference front();
      Literals::const_reference front() const;
      Literals::reference back();
      Literals::const_reference back() const;
      Literals::reference operator[](Literals::size_type n);
      Literals::const_reference operator[](Literals::size_type n) const;
      Literals::reference at(const Literals::size_type n);
      Literals::const_reference at(const Literals::size_type n) const;

      // muting
      void push_back(const Literals::value_type& val);
      void push_back(Literals::value_type&& val);
      template <class... Args>
      Literals::iterator emplace(Literals::const_iterator position, Args&&... args) {
         return this->literals.emplace(position, args...);
      }
      template <class... Args>
      void emplace_back(Args&&... args) {
         this->literals.emplace_back(args...);
      }
      void pop_back();
      Literals::iterator erase(Literals::const_iterator position);
      Literals::iterator erase(Literals::const_iterator first, Literals::const_iterator last);
      Literals::iterator insert(Literals::const_iterator position, const Literals::value_type& val);
      Literals::iterator insert(Literals::const_iterator position, Literals::size_type n, const Literals::value_type& val);
      template <class InputIterator>
      Literals::iterator insert(Literals::const_iterator position, InputIterator first, InputIterator last) {
         return this->literals.insert(position, first, last);
      }
      Literals::iterator insert(Literals::const_iterator position, Literals::value_type&& val);
      Literals::iterator insert(Literals::const_iterator position, std::initializer_list<Literals::value_type> il);
      void clear() noexcept;

      // size
      void reserve(Literals::size_type n);
      Literals::size_type capacity() const noexcept;
      void resize(Literals::size_type n);
      void resize(Literals::size_type n, const Literals::value_type& val);
      Literals::size_type size() const noexcept;
      Literals::size_type max_size() const noexcept;

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
