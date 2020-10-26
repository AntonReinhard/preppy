#include "Model.h"
#include "Utility.h"

#include <string>
#include <vector>

namespace preppy::cnf {

   Model::Model() {

   }

   bool Model::readFromFile(const std::string& filepath) {
      if (!util::Utility::fileExists(filepath)) {
         util::Utility::logError("File ", filepath, " doesn't exist");
         return false;
      }

      std::ifstream file(filepath);
      if (!file.is_open()) {
         util::Utility::logError("File ", filepath, " couldn't be opened");
         return false;
      }

      return this->readFromStream(file);
   }

   bool Model::readFromStream(std::istream& input) {
      util::Utility::logDebug("Reading model");

      std::string line;
      // variable 0 doesn't exist ->
      this->push_back(false);

      while (!input.eof()) {
         std::getline(input, line);
         std::istringstream ss(line);
         char linetype;
         ss >> linetype;
         if ('v' == linetype) {      // variable lines will have a v at the beginning
            int literal;
            while (ss >> literal) {
               if (literal > 0) {
                  this->push_back(true);
               }
               else if (literal < 0) {
                  this->push_back(false);
               }
               else {
                  // 0 is at the end of the modelgi
                  break;
               }
            }
         }
      }

      return true;
   }

   std::string Model::toString() {
      std::ostringstream ss;
      for (size_t i = 1; i < this->size(); ++i) {
         ss << ((*this)[i] ? "" : "-")    // - if literal is negative
            << i << " ";
      }

      return ss.str();
   }

}
