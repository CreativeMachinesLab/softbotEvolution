Project(NEAT)

ADD_DEFINITIONS(-DBUILD_NEAT_DLL -DTIXML_USE_STL -DHCUBE_NOGUI)

IF(ENDLESSFORMS)
        ADD_DEFINITIONS(-DENDLESSFORMS)
ENDIF(ENDLESSFORMS)


IF(BOOSTOLD)
        ADD_DEFINITIONS(-DBOOSTOLD)
ENDIF(BOOSTOLD)


INCLUDE_REGULAR_EXPRESSION( "^.*[Nn][Ee][Aa][Tt][_].*$" )

#GET_CMAKE_PROPERTY(CMAKETESTPROP CACHE_VARIABLES)

SET(
TINYXMLDLL_INCLUDE
"../../../tinyxmldll/include"
CACHE
FILEPATH 
"TinyXMLDLL include directory"
) 

SET(
BOOST_ROOT
"../../../../Libraries/boost-trunk"
CACHE
FILEPATH 
"Boost Base Directory"
)

SET(
JGTL_INCLUDE
"../../../JGTL/include"
CACHE
FILEPATH 
"JGTL include directory"
) 

SET(
ZLIB_INCLUDE
"../../../zlib"
CACHE
FILEPATH 
"Zlib include directory"
) 

INCLUDE_DIRECTORIES(
include
${TINYXMLDLL_INCLUDE}
${BOOST_ROOT}
${Boost_INCLUDE_DIRS}
${JGTL_INCLUDE}
${ZLIB_INCLUDE}
)

IF(CYGWIN)

	SET(
	BOOST_STAGE
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../../Libraries/boost-trunk/stage_cygwin/lib"
	CACHE
	FILEPATH 
	"Boost Stage Directory"
	)

ELSE(CYGWIN)
	IF(WIN32)
		SET(
		BOOST_STAGE
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../Libraries/boost-trunk/stage_msvc/lib"
		CACHE
		FILEPATH 
		"Boost Stage Directory"
		)

	ELSE(WIN32)

		SET(
		BOOST_STAGE
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../Libraries/boost-trunk/stage_linux/lib"
		CACHE
		FILEPATH 
		"Boost Stage Directory"
		)

	ENDIF(WIN32)
ENDIF(CYGWIN)

SET(
TINYXMLDLL_LIB
"${CMAKE_CURRENT_SOURCE_DIR}/../../../tinyxmldll/out/"
CACHE
FILEPATH 
"TinyXMLDLL Library Directory"
) 

LINK_DIRECTORIES(
${TINYXMLDLL_LIB}
${BOOST_STAGE}
)

ADD_LIBRARY(
NEAT
SHARED 

src/NEAT_FastNetwork.cpp
src/NEAT_FastBiasNetwork.cpp
src/NEAT_GeneticGene.cpp
src/NEAT_GeneticGeneration.cpp
src/NEAT_GeneticIndividual.cpp
src/NEAT_GeneticLinkGene.cpp
src/NEAT_GeneticNodeGene.cpp
src/NEAT_GeneticPopulation.cpp
src/NEAT_GeneticSpecies.cpp
src/NEAT_Globals.cpp
src/NEAT_ModularNetwork.cpp
src/NEAT_Network.cpp
src/NEAT_NetworkLink.cpp
src/NEAT_NetworkNode.cpp
src/NEAT_Random.cpp
src/cRandom.cc

include/NEAT_FastNetwork.h
include/NEAT_FastBiasNetwork.h
include/NEAT_GeneticGene.h
include/NEAT_GeneticGeneration.h
include/NEAT_GeneticIndividual.h
include/NEAT_GeneticLinkGene.h
include/NEAT_GeneticNodeGene.h
include/NEAT_GeneticPopulation.h
include/NEAT_GeneticSpecies.h
include/NEAT_Globals.h
include/NEAT.h
include/NEAT_Defines.h
include/NEAT_ModularNetwork.h
include/NEAT_Network.h
include/NEAT_NetworkLink.h
include/NEAT_NetworkNode.h
include/NEAT_Random.h
include/NEAT_STL.h
..//Hypercube_NEAT/include/HCUBE_Defines.h
include/cRandom.h
)

SET_TARGET_PROPERTIES(NEAT PROPERTIES DEBUG_POSTFIX _d)

TARGET_LINK_LIBRARIES(
NEAT 

debug
tinyxmldll_d

optimized
tinyxmldll
)

