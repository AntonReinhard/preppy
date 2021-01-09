# preppy - A Preprocessor for SAT Model Counting

Preppy is a preprocessor for SAT Model Counting. It transforms cnf files into other cnf files, attempting to simplify them and make them easier for SAT Model Counters to solve.

## Installation

You need a C++ compiler supporting at least c++17, cmake version 3.1 or newer and make to build the program. To execute you also need a supported SAT-Solver.

Clone the repository and execute `./build.sh` in the repo's root directory. It will create a build directory for the build files, an install directory where the binary is copied and an output directory, where output files will be written by default. You can give the build script a cmake build type as argument, such as `./build.sh Debug`. By default Release will be used.

## Usage

`preppy [OPTION...] <INPUT FILE>`

### Options

  **-f, --force**: Override output files if they already exist

  **-o, --output=\<FILE/DIRECTORY\>**: Set a output file or directory. If a directory is set the output file name will be <inputfilestem>out.cnf

  **-v, -l, --verbose=LOG_LEVEL, --logging=LOG_LEVEL**: Set log level of the application, 0 = NOTHING, 1 = ERROR, 2 = WARNING (default), 3 = INFO, 4 = DEBUG

  **-?, --help**: Give this help list

  **--usage**: Give a short usage message

  **-V, --version**: Print program version

## Supported Solvers

- clasp
