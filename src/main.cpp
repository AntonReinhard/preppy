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

   testCNF.readFromFile(inputFileName);

   util::Utility::logWarning("CNF has ", testCNF.getVariables(), " variables (", testCNF.getMaxVariable(), " max) and ", testCNF.size(), " clauses; Compressing...");

   testCNF.compress();

   util::Utility::logWarning("CNF has ", testCNF.getVariables(), " variables (", testCNF.getMaxVariable(), " max) and ", testCNF.size(), " clauses");

   solvers::clasp testSolver(std::chrono::seconds(5));

   cnf::Model testModel = testSolver.getModel(testCNF);

   util::Utility::logInfo("Test model read: ", testModel.toString());

   procedures::BackboneSimplification procedure(std::make_shared<solvers::clasp>(std::chrono::seconds(5)));

   auto backbone = procedure.getBackbone(testCNF);

   std::stringstream ss;
   for (const auto& l : backbone) {
      ss << l << " ";
   }

   util::Utility::logInfo("Computed backbone of the formula with ", backbone.size(), " literals: ", ss.str());

   util::Utility::cleanup();
}
