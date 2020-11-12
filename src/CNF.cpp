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

   CNF::CNF(std::initializer_list<Clauses::value_type> l)
      : clauses(l)
      , variablesDirtyBit(true)
      , maxVariableDirtyBit(true) {

   }

   void CNF::compress() {
      util::Utility::logDebug("Compressing CNF by ", this->getMaxVariable() - this->getVariables(), " variables");

      if (this->isCompressed()) {
         return;
      }

      // collect all variables used in the formula
      std::set<unsigned> variables;
      for (const auto& clause : *this) {
         for (const int var : clause) {
            variables.insert(std::abs(var));
         }
      }

      // compress
      for (unsigned i = 1; !variables.empty() && i < *(variables.rbegin()); ++i) {
         const int maxVar = *(variables.rbegin());
         
         // if this variable is missing -> compress
         if (variables.find(i) == variables.end()) {
            variables.erase(maxVar);
            this->renameVariable(maxVar, i);
            this->compressionInformation.emplace_back(maxVar, i, false);
         }
      }
      this->maxVariableDirtyBit = true;
   }

   int CNF::compress(int literal) {
      bool sign = (literal > 0);
      literal = std::abs(literal);
      for (const auto& step : this->compressionInformation) {
         if (literal == static_cast<int>(std::get<0>(step))) {
            literal = std::get<1>(step);
         }
      }

      return sign ? literal : -literal;
   }

   void CNF::compress(cnf::Model& model) {
      for (const auto& step : this->compressionInformation) {
         model[std::get<1>(step)] = model[std::get<0>(step)];
         model.erase(model.begin() + std::get<0>(step));
      }
   }

   void CNF::decompress() {
      // TODO
   }

   int CNF::decompress(int literal) {
      bool sign = (literal > 0);
      literal = std::abs(literal);
      
      for (auto step = this->compressionInformation.rbegin(); step != this->compressionInformation.rend(); ++step) {
         if (literal == static_cast<int>(std::get<1>(*step))) {
            literal = std::get<0>(*step);
         }
      }

      return sign ? literal : -literal;
   }

   void CNF::decompress(cnf::Model& model) {
      //for (auto step = this->compressionInformation.rbegin(); step != this->compressionInformation.rend(); ++step) {
      for (auto step : this->compressionInformation) {
         model.insert(model.begin() + std::get<1>(step), std::get<2>(step));
      }
   }

   void CNF::renameVariable(const unsigned variable, const unsigned newName) {
      for (auto& clause : *this) {
         clause.renameVariable(variable, newName);
      }
      this->maxVariableDirtyBit = true;
   }

   void CNF::joinFormula(const cnf::CNF& formula) {
      for (const auto& clause : formula) {
         this->push_back(clause);
      }
      this->maxVariableDirtyBit = true;
      this->variablesDirtyBit = true;
   }

   void CNF::setLiteralBackpropagated(int literal) {
      // literal is not part of the formula anymore, so take biggest variable and rename it to literal's variable name
      
      for (auto& clause : *this) {
         clause.renameVariable(this->getMaxVariable(), std::abs(literal));
      }
      this->maxVariable--;
      this->compressionInformation.emplace_back(this->getMaxVariable(), std::abs(literal), (literal>0));
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

   unsigned CNF::getClauses() const {
      return this->size();
   }

   bool CNF::isCompressed() {
      return this->getVariables() == this->getMaxVariable();
   }

   void CNF::setDirtyBitsTrue() {
      this->maxVariableDirtyBit = true;
      this->variablesDirtyBit = true;
   }

   std::vector<unsigned> CNF::countVariables() {
      std::vector<unsigned> varCount(this->getVariables());
      for (const auto& clause : *this) {
         for (const auto& literal : clause) {
            varCount[std::abs(literal)]++;
         }
      }

      return varCount;
   }

   Clauses::iterator CNF::begin() noexcept {
      return this->clauses.begin();
   }

   Clauses::const_iterator CNF::begin() const noexcept {
      return this->clauses.begin();
   }

   Clauses::iterator CNF::end() noexcept {
      return this->clauses.end();
   }

   Clauses::const_iterator CNF::end() const noexcept {
      return this->clauses.end();
   }
   
   Clauses::reference CNF::front() {
      return this->clauses.front();
   }

   Clauses::const_reference CNF::front() const {
      return this->clauses.front();
   }

   Clauses::reference CNF::back() {
      return this->clauses.back();
   }
   
   Clauses::const_reference CNF::back() const {
      return this->clauses.back();
   }
   
   Clauses::reference CNF::operator[](Clauses::size_type n) {
      return this->clauses[n];
   }

   Clauses::const_reference CNF::operator[](Clauses::size_type n) const {
      return this->clauses[n];
   }
   
   Clauses::reference CNF::at(const Clauses::size_type n) {
      return this->clauses.at(n);
   }

   Clauses::const_reference CNF::at(const Clauses::size_type n) const {
      return this->clauses.at(n);
   }

   void CNF::push_back(const Clauses::value_type& val) {
      this->clauses.push_back(val);
   }

   void CNF::push_back(Clauses::value_type&& val) {
      this->clauses.push_back(val);
   }

   void CNF::pop_back() {
      this->clauses.pop_back();
   }

   Clauses::iterator CNF::erase(Clauses::const_iterator position) {
      return this->clauses.erase(position);
   }

   Clauses::iterator CNF::erase(Clauses::const_iterator first, Clauses::const_iterator last) {
      return this->clauses.erase(first, last);
   }

   Clauses::iterator CNF::insert(Clauses::const_iterator position, const Clauses::value_type& val) {
      return this->clauses.insert(position, val);
   }

   Clauses::iterator CNF::insert(Clauses::const_iterator position, Clauses::size_type n, const Clauses::value_type& val) {
      return this->clauses.insert(position, n, val);
   }

   Clauses::iterator CNF::insert(Clauses::const_iterator position, Clauses::value_type&& val) {
      return this->clauses.insert(position, val);
   }

   Clauses::iterator CNF::insert(Clauses::const_iterator position, std::initializer_list<Clauses::value_type> il) {
      return this->clauses.insert(position, il);
   }

   void CNF::clear() noexcept {
      this->clauses.clear();
   }

   void CNF::reserve(Clauses::size_type n) {
      this->clauses.reserve(n);
   }

   Clauses::size_type CNF::capacity() const noexcept {
      return this->clauses.capacity();
   }

   void CNF::resize(Clauses::size_type n) {
      this->clauses.resize(n);
   }

   void CNF::resize(Clauses::size_type n, const Clauses::value_type& val) {
      this->clauses.resize(n, val);
   }

   Clauses::size_type CNF::size() const noexcept {
      return this->clauses.size();
   }

   Clauses::size_type CNF::max_size() const noexcept {
      return this->clauses.max_size();
   }

}
