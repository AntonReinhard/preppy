#include "CNF.h"
#include "Clause.h"
#include "Utility.h"
#include "solvers/clasp.h"

using namespace preppy;

int main(/*int argc, char** argv*/) {

   util::Utility::init();

   cnf::CNF testCNF;

   testCNF.readFromFile("../../examples/uf50-01.cnf");

   util::Utility::logInfo("CNF has ", testCNF.getVariables(), " variables and ", testCNF.size(), " clauses");

   solvers::clasp testSolver(std::chrono::seconds(5));

   cnf::Model testModel = testSolver.getModel(testCNF);

   util::Utility::logInfo("Test model read: ", testModel.toString());

   util::Utility::cleanup();
}
