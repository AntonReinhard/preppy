#pragma once

#include <string>
#include <vector>

namespace preppy::cnf {

   class Model 
      : public std::vector<bool> {
   public:

      Model();

      // Reads a model from a given file, returns true on success
      bool readFromFile(const std::string& filepath);

      // Reads a model from a stream, returns true on success
      bool readFromStream(std::istream& input);

      // Returns a string containing the model
      std::string toString();

   protected:


   private:

   };

}
