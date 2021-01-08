/**
 * @file Solver.h
 * @author Anton Reinhard
 * @brief Solver base class header
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "../CNF.h"
#include "../Model.h"

namespace preppy::solvers {

   /**
    * @brief Solver base class
    * 
    */
   class Solver {
   public:

      /**
       * @brief Constructs the solver with a name. The name should also be the systemcall, e.g. clasp
       * 
       * @param name The name of the solver
       */
      Solver(std::string name);

      /**
       * @brief Destroy the Solver
       * 
       */
      virtual ~Solver() = default;

      /**
       * @brief Checks whether a formula is satisfiable
       * 
       * @param formula The formula to check
       * @return bool True if satisfiable, False if unsatisfiable, error or timeout
       */
      virtual bool isSatisfiable(cnf::CNF& formula) = 0;

      /**
       * @brief Gets a model for the formula
       * 
       * @param formula The formula to get a model for
       * @return cnf::Model A single model for the formula, or empty if the formula is unsatisfiable, or on timeout or error
       */
      virtual cnf::Model getModel(cnf::CNF& formula) = 0;

      /**
       * @brief Returns the name of this solver
       * 
       * @return const std::string& The name of the solver
       */
      const std::string& getName();

   protected:

      /**
       * @brief Helper function executing a solver with the given command, putting the result in the string solution
       * 
       * @param command The command to execute
       * @param formula The formula as input
       * @param solution Will contain the solution after the call
       * @return bool True on success, False on error, e.g. if file could not be written
       */
      bool executeSolver(const std::string& command, cnf::CNF& formula, std::string& solution);

      /**
       * @brief The file name to write the formula to before executing the solver
       * 
       */
      const std::string inFileName = "in.cnf";

   private:

      /**
       * @brief The name of the solver
       * 
       */
      std::string solvername;

   };

}
