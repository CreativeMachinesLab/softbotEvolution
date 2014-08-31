#!/bin/sh

echo
rootDir=`pwd`
echo Cleaning HyperNEAT
cd ${rootDir}
cd HyperNEAT
rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake
echo removed: CMakeFiles CMakeCache.txt cmake_install.cmake
echo done.
echo