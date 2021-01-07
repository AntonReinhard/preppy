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
    echo "Deleting build"
    rm -r build
fi

if [ -d "install" ]
then
    echo "Deleting install"
    rm -r install
fi

mkdir build && mkdir install

cd build
echo "Executing cmake"
cmake .. -DCMAKE_BUILD_TYPE=$BuildType -DCMAKE_INSTALL_PREFIX=../install > /dev/null

echo "Executing make"
make install > /dev/null
