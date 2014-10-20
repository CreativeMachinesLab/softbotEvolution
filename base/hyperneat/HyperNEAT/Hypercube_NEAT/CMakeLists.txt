Project(NEAT)

ADD_DEFINITIONS(
		-DTIXML_USE_STL
		-DNOPCH
		-D_USE_MATH_DEFINES
		-pthread
		-DONETHREADual
		#-DINTERACTIVELYEVOLVINGSHAPES
		-DVISUALIZESHAPES
		)

IF(BOOSTOLD)
        ADD_DEFINITIONS(-DBOOSTOLD)
ENDIF(BOOSTOLD)

IF(COMPILE_ODE)
	ADD_DEFINITIONS(-DCOMPILE_ODE)
ENDIF(COMPILE_ODE)

IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	ADD_DEFINITIONS(-fPIC)
ENDIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)

INCLUDE_REGULAR_EXPRESSION( "^.*([Hh][Cc][Uu][Bb][Ee]|[Nn][Ee][Aa][Tt]|main).*$" )

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

find_package( Boost 1.34 COMPONENTS filesystem system thread )

message(STATUS "Boost_LIBRARIES=${Boost_LIBRARIES}")
message(STATUS "Boost_LIBRARY_DIRS=${Boost_LIBRARY_DIRS}")
message(STATUS "Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")

link_directories ( ${Boost_LIBRARY_DIRS} )

INCLUDE_DIRECTORIES(
	include
	src/hiller
	../NEAT/include
	${TINYXMLDLL_INCLUDE}
	${BOOST_ROOT}
	${Boost_INCLUDE_DIRS}
	${JGTL_INCLUDE}
	${ZLIB_INCLUDE}
	${ODE_INCLUDE}
	${SFML_INCLUDE}
	)

LINK_DIRECTORIES(
	${TINYXMLDLL_LIB}
	${BOOST_STAGE}
	${LIBRARY_OUTPUT_PATH}
	${ODE_LIB}
	${SFML_LIB}
	)

ADD_LIBRARY(
	Hypercube_NEAT_Base
	STATIC

	src/HCUBE_Defines.cpp
	src/HCUBE_ExperimentRun.cpp
	src/HCUBE_EvaluationSet.cpp

	src/hiller/Array3D.cpp
	src/hiller/Array3D.h
	src/hiller/DigitalMatter.cpp
	src/hiller/DigitalMatter.h
	src/hiller/MarchCube.cpp
	src/hiller/MarchCube.h
	src/hiller/Mesh.h
	src/hiller/Mesh.cpp
	src/hiller/XML_Rip.cpp
	src/hiller/XML_Rip.h

	src/Experiments/HCUBE_FindClusterExperiment.cpp
	src/Experiments/HCUBE_FindClusterNoGeomExperiment.cpp
	src/Experiments/HCUBE_LegSwingExperiment.cpp
	src/Experiments/HCUBE_BitMirroringV2Experiment.cpp
	src/Experiments/HCUBE_BitMirroringV2NoGeomExperiment.cpp
	src/Experiments/HCUBE_LegSwingExperiment-Pneat.cpp
	src/Experiments/HCUBE_LegSwingExperiment-NEAT.cpp
	src/Experiments/HCUBE_TargetWeightsExperiment.cpp
	src/Experiments/HCUBE_TargetWeightsNoGeomExperiment.cpp
	src/Experiments/HCUBE_ShapesExperiment.cpp
	src/Experiments/HCUBE_SoftbotsExperiment.cpp
	src/Experiments/HCUBE_SpiderRobotExperiment.cpp
	src/Experiments/HCUBE_Retina-FTNEAT.cpp	
	src/Experiments/HCUBE_RetinaExperiment.cpp	
	src/HCUBE_ODE.cpp 

	include/HCUBE_Boost.h
	include/HCUBE_Defines.h
	include/HCUBE_EvaluationSet.h
	include/HCUBE_ExperimentRun.h
	include/HCUBE_STL.h

	include/Experiments/HCUBE_Experiment.h
	include/Experiments/HCUBE_FindClusterExperiment.h
	include/Experiments/HCUBE_FindClusterNoGeomExperiment.h
	include/Experiments/HCUBE_LegSwingExperiment.h
	include/Experiments/HCUBE_LegSwingExperiment-NEAT.h
	include/Experiments/HCUBE_TargetWeightsExperiment.h
	include/Experiments/HCUBE_TargetWeightsNoGeomExperiment.h
	include/Experiments/HCUBE_BitMirroringV2Experiment.h
	include/Experiments/HCUBE_BitMirroringV2NoGeomExperiment.h
	include/Experiments/HCUBE_ShapesExperiment.h
	include/Experiments/HCUBE_SoftbotsExperiment.h

	include/Experiments/HCUBE_SpiderRobotExperiment.h
	include/Experiments/HCUBE_Retina-FTNEAT.h
	include/Experiments/HCUBE_RetinaExperiment.h

	include/HCUBE_ODE.h
	)

ADD_DEPENDENCIES(Hypercube_NEAT_Base NEAT)

SET_TARGET_PROPERTIES(Hypercube_NEAT_Base PROPERTIES DEBUG_POSTFIX _d)

ADD_EXECUTABLE(
	Hypercube_NEAT
	src/main.cpp
	)

SET_TARGET_PROPERTIES(Hypercube_NEAT PROPERTIES DEBUG_POSTFIX _d)


TARGET_LINK_LIBRARIES(
		

			Hypercube_NEAT 
			
			debug
			Hypercube_NEAT_Base_d

			optimized
			Hypercube_NEAT_Base

			debug
			tinyxmldll_d

			optimized
			tinyxmldll

			debug
			NEAT_d

			optimized
			NEAT

			${Boost_LIBRARIES}

			#sfml-audio
			#sfml-graphics
			#sfml-network
			#sfml-system
			#sfml-window

			glut
			GLU

			${ToODEorNot}
			)


ADD_DEPENDENCIES(Hypercube_NEAT NEAT Hypercube_NEAT_Base)

