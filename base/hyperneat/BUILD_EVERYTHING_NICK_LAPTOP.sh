#!/bin/sh

# NOTE: In some circumstances, you may need to clean the directory before building
# (if you don't know why your build is not working, this is a good place to start)
# for convencice, this functionality has been moved to a stand-alone script, do use it do:
# $ ./BUILD_CLEAN.sh


# BUILDING HyperNEAT:

# ==================================================================================================
# If you do not have Boost, install now before continuing 
#
# boost_stage is the location of "libboost_filesystem.a", libboost_system.a", and "libboost_thread.a"
# to find it, simply do:
# $ cd /usr
# $ find * | grep libboost_filesystem.a
#
boost_stage=/usr/local/lib # change me!  
#===================================================================================================

# Nothing after this point SHOULD need to be changed:

echo Building required packages
echo

rootDir=`pwd`
tinyXML_root=${rootDir}/tinyxmldll
JGTL_include=${rootDir}/JGTL/include

echo ${tinyXML_root}/out/libtinyxmldll.${boost_lib_ext}

# build TinyXML
if test -e ${tinyXML_root}/out/libtinyxmldll.${boost_lib_ext}
then
    echo TinyXML exists.  Skipping
    echo
else
    echo Building TinyXML
    cd ${rootDir}
    mkdir zlib/build/cygwin_debug
    mkdir zlib/build/cygwin_release
    cd zlib
    cmake .
    make

    cd ..
    mkdir tinyxmldll/build/cygwin_debug
    mkdir tinyxmldll/build/cygwin_release
    cd tinyxmldll
    cmake .
    make
    cd ${rootDir}

    echo Finished making TinyXML
    echo
fi

# check for Boost
if test -e ${boost_stage}/libboost_filesystem.a &&
   test -e ${boost_stage}/libboost_system.a &&
   test -e ${boost_stage}/libboost_thread.a
then
    echo Boost exists.  Skipping
    echo
else
    echo Boost does not exist.  Exitting.
    exit
fi

# build HyperNEAT
if test -e ${rootDir}/HyperNEAT/out/Hypercube_NEAT
#if test -e ${rootDir}/out/Hypercube_NEAT
then
    echo Hypercube_NEAT exists.  Skipping
    echo
else
    echo Building HyperNEAT
    cd ${rootDir}
    cd HyperNEAT
    # rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake  # <- moved to BUILD_CLEAN.sh; execute this script if you need to clear the cache before building 
    cmake .
    cmake -DCMAKE_INSTALL_PREFIX=${rootDir}/out -DJGTL_INCLUDE=${JGTL_include} -DBOOST_ROOT=${boost_root} -DBOOST_STAGE:string=${boost_stage} -DBUILD_PYTHON:bool=OFF -DEXECUTABLE_OUTPUT_PATH=${rootDir}/out -DJGTL_INCLUDE=${JGTL_include} -DLIBRARY_OUTPUT_PATH:string=${rootDir}/hyperneatLibs -DTINYXMLDLL_INCLUDE=${tinyXML_root}/include -DTINYXMLDLL_LIB=${tinyXML_root}/out -DUSE_GUI=OFF  -DCMAKE_COMPILER_IS_GNUCXX:string=/opt/intel/cce/10.0.025/bin/icpc -DBUILD_MPI:bool=OFF -DBOOSTOLD:bool=ON -SFML_INCLUDE=${SFML_root} -SFML_LIB=${SFML_lib} 
    make #--debug=v
    cd ${rootDir}

    echo Finished making Hypercube_NEAT
    echo
fi