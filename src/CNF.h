#pragma once
#include "Clause.h"

#include <vector>

namespace preppy::cnf {

   /*
    * Represents a CNF Formula. Derived from a vector of Clauses
    */
   class CNF 
      : public std::vector<cnf::Clause> {
   public:

      // Creates empty CNF
      CNF();

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

      // Returns true if there are any variables that don't occur in the formula but are smaller than the biggest variable
      bool isCompressed();

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
   };

}
