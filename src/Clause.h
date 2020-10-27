#pragma once

#include <string>
#include <vector>

namespace preppy::cnf {

   /*
    * Representing a Clause, consisting of literals represented by numbers. Negative number means the literal is negated.
    */
   class Clause 
      : public std::vector<int> {
   public:

      // Construct empty Clause
      Clause() = default;

      // Construct this Clause from a line in a cnf file
      Clause(const std::string& line);

      // Sets the given literal to true, if the clause becomes satisfied through this it will be empty
      // Returns true if the clause is satisfied afterwards
      bool setLiteral(const int literal);

      // Renames the literals in the clause from old to new
      void renameVariable(const unsigned oldVar, const unsigned newVar);

      // Get this clause as a human readable string
      std::string toString() const;

      // Get a string representing this clause in cnf format
      std::string toCNFLine() const;

      // Returns the maximum occuring variable in this clause
      unsigned getMaxVariable() const;

   protected:


   private:


   };

}
