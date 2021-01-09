/**
 * @file Procedures.h
 * @author Anton Reinhard
 * @brief Procedures abstract definition
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "../definitions.h"
#include "../CNF.h"

namespace preppy::procedures {

   /**
    * @brief Interface of every procedure
    * 
    */
   class Procedure {
   public:

      /**
       * @brief Construct a new Procedure
       * 
       * @param name The name of the procedure
       * @param eqType The equivalence type this procedure will guarantee
       */
      Procedure(const std::string& name, cnf::EQUIVALENCE_TYPE eqType);

      /**
       * @brief Destroy a procedure object
       * 
       */
      virtual ~Procedure() = default;

      /**
       * @brief Takes a reference to the formula and applies the procedure on it.
       * 
       * @param formula The formula to apply the procedure to
       * @return bool True on success, False if some error occurred
       * 
       * @details This is the function to call when applying a procedure to a given CNF. 
       * It will add this procedure's name to the applied procedures of the formula and
       * set the formula's Equivalence Type according to the procedure's Equivalence Type.
       * Then it calls the impl() function implemented by inheriting classes.
       * 
       * @note A procedure may not recursively call this function to apply itself recursively. Instead
       * call impl recursively. This function is only the entry point.
       */
      bool apply(cnf::CNF& formula);

      /**
       * @brief Get the name of this procedure
       * 
       * @return std::string The name
       */
      std::string getName() const;

      /**
       * @brief Get the equivalence type of this procedure
       * 
       * @return cnf::EQUIVALENCE_TYPE The equivalence type
       */
      cnf::EQUIVALENCE_TYPE getEqType() const;

   protected:

      /**
       * @brief Takes a reference to the formula and works on it. This will be called
       * only by apply() and implemented by the specific Procedure inheriting from this.
       * 
       * @param formula The formula to apply the procedure to and work on
       * @return bool True on success, False if some error occurred
       */
      virtual bool impl(cnf::CNF& formula) = 0;

      /**
       * @brief Adds itself to having been applied on the formula
       * 
       * @details Adds this procedure's name to the applied procedures of the given formula
       * and sets the equivalence type of the formula according to this procedure's set equivalence
       * type.
       */
      void setApplied(cnf::CNF& formula) const;

   private:

      /**
       * @brief Name of this procedure, set in the constructor
       * 
       */
      const std::string name;

      /**
       * @brief The type of equivalence this procedure guarantees
       * 
       */
      const cnf::EQUIVALENCE_TYPE eqType;

   };

}
