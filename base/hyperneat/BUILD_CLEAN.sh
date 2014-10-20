#!/bin/sh

echo
rootDir=`pwd`
echo Cleaning HyperNEAT
cd ${rootDir}
cd HyperNEAT
rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake zlib/CMakeCache.txt tinyxmldll/CMakeCache.txt
echo removed: CMakeFiles CMakeCache.txt cmake_install.cmake
cd ../zlib
mkdir build
rm -rf CMakeCache.txt
cmake .
echo removed zlib/CMakeCache.txt and remade
cd ../tinyxmldll
mkdir build
rm -rf CMakeCache.txt
cmake .
echo removed tinyxmldll/CMakeCache.txt and remade
echo done.
echo

