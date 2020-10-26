#include "CNF.h"
#include "Utility.h"

#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <sstream>

namespace preppy::cnf {

   CNF::CNF() 
      : variablesDirtyBit(true) 
      , maxVariableDirtyBit(true){

   }

   bool CNF::readFromFile(const std::string& filepath) {
      this->name = filepath;
      if (!util::Utility::fileExists(filepath)) {
         util::Utility::logError("File ", filepath, " doesn't exist");
         return false;
      }

      std::fstream file(filepath);
      if (!file.is_open()) {
         util::Utility::logError("File ", filepath, " couldn't be opened");
         return false;
      }

      // collects all the lines containing clauses
      std::stringstream clausestream;
      unsigned variables, clauses;

      std::string line;
      bool problemLineFound = false;
      while (!file.eof()) {
         std::getline(file, line);
         if ('c' == line[0]) {            // comment line
         }
         else if ('p' == line[0]) {       // problem line
            if (problemLineFound) {
               util::Utility::logError("Multiple problem lines found in cnf file \"", this->name, "\"");
               return false;
            }
            std::stringstream ss(line);
            std::string discard;
            ss >> discard;                // "p"
            ss >> discard;                // "cnf"
            ss >> variables;
            ss >> clauses;
            util::Utility::logInfo("Read problem line: ", variables, " variables and ", clauses, " clauses");
            
            problemLineFound = true;
         }
         else {
            clausestream << line;
         }
      }

      if (!problemLineFound) {
         util::Utility::logWarning("There was no problem line to read");
         return false;
      }

      std::vector<std::string> clausestrings = util::Utility::tokenizeString(clausestream.str(), " 0");

      unsigned maxVar = 0;
      for (const auto& clausestring : clausestrings) {
         this->push_back(Clause(clausestring));
         
         unsigned v = this->back().getMaxVariable();
         
         if (v >= maxVar) {
            maxVar = v;
         }
      }

      this->maxVariableDirtyBit = false;
      this->maxVariable = maxVar;

      if (this->size() != clauses) {
         util::Utility::logWarning("Number of clauses read (", this->size(), ") differs from number of clauses in header (", clauses, ")");
      }
      if (this->getVariables() != variables) {
         util::Utility::logWarning("Number of variables read (", this->getVariables(), ") differs from number of variables in header (", variables, ")");
      }
      if (!this->isCompressed()) {
         util::Utility::logWarning("The variables in this cnf are not dense, maximum variable is ", maxVar, " but number of variables is ", this->getVariables());
      }

      util::Utility::logInfo("Successfully read ", this->getVariables(), " variables and ", this->size(), " clauses from file");

      return true;
   }

   std::string CNF::toString() const {
      std::stringstream ss;
      for (const auto& clause : *this) {
         ss << clause.toString() << "\n";
      }
      return ss.str();
   }

   bool CNF::writeToFile(const std::string& path, const bool force) {
      if (!force && util::Utility::fileExists(path)) {
         util::Utility::logWarning("File ", path, " already exists");
         return false;
      }
      std::ofstream file(path);
      if (!file.is_open()) {
         if (util::Utility::fileExists(path)) {
            util::Utility::logError("Couldn't open file ", path);
         }
         else {
            util::Utility::logError("Couldn't create file ", path);
         }
         return false;
      }

      // write header
      file << "c " << this->name << std::endl;
      file << "p cnf " << this->getMaxVariable() << " " << this->size() << std::endl;
      for (const auto& clause : *this) {
         file << clause.toCNFLine();
      }

      file.close();
      return true;
   }

   unsigned CNF::getVariables() {
      if (this->variablesDirtyBit) {
         std::set<unsigned> variables;
         for (const auto& clause : *this) {
            for (const int var : clause) {
               variables.insert(std::abs(var));
            }
         }
         this->variables = variables.size();
         this->variablesDirtyBit = false;
      }

      return this->variables;
   }

   unsigned CNF::getMaxVariable() {
      if (this->maxVariableDirtyBit) {
         unsigned maxVar = 0;
         for (const auto& clause : *this) {
            if (clause.getMaxVariable() > maxVar) {
               maxVar = clause.getMaxVariable();
            }
         }
         this->maxVariable = maxVar;
         this->maxVariableDirtyBit = false;
      }

      return this->maxVariable;
   }

   bool CNF::isCompressed() {
      return this->getVariables() == this->getMaxVariable();
   }

}
