#include "CNF.h"
#include "Clause.h"
#include "Utility.h"
#include "solvers/clasp.h"
#include "procedures/BackboneSimplification.h"

#include <string>

using namespace preppy;

int main(const int argc, char** argv) {

   if (!util::Utility::parseCommandLine(argc, argv)) {
      exit(0);
   }

   std::string inputFileName = argv[1];

   util::Utility::init();

   cnf::CNF testCNF;

   util::Utility::logOutput("Reading file \"", inputFileName, "\"");
   testCNF.readFromFile(inputFileName);

   auto variables = testCNF.getVariables();
   auto maxVariable = testCNF.getMaxVariable();
   auto clauses = testCNF.getClauses();

   util::Utility::logOutput("CNF has ", variables, " variables (", maxVariable, " max) and ", clauses, " clauses");

   if (!testCNF.isCompressed()) {
      util::Utility::logOutput("Formula isn't compressed, compressing...");
      testCNF.compress();
      variables = testCNF.getVariables();
      maxVariable = testCNF.getMaxVariable();
      util::Utility::logOutput("CNF has ", variables, " variables (", maxVariable, " max) after compression");
   }

   solvers::clasp testSolver(std::chrono::seconds(5));
   procedures::BackboneSimplification procedure(std::make_shared<solvers::clasp>(std::chrono::seconds(5)));

   util::Utility::logOutput("Computing backbone of the formula");
   procedure.apply(testCNF);

   //util::Utility::logOutput("Computed backbone of the formula with ", testCNF.getClauses() - clauses, " literals");

   std::string solutionFileName = "simplified.cnf";
   util::Utility::logOutput("Writing result to \"", solutionFileName, "\"");
   
   testCNF.writeToFile(solutionFileName, true);

   util::Utility::cleanup();
}
