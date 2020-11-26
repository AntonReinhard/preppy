#include "CNF.h"
#include "Clause.h"
#include "Utility.h"
#include "solvers/clasp.h"
#include "procedures/BackboneSimplification.h"
#include "procedures/BipartitionAndElimination.h"
#include "procedures/BooleanConstraintPropagation.h"

#include <string>
#include <algorithm>

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

   procedures::BooleanConstraintPropagation procedure;
   auto outputVariables = procedure.getBcp(testCNF);
   std::stringstream ss;
   for (const auto& var : outputVariables) {
      ss << var << " ";
   }
   util::Utility::logOutput("Unit propagated Literals: ", ss.str());

   procedure.apply(testCNF);

   util::Utility::logOutput("Formula: ", testCNF.toString());

   util::Utility::cleanup();
}
