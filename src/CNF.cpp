/**
 * @file CNF.cpp
 * @author Anton Reinhard
 * @brief CNF implementation
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "CNF.h"
#include "Clause.h"
#include "Utility.h"

#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <sstream>

namespace preppy::cnf {

   CNF::CNF() 
      : variablesDirtyBit(true) 
      , maxVariableDirtyBit(true)
      , totalProcessingTime(0)
      , equivalence(EQUIVALENCE_TYPE::EQUIVALENT) {

   }

   CNF::CNF(const cnf::CNF& other) {
      *this = other;
   }

   cnf::CNF& CNF::operator=(const cnf::CNF& other) {
      if (&other == this) {
         return *this;
      }

      this->name = other.name;
      this->source = other.source;
      this->totalProcessingTime = other.totalProcessingTime;
      this->procedures = other.procedures;
      this->equivalence = other.equivalence;

      for (const auto& clause : other) {
         this->push_back(std::make_unique<cnf::Clause>(*clause));
      }
      return *this;
   }

   CNF CNF::getMetadataCopy() const {
      CNF copy;
      copy.name = this->name;
      copy.source = this->source;
      copy.totalProcessingTime = this->totalProcessingTime;
      copy.procedures = this->procedures;
      copy.equivalence = this->equivalence;
      return copy;
   }

   void CNF::compress() {
      util::Utility::logDebug("Compressing CNF by ", this->getMaxVariable() - this->getVariables(), " variables");

      if (this->isCompressed()) {
         return;
      }

      // collect all variables used in the formula
      std::set<unsigned> variables;
      for (const auto& clause : *this) {
         for (const int var : *clause) {
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
         clause->renameVariable(variable, newName);
      }
      this->maxVariableDirtyBit = true;
      this->variablesDirtyBit = true;
   }

   void CNF::joinFormula(const cnf::CNF& formula) {
      for (const auto& clause : formula) {
         this->push_back(std::make_unique<cnf::Clause>(*clause));
      }
      this->maxVariableDirtyBit = true;
      this->variablesDirtyBit = true;
   }

   void CNF::setLiteralBackpropagated(int literal) {
      // literal is not part of the formula anymore, so take biggest variable and rename it to literal's variable name
      
      for (auto& clause : *this) {
         clause->renameVariable(this->getMaxVariable(), std::abs(literal));
      }
      this->maxVariable--;
      this->compressionInformation.emplace_back(this->getMaxVariable(), std::abs(literal), (literal>0));
   }

   bool CNF::readFromFile(const std::string& filepath) {
      this->name = filepath;
      this->source = filepath;
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
            
            this->reserve(clauses * 1.2);
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
         this->push_back(std::make_unique<Clause>(clausestring));
         
         unsigned v = this->back()->getMaxVariable();
         
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
         ss << clause->toString() << "\n";
      }
      return ss.str();
   }

   bool CNF::writeToFile(const std::string& path, const bool force) {
      std::filesystem::path filePath;

      if (util::Utility::isDirectory(path)) {
         std::string origFileName;
         if (!this->source.has_stem()) {
            util::Utility::logWarning("CNF doesn't have an original file name, using default 'out'");
         }
         else {
            origFileName = this->source.stem();
         }
         origFileName += "out";
         filePath = path + origFileName;
         filePath.replace_extension("cnf");
      }
      else {
         // assume it's a file
         filePath = path;
      }

      util::Utility::logDebug("Writing CNF to file ", filePath);

      if (!force && util::Utility::fileExists(filePath)) {
         util::Utility::logWarning("File ", filePath, " already exists, use -f to force override");
         return false;
      }

      std::ofstream file(filePath);
      if (!file.is_open()) {
         if (util::Utility::fileExists(path)) {
            util::Utility::logError("Couldn't open file ", filePath);
         }
         else {
            util::Utility::logError("Couldn't create file ", filePath);
         }
         return false;
      }

      file << this->getFileHeader();

      for (const auto& clause : *this) {
         file << clause->toCNFLine();
      }

      file.close();
      return true;
   }

   std::string CNF::getFileHeader() {
      std::stringstream ss;
      
      ss << "c " << this->name << "\n";
      ss << "c Processed by preppy v" << util::VERSION << " Type: " << util::BUILD_TYPE << "\n";
      ss << "c Processing took " << util::Utility::durationToString(this->totalProcessingTime) << "\n";
      
      // Add a line telling what procedures were used
      ss << "c Used Procedures: ";

      if (this->procedures.empty()) {
         ss << "None";
      }
      else {
         // write comma seperated procedure names
         unsigned i = 0;
         for (const auto& proc : this->procedures) {
            ++i;
            ss << "\"" << proc << "\"";
            if (i < this->procedures.size()) {
               ss << ", ";
            }
         }
      }
      ss << "\n";

      // Add a line telling what equivalence to the original input formula remains
      if (this->equivalence == EQUIVALENCE_TYPE::EQUIVALENT) {
         ss << "c This formula is Equivalent to the original input formula\n";
      }
      else {
         ss << "c This formula is at least " << cnf::equivalenceTypeToString(this->equivalence) << " to the original input formula\n";
      }

      // actual header line
      ss << "p cnf " << this->getMaxVariable() << " " << this->size() << "\n";

      return ss.str();
   }

   unsigned CNF::getVariables() {
      if (this->variablesDirtyBit) {
         std::set<unsigned> variables;
         for (const auto& clause : *this) {
            for (const int var : *clause) {
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
            if (clause->getMaxVariable() > maxVar) {
               maxVar = clause->getMaxVariable();
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

   unsigned CNF::getLiterals() const {
      unsigned totalLiterals = 0;
      for (const auto& clause : *this) {
         totalLiterals += clause->size();
      }
      return totalLiterals;
   }

   bool CNF::isCompressed() {
      return this->getVariables() == this->getMaxVariable();
   }

   void CNF::setDirtyBitsTrue() {
      this->maxVariableDirtyBit = true;
      this->variablesDirtyBit = true;
   }

   std::vector<unsigned> CNF::countVariables() {
      std::vector<unsigned> varCount(this->getMaxVariable() + 1);
      for (const auto& clause : *this) {
         for (const auto& literal : *clause) {
            varCount[std::abs(literal)]++;
         }
      }

      return varCount;
   }

   void CNF::addProcedure(const std::string& procedure) {
      this->procedures.insert(procedure);
   }

   void CNF::setEquivalence(const cnf::EQUIVALENCE_TYPE eqType) {
      if (eqType > this->equivalence) {
         this->equivalence = eqType;
      }
   }

   void CNF::addProcessingTime(const util::clock::duration& duration) {
      this->totalProcessingTime += duration;
   }

   WatchedLiteralsT& CNF::getWatchedLiterals() {
      return this->watchedLiterals;
   }

   std::string equivalenceTypeToString(cnf::EQUIVALENCE_TYPE eqType) {
      switch (eqType) {
      case cnf::EQUIVALENCE_TYPE::EQUIVALENT:
         return "Equivalent";
      case cnf::EQUIVALENCE_TYPE::NUMBER_EQUIVALENT:
         return "#Equivalent";
      case cnf::EQUIVALENCE_TYPE::SAT_EQUIVALENT:
         return "satEquivalent";
      case cnf::EQUIVALENCE_TYPE::UNEQUIVALENT:
         return "Not Equivalent";
      default:
         return "Unknown Equivalence";
      }
   }

}
