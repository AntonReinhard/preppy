/**
 * @file CNF.h
 * @author Anton Reinhard
 * @brief CNF Formula
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include "Clause.h"
#include "Model.h"
#include "definitions.h"

#include <vector>
#include <filesystem>
#include <initializer_list>
#include <tuple>

namespace preppy::cnf {

   /**
    * @brief Represents a CNF Formula. Derived from a vector of Clauses
    * 
    */
   class CNF {
   public:

      /**
       * @brief Creates empty CNF
       * 
       */
      CNF();

      /**
       * @brief Creates a CNF with given initializer list of Clauses
       * 
       * @param l initializer list of clauses
       */
      CNF(std::initializer_list<Clauses::value_type> l);

      /**
       * @brief Get a CNF with the same Metadata as this CNF, but no clauses
       * 
       * @return CNF The new CNF
       */
      CNF getMetadataCopy() const;

      /**
       * @brief Compresses this formula
       * 
       */
      void compress();

      /**
       * @brief Compresses a single literal from the original formula to this version, i.e. it looks up what the given literal in the original is called now
       * 
       * @param literal The literal to compress
       * @return int The compressed literal
       */
      int compress(int literal);

      /**
       * @brief Compresses a model of the original formula to a model of this version
       * 
       * @param model The Model to compress
       */
      void compress(cnf::Model& model);

      /**
       * @brief Decompresses this formula using the saved compressionInformation
       * 
       */
      void decompress();

      /**
       * @brief Decompresses a single literal from this version of the formula to the original
       * 
       * @param literal The Literal to decompress
       * @return int The decompressed literal
       */
      int decompress(int literal);

      /**
       * @brief Decompresses a model from this version of the formula to the original
       * 
       * @param model The Model to decompress
       */
      void decompress(cnf::Model& model);

      /**
       * @brief Renames the given variable to the new name, does not check if the new name was already used
       * 
       * @param variable The current variable's name
       * @param newName The name to rename the variable to
       */
      void renameVariable(const unsigned variable, const unsigned newName);

      /**
       * @brief Joins a second formula to this one, adding all clauses
       * 
       * @param formula The other formula to join with this
       */
      void joinFormula(const cnf::CNF& formula);

      /**
       * @brief To call when a literal was backpropagated and is now not part of the formula anymore, so it can be compressed again
       * 
       * @param literal The literal to set as having been backpropagated
       */
      void setLiteralBackpropagated(int literal);

      /**
       * @brief Generate a string containing this CNF Formula in .cnf format
       * 
       * @return std::string The formula in .cnf format as a string
       */
      std::string toString() const;

      /**
       * @brief Create this object from a given cnf file
       * 
       * @param filepath The path to the cnf file to read
       * @return bool True if successfull
       */
      bool readFromFile(const std::string& filepath);

      /**
       * @brief Writes this formula to a path
       * 
       * @param filepath A file path or directory to write to. 
       * @param force When set any existing file will be overriden
       * @return bool True on success
       * 
       * In case of a directory the original file name + out.cnf will be used. In case of a file the exact path will be used
       */
      bool writeToFile(const std::string& filepath, const bool force = false);

      /**
       * @brief Returns the amount of variables in the formula
       * 
       * @return unsigned The number of distinct used variables in this formula
       */
      unsigned getVariables();

      /**
       * @brief Returns the maximum variable number in the formula. 
       * Will usually be the same as getVariables but can differ in improperly saved cnf files or during modifications to the formula.
       * 
       * @return unsigned The maximum variable in the formula
       */
      unsigned getMaxVariable();

      /**
       * @brief Returns the number of clauses in the formula
       * 
       * @return unsigned The number of clauses in the formula
       */
      unsigned getClauses() const;

      /**
       * @brief Returns the number of total literals in the formula
       * 
       * @return unsigned The number of literals in the formula, counting duplicates
       */
      unsigned getLiterals() const;

      /**
       * @brief Checks whether there are any variables that don't occur in the formula but are smaller than the biggest variable
       * 
       * @return bool True if maxVariable == Variables
       */
      bool isCompressed();

      /**
       * @brief Sets the dirty bits to true.
       * Call this when changing the formula from the outside.
       * 
       */
      void setDirtyBitsTrue();

      /**
       * @brief Counts how often a variable appears in the formula and returns a vector, where the nth place is how often n appears
       * 
       * @return std::vector<unsigned> The vector containing variable occurrence counts
       */
      std::vector<unsigned> countVariables();

#pragma region vectorfunctions

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

#pragma endregion vectorfunctions

   protected:


   private:

      /**
       * @brief The clauses in this formula. Exposed using the standard vector functions
       * 
       */
      Clauses clauses;

      /**
       * @brief Name of the formula
       * 
       */
      std::string name;

      /**
       * @brief The original path the formula was read from
       * 
       */
      std::filesystem::path source;

      /**
       * @brief Number of variables in this formula, stored so it does not need to be recalculated every time it is asked
       * 
       */
      unsigned variables;

      /**
       * @brief Dirty bit for variables count, if true "variables" might be wrong
       * 
       */
      bool variablesDirtyBit = true;

      /**
       * @brief The maximum variable in this formula, stored so it does not need to be recalculated every time it is asked
       * 
       */
      unsigned maxVariable;

      /**
       * @brief Dirty bit for the maxVariable, if true "maVariable" might be wrong
       * 
       */
      bool maxVariableDirtyBit = true;

      /**
       * @brief Compression information for compressing original models and decompressing models of this to the original
       * the first number represents the original number, the second the new; the boolean saves if the replaced variable was assigned true or false
       * 
       */
      std::vector<std::tuple<unsigned, unsigned, bool>> compressionInformation;
   };

}
