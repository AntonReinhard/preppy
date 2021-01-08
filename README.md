# preppy - A Preprocessor for SAT Model Counting

Preppy is a preprocessor for SAT Model Counting. It transforms cnf files into other cnf files, attempting to simplify them and make them easier for SAT Model Counters to solve.

## Installation

You need a C++ compiler supporting at least c++17, cmake version 3.1 or newer and make to build the program. To execute you also need a supported SAT-Solver.

Clone the repository and execute `./build.sh` in the repo's root directory. It will create a build directory for the build files, an install directory where the binary is copied and an output directory, where output files will be written by default. You can give the build script a cmake build type as argument, such as `./build.sh Debug`. By default Release will be used.

## Usage

Basic Usage: `./install/bin/preppy [input cnf file] [options]`

Use `./install/bin/preppy -h` for help

### Options
- `-h`: Get info on how to use preppy
- `-v <n>`: Setting the log level. 0 means nothing will be logged, 1 means only errors and program output will be logged, 2 means additionally warnings will be logged, 3 and 4 add more debugging info respectively.

## Supported Solvers

- clasp
