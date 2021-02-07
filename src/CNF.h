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
#include <set>
#include <filesystem>
#include <initializer_list>
#include <tuple>
#include <algorithm>

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
       * @brief Copy a CNF
       * 
       * @param other The CNF to copy
       */
      CNF(const cnf::CNF& other);

      /**
       * @brief Copy other to this
       * 
       * @param other The CNF to copy
       * @return CNF& A reference to this
       */
      CNF& operator=(const cnf::CNF& other);

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
       * @brief Helper function for writing the File
       * 
       * @return std::string The header of the output cnf file
       */
      std::string getFileHeader();

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

      /**
       * @brief Add the name of a procedure that was used on this CNF
       * 
       * @param procedure The procedure to add
       */
      void addProcedure(const std::string& procedure);

      /**
       * @brief Set the Equivalence Type of this formula. Will only set if the Equivalence 
       * to set is less strong than the one already in place
       * 
       * @param eqType The equivalence to set
       */
      void setEquivalence(cnf::EQUIVALENCE_TYPE eqType);

      /**
       * @brief Adds a given duration to this CNFs total processing time
       * 
       * @param duration The duration to add
       */
      void addProcessingTime(const util::clock::duration& duration);

      /**
       * @brief Get a reference to this formula's watched literals
       * 
       * @return WatchedLiteralsT& The reference to the watched literals
       */
      WatchedLiteralsT& getWatchedLiterals();

#pragma region vectorfunctions
   //Put function definitions here for compiler optimizations and inlining

   Clauses::iterator begin() noexcept {
      return this->clauses.begin();
   }

   Clauses::const_iterator begin() const noexcept {
      return this->clauses.begin();
   }

   Clauses::iterator end() noexcept {
      return this->clauses.end();
   }

   Clauses::const_iterator end() const noexcept {
      return this->clauses.end();
   }
   
   Clauses::reference front() {
      return this->clauses.front();
   }

   Clauses::const_reference front() const {
      return this->clauses.front();
   }

   Clauses::reference back() {
      return this->clauses.back();
   }
   
   Clauses::const_reference back() const {
      return this->clauses.back();
   }
   
   Clauses::reference operator[](Clauses::size_type n) {
      return this->clauses[n];
   }

   Clauses::const_reference operator[](Clauses::size_type n) const {
      return this->clauses[n];
   }
   
   Clauses::reference at(const Clauses::size_type n) {
      return this->clauses.at(n);
   }

   Clauses::const_reference at(const Clauses::size_type n) const {
      return this->clauses.at(n);
   }

   void push_back(const Clauses::value_type& val) {
      this->clauses.push_back(std::make_unique<cnf::Clause>(*val));
      this->addClauseToWatchLiterals(this->clauses.back().get());
   }

   void push_back(Clauses::value_type&& val) {
      this->clauses.push_back(std::move(val));
      this->addClauseToWatchLiterals(this->clauses.back().get());
   }

   void pop_back() {
      this->removeClauseFromWatchLiterals(this->clauses.back().get());
      this->clauses.pop_back();
   }

   Clauses::iterator erase(Clauses::const_iterator position) {
      this->removeClauseFromWatchLiterals((*this)[position - this->begin()].get());
      return this->clauses.erase(position);
   }

   Clauses::iterator erase(Clauses::const_iterator first, Clauses::const_iterator last) {
      auto it = first;
      while (it != last) {
         this->removeClauseFromWatchLiterals((*this)[it - this->begin()].get());
         ++it;
      }
      return this->clauses.erase(first, last);
   }
/*
   Clauses::iterator insert(Clauses::const_iterator position, const Clauses::value_type& val) {
      return this->clauses.insert(position, val);
   }

   Clauses::iterator insert(Clauses::const_iterator position, Clauses::size_type n, const Clauses::value_type& val) {
      return this->clauses.insert(position, n, val);
   }

   Clauses::iterator insert(Clauses::const_iterator position, Clauses::value_type&& val) {
      return this->clauses.insert(position, val);
   }

   Clauses::iterator insert(Clauses::const_iterator position, std::initializer_list<Clauses::value_type> il) {
      return this->clauses.insert(position, il);
   }*/

   void clear() noexcept {
      this->clauses.clear();
      this->watchedLiterals.clear();
   }

   void reserve(Clauses::size_type n) {
      this->clauses.reserve(n);
   }

   Clauses::size_type capacity() const noexcept {
      return this->clauses.capacity();
   }

   void resize(Clauses::size_type n) {
      this->clauses.resize(n);
   }

   /*void resize(Clauses::size_type n, const Clauses::value_type& val) {
      this->clauses.resize(n, val);
   }*/

   Clauses::size_type size() const noexcept {
      return this->clauses.size();
   }

   Clauses::size_type max_size() const noexcept {
      return this->clauses.max_size();
   }

#pragma endregion vectorfunctions

   protected:

      /**
       * @brief Adds a clause to the watched literals.
       * @note Is only called by the vector functions to keep the watched literals accurate at all times
       * 
       * @param clause The pointer to the clause to add
       */
      void addClauseToWatchLiterals(cnf::Clause* clause) {
         if (clause->size() <= 1) {
            // nothing to be done
            return;
         }

         //if there's at least 2 literals, add the first two literals of the clause to the watched literals
         this->watchedLiterals[(*clause)[0]].push_back(clause);
         this->watchedLiterals[(*clause)[1]].push_back(clause);
      }

      /**
       * @brief Removes a clause from the watched literals.
       * @note Is only called by vector functions to keep the watched literals accurate at all times
       * 
       * @param clause The pointer to the clause to remove. The clause has to still exist when calling this
       */
      void removeClauseFromWatchLiterals(cnf::Clause* clause) {
         for (int literal : *clause) {
            auto& clauses = this->watchedLiterals[literal];
            clauses.erase(
               std::remove_if(clauses.begin(), clauses.end(), [&clause](cnf::Clause* c)->bool{
                  return c == clause;
               }),
               clauses.end()
            );
         }
      }

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
       * @brief Saves the Equivalence to the originally read formula. Will start at EQUIVALENT, and
       * then potentially degrade as procedures are applied.
       * 
       */
      EQUIVALENCE_TYPE equivalence;

      /**
       * @brief The total time this CNF was processed by a procedure
       * 
       */
      util::clock::duration totalProcessingTime;

      /**
       * @brief A set of procedures that were used on this CNF
       * 
       */
      std::set<std::string> procedures;

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

      /**
       * @brief Saves pointers to clauses, each clause is in here exactly twice (except for unit clauses)
       * 
       * @details This is used primarily by the BooleanConstraintPropagation procedure. It is stored in this class because
       * otherwise the procedure has to rebuild it in every iteration, which is very slow.
       */
      WatchedLiteralsT watchedLiterals;

   };

   /**
    * @brief Convert an EQUIVALENCE_TYPE from enum to a string
    * 
    * @param eqType The enum type to convert
    * @return std::string The resulting string
    */
   std::string equivalenceTypeToString(cnf::EQUIVALENCE_TYPE eqType);

}
