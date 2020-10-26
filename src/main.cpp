#include "CNF.h"
#include "Clause.h"
#include "Utility.h"
#include "solvers/clasp.h"

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

   util::Utility::logInfo("CNF has ", testCNF.getVariables(), " variables and ", testCNF.size(), " clauses");

   solvers::clasp testSolver(std::chrono::seconds(5));

   cnf::Model testModel = testSolver.getModel(testCNF);

   util::Utility::logInfo("Test model read: ", testModel.toString());

   util::Utility::cleanup();
}
