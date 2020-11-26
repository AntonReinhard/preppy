#pragma once

#include "definitions.h"

#include <string>
#include <vector>
#include <initializer_list>

namespace preppy::cnf {

   /*
    * Representing a Clause, consisting of literals represented by numbers. Negative number means the literal is negated.
    */
   class Clause {
   public:

      // Construct empty Clause
      Clause() = default;

      // Constructs clause with given literals
      Clause(std::initializer_list<Literals::value_type> l);

      // Construct this Clause from a line in a cnf file
      explicit Clause(const std::string& line);

      // Sets the given literal to true, removes it from the clause if it's negated
      // Returns true if the clause is satisfied afterwards
      bool setLiteral(const int literal);

      // Returns this clause when the given literals are applied as facts to it
      cnf::Clause getPartialClause(const cnf::Literals& literals) const;

      // Renames the literals in the clause from old to new
      void renameVariable(const unsigned oldVar, const unsigned newVar);

      // Get this clause as a human readable string
      std::string toString() const;

      // Get a string representing this clause in cnf format
      std::string toCNFLine() const;

      // Returns the maximum occuring variable in this clause
      unsigned getMaxVariable() const;

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

   protected:


   private:

      // The literals in this Clause
      Literals literals;

   };

}
