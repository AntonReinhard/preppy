#!/bin/bash
if [[ $# -ge 2 ]]
then
    echo "Please give \"Debug\", \"Release\", \"MinSizeRel\" or \"RelWithDebInfo\" as the only argument"
    exit 1
fi

if [[ $# -eq 0 ]]
then
    echo "No given Build Type, using Release"
    BuildType=Release
else
    BuildType=$1
fi

if [ -d "build" ]
then
    echo "build directory already exists"
else
    mkdir build
fi

if [ -d "install" ]
then
    echo "install directory already exists"
else
    mkdir install
fi

if [ -d "output" ]
then
    echo "output directory already exists"
else
    mkdir output
fi

cd build
echo "Executing cmake"
cmake .. -DCMAKE_BUILD_TYPE=$BuildType -DCMAKE_INSTALL_PREFIX=../install > /dev/null

echo "Executing make"
make install > /dev/null
