/**
 * @file Model.h
 * @author Anton Reinhard
 * @brief Model header
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <string>
#include <vector>

namespace preppy::cnf {

   /**
    * @brief A model for a cnf formula, mapping variables to either true or false
    * TODO: Don't inherit from std::vector
    */
   class Model
      : public std::vector<bool> {
   public:

      /**
       * @brief Construct a new Model object
       * 
       */
      Model();

      /**
       * @brief Reads a model from a given file
       * 
       * @param filepath The filepath of the file to read from
       * @return bool True on success
       */
      bool readFromFile(const std::string& filepath);

      /**
       * @brief Reads a model from a stream
       * 
       * @param input The input stream to read from
       * @return bool True on success
       */
      bool readFromStream(std::istream& input);

      /**
       * @brief Converts the model to a string
       * 
       * @return std::string The string containing the model
       */
      std::string toString();

   protected:


   private:

   };

}
