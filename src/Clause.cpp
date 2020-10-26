#include "Clause.h"
#include "Utility.h"

#include <string>
#include <sstream>
#include <vector>

namespace preppy::cnf {

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

}
