/**
 * @file main.cpp
 * @author Anton Reinhard
 * @brief main file
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "CNF.h"
#include "Clause.h"
#include "Utility.h"
#include "CommandLineParser.h"
#include "solvers/clasp.h"
#include "procedures/BackboneSimplification.h"
#include "procedures/BipartitionAndElimination.h"
#include "procedures/BooleanConstraintPropagation.h"
#include "procedures/Vivification.h"

#include <string>
#include <algorithm>

using namespace preppy;

int main(const int argc, char** argv) {

   util::Arguments args = util::Utility::parseCommandLine(argc, argv);

   std::string inputFileName = args.fileIn;

   util::Utility::init();

   cnf::CNF testCNF;

   util::Utility::logOutput("Reading file \"", inputFileName, "\"");
   testCNF.readFromFile(inputFileName);

   auto variablesPre = testCNF.getVariables();
   auto maxVariablePre = testCNF.getMaxVariable();
   auto clausesPre = testCNF.getClauses();
   auto literalsPre = testCNF.getLiterals();

   util::Utility::logOutput("CNF has ", variablesPre, " variables (", maxVariablePre, " max), ", clausesPre, " clauses and ", literalsPre, " literals");

   if (!testCNF.isCompressed()) {
      util::Utility::logOutput("Formula isn't compressed, compressing...");
      testCNF.compress();
      variablesPre = testCNF.getVariables();
      maxVariablePre = testCNF.getMaxVariable();
      util::Utility::logOutput("CNF has ", variablesPre, " variables (", maxVariablePre, " max) after compression");
   }

   util::Utility::startTimer("vivification");
   procedures::Vivification procedure;
   procedure.apply(testCNF);
   testCNF.addProcessingTime(util::Utility::stopTimer("vivification"));

   auto variablesPost = testCNF.getVariables();
   auto maxVariablePost = testCNF.getMaxVariable();
   auto clausesPost = testCNF.getClauses();
   auto literalsPost = testCNF.getLiterals();

   util::Utility::logOutput("CNF has ", variablesPost, " variables (", maxVariablePost, " max), ", clausesPost, " clauses and ", literalsPost, " literals");
   util::Utility::logOutput("Removed ", clausesPre - clausesPost, " clauses and ", literalsPre - literalsPost, " literals");

   if (args.fileOut.empty()) {
      testCNF.writeToFile("output/", args.force);
   }
   else {
      testCNF.writeToFile(args.fileOut, args.force);
   }

   util::Utility::cleanup();
}
