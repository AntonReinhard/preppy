#pragma once
#include "Clause.h"
#include "Model.h"
#include "definitions.h"

#include <vector>
#include <initializer_list>
#include <tuple>

namespace preppy::cnf {

   /*
    * Represents a CNF Formula. Derived from a vector of Clauses
    */
   class CNF {
   public:

      // Creates empty CNF
      CNF();

      // Creates a CNF with given initializer list of Clauses
      CNF(std::initializer_list<Clauses::value_type> l);

      // Compresses this formula
      void compress();

      // Compresses a single literal from the original formula to this version
      int compress(int literal);

      // Compresses a model of the original formula to a model of this version
      void compress(cnf::Model& model);

      // Decompresses this formula using the saved compressionInformation
      void decompress();

      // Decompresses a single literal from this version of the formula to the original
      int decompress(int literal);

      // Decompresses a model from this version of the formula to the original
      void decompress(cnf::Model& model);

      // Renames the given variable to the new name, does not check if the new name was already used
      void renameVariable(const unsigned variable, const unsigned newName);

      // Joins a second formula to this one, adding all clauses
      void joinFormula(const cnf::CNF& formula);

      // To call when a literal was backpropagated and is now not part of the formula anymore, so it can be compressed again
      void setLiteralBackpropagated(int literal);

      // Generate a string containing this CNF Formula in .cnf format
      std::string toString() const;

      // Read a cnf file, return success
      bool readFromFile(const std::string& filepath);

      // Writes this formula to the given filepath, returns true on success
      // if force is set to true it will force override any existing file
      bool writeToFile(const std::string& filepath, const bool force = false);

      // Returns the amount of variables in the formula
      unsigned getVariables();

      // Returns the maximum variable number in the formula. Should usually be the same as getVariables but can differ in improperly saved cnf files
      unsigned getMaxVariable();

      // Returns the number of clauses in the formula
      unsigned getClauses() const;

      // Returns true if there are any variables that don't occur in the formula but are smaller than the biggest variable
      bool isCompressed();

      // when changing the formula from the outside, call this to set the dirty bits to true again
      void setDirtyBitsTrue();

      // Counts how often a variable appears in the formula and returns a vector, where the nth place is how often n appears
      std::vector<unsigned> countVariables();


      // exposing vector member functions for clauses
      // access
      Clauses::iterator begin() noexcept;
      Clauses::const_iterator begin() const noexcept;
      Clauses::iterator end() noexcept;
      Clauses::const_iterator end() const noexcept;
      Clauses::reference front();
      Clauses::const_reference front() const;
      Clauses::reference back();
      Clauses::const_reference back() const;
      Clauses::reference operator[](Clauses::size_type n);
      Clauses::const_reference operator[](Clauses::size_type n) const;
      Clauses::reference at(const Clauses::size_type n);
      Clauses::const_reference at(const Clauses::size_type n) const;

      // muting
      void push_back(const Clauses::value_type& val);
      void push_back(Clauses::value_type&& val);
      template <class... Args>
      Clauses::iterator emplace(Clauses::const_iterator position, Args&&... args) {
         return this->clauses.emplace(position, args...);
      }
      template <class... Args>
      void emplace_back(Args&&... args) {
         this->clauses.emplace_back(args...);
      }
      void pop_back();
      Clauses::iterator erase(Clauses::const_iterator position);
      Clauses::iterator erase(Clauses::const_iterator first, Clauses::const_iterator last);
      Clauses::iterator insert(Clauses::const_iterator position, const Clauses::value_type& val);
      Clauses::iterator insert(Clauses::const_iterator position, Clauses::size_type n, const Clauses::value_type& val);
      template <class InputIterator>
      Clauses::iterator insert(Clauses::const_iterator position, InputIterator first, InputIterator last) {
         return this->clauses.insert(position, first, last);
      }
      Clauses::iterator insert(Clauses::const_iterator position, Clauses::value_type&& val);
      Clauses::iterator insert(Clauses::const_iterator position, std::initializer_list<Clauses::value_type> il);
      void clear() noexcept;

      // size
      void reserve(Clauses::size_type n);
      Clauses::size_type capacity() const noexcept;
      void resize(Clauses::size_type n);
      void resize(Clauses::size_type n, const Clauses::value_type& val);
      Clauses::size_type size() const noexcept;
      Clauses::size_type max_size() const noexcept;

   protected:


   private:

      // the clauses in this formula
      Clauses clauses;

      // name of the formula, currently the file path it was read from
      std::string name;

      // number of variables in this formula, stored so it does not need to be recalculated every time
      unsigned variables;

      // dirty bit for variables count, if true "variables" might be wrong
      bool variablesDirtyBit = true;

      // the maximum variable in this formula, stored so it does not need to be recalculated every time
      unsigned maxVariable;

      // dirty bit for the maxVariable, if true "maVariable" might be wrong
      bool maxVariableDirtyBit = true;

      // compression information for compressing original models and decompressing models of this to the original
      // the first number represents the original number, the second the new; the boolean saves if the replaced variable was assigned true or false
      std::vector<std::tuple<unsigned, unsigned, bool>> compressionInformation;
   };

}
