+============================================================================+
| Softbot Evolution                                                          |
| Nick Cheney                                                                |
| contact: nac93@cornell.edu, http://www.ncheney.com/contact	             |
| 2014-08-25								     |
|                                                                            |
| please cite the following paper when using this code:                      |
|                                                                            |
| Cheney, N., MacCurdy, R., Clune, J., & Lipson, H. (2013).                  |
| Unshackling evolution: evolving soft robots with multiple materials        |
| and a powerful generative encoding. In Proceeding of the Fifteenth Annual  |
| Conference on Genetic and Evolutionary Computation (pp. 167-174). ACM.     |
+============================================================================+

+=================================================+
| Legend:                                         |
| [rootdir] = the folder containing this document |
| $ = command line entry                          |
| -> = GUI direction                              |
| () = comment                                    |
+=================================================+

*** NOTE:  These instructions were made on a Linux system running Ubuntu ***
           and gcc version 4.6.3

           These instructions are a work in progress, if you have errors, 
           please contact me so that I can help find a solution and update 
           the instructions for future users.  Thank you!


Step 1: Installing HyperNEAT
----------------------------
(Install Boost and CMake if you have not already done so)

1) find where Boost lives and update the build script with that information:
$ cd /usr
$ find * | grep libboost_filesystem.a
(note the path leading up to "libboost_filesystem.a", and set it as "boost_stage" in:)
$ cd [rootdir]/hyperneat
$ nano BUILD_EVERYTHING.sh
(note: to build on more than one computer, you may need to make and rename copies of this file)

2) build HyperNEAT
$ ./BUILD_CLEAN.sh
$ ./BUILD_EVERYTHING.sh
(the HyperNEAT executable should appear in the folder: [rootdir]/hyperneat/out )
(note: in some circumstances you may need to run "$ ./BUILD_CLEAN" first if you need to clear your cache of built files)

Step 2: Installing VoxCad
-------------------------
(Install Qt and QMake if you have not already done so)

$ cd [rootdir]/voxcad-code-2014
$ make
(if you get the error: "make: *** No rule to make target..." , run $qmake, then $make)
(NOTE: You can skip this step and install the Voxelyze library only for simulation without visualization) 

Step 3: Installing Voxelyze
---------------------------
1) install voxelyze library:
$ cd [rootdir]/voxcad-code-2014/Voxelyze
$ make
$ make installusr      (or sudo make installglobal)

2) make voxelyze executable:
$ cd [rootdir]/voxelyzeMain
$ make


Step 4: Test Execution of the Simulators
----------------------------------------
1) running Example_1.vxa with VoxCad GUI:
$ cd [rootdir]/voxcad-code-2014/release
$ ./VoxCad
-> File -> Import -> Simulation -> [rootdir]/voxelyzeMain/Example_1.vxa
-> Physics Sandbox   (5th icon from the right)

2) running Example_1.vxa with voxelyze (no GUI):
$ cd [rootdir]/voxelyzeMain
$ ./voxelyze -f Example_1.vxa -p   (-p flag = print simulation status to terminal, this flag is removed when called by HyperNEAT)
(note the presence of fitness file "Change_Me_output.xml" after simulation has finished)


Step 5: Running Softbot Evolution with HyperNEAT
------------------------------------------------
1) create a folder for you run:
$ cd [rootdir]
$ mkdir [runName]
$ cp hyperneat/out/Hypercube_NEAT [runName]
$ cp hyperneat/out/SoftbotsExperiment.dat [runName]
$ cp voxelyzeMain/voxelyze [runName]
(note: this is a good time to copy any files you have made changes to, such as HCUBE_SoftbotsExperiment.cpp, here for record keeping)
$ cd [runName]

2) make changes to (or double check) your SoftbotsExperiment.dat config file 
$ nano SoftbotsExperiment.dat
(examples of things that affect runtime and you may want to change are:)
* PopulationSize
* MaxGenerations

3) execute a run:
$ ./HyperNEAT -I SoftbotsExperiment.dat -O [runName] -R [randomSeed]
(note: to track fitness over time, look in the file named "gen-Genchamp-AvgFit.txt")
(to view the evolved robots, simply import them as a simulation into the VoxCad GUI)


Step 6: Making Changes
----------------------
Some common files that you might want to change at some point are: 

* hyperneat/HyperNEAT/Hypercube_NEAT/src/Experiments/HCUBE_SoftbotsExperiment.cpp  
	(describes the input and output nodes for the CPPN -- in "createInitialPopulation" )
	(also describes the creation and evaluation of the soft robots -- in "processEvaluation")

* voxelyzeMain/main.cpp
	(calls the Voxelyze library to run the simulation -- useful for adding print statements under debug/verbose flag)

* voxcad-code-2014/Voxelyze/VX_SimGA.cpp
	(describes the information written to the fitness return file -- in "WriteResultFile")

* voxcad-code-2014/Voxelyze/VXS_Voxel.cpp
	(describes the physics acting on each voxel -- especially in the "EulerStep")

In general:
hyperneat/HyperNEAT/Hypercube_NEAT/src/Experiments/HCUBE_SoftbotsExperiment.cpp:   	describes the setup and execution of the experiment (evaluation of individuals)
softbotsExperiment.dat:									describes the configuration of parameters for evolution
hyperneat/HyperNEAT/NEAT:								describes the evolutionary algorithm
voxcad-code-2014/Voxelyze:								describes the soft body physics and execution of the fitness evaluation
