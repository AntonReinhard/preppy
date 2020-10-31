#pragma once
#include "Clause.h"
#include "Model.h"

#include <vector>
#include <initializer_list>
#include <tuple>

namespace preppy::cnf {

   /*
    * Represents a CNF Formula. Derived from a vector of Clauses
    */
   class CNF 
      : public std::vector<cnf::Clause> {
   public:

      // Creates empty CNF
      CNF();

      // Creates a CNF with given initializer list of Clauses
      CNF(std::initializer_list<cnf::Clause> l);

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

   protected:


   private:

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
