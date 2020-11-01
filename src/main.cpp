#include "CNF.h"
#include "Clause.h"
#include "Utility.h"
#include "solvers/clasp.h"
#include "procedures/BackboneSimplification.h"
#include "procedures/BipartitionAndElimination.h"

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

/*
   procedures::BackboneSimplification procedure(util::Utility::getSolver());

   util::Utility::logOutput("Computing backbone of the formula");
   procedure.apply(testCNF);
*/

   procedures::BipartitionAndElimination procedure;

   util::Utility::logOutput("Computing Bipartition of the formula");
   cnf::Variables outputVariables = procedure.bipartition(testCNF);

   std::stringstream ss;
   for (const auto& var : outputVariables) {
      ss << var << " ";
   }
   util::Utility::logOutput("Output Variables: ", ss.str());

/*
   std::string solutionFileName = "simplified.cnf";
   util::Utility::logOutput("Writing result to \"", solutionFileName, "\"");
   
   testCNF.writeToFile(solutionFileName, true);
*/
   util::Utility::cleanup();
}
