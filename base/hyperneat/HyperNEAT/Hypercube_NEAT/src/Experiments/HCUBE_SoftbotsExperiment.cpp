#include <iomanip> 
#include "Experiments/HCUBE_SoftbotsExperiment.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
//#include "DigitalMatter.h"
//#include "DM_FEA.h" 
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
//ACHEN: Included for temporary hack
#include <tinyxml.h> // #include “tinyxml.h"?

#ifdef VISUALIZESHAPES
//#include <SFML/Graphics.hpp>
#endif
#include <sstream>
#include <map>
#include <algorithm>
#include <queue>

#define SHAPES_EXPERIMENT_DEBUG (0)

namespace HCUBE
{
    using namespace NEAT;

    SoftbotsExperiment::SoftbotsExperiment(string _experimentName)
    :   Experiment(_experimentName)
    {
        
        cout << "Constructing experiment named SoftbotsExperiment.\n";
	
		convergence_step = int(NEAT::Globals::getSingleton()->getParameterValue("ConvergenceStep"));
		if(convergence_step < 0) convergence_step = INT_MAX; // never switch

		// create folders for vxa files of best in each be recorded by generation
		if (NEAT::Globals::getSingleton()->getParameterValue("SaveAllChampVXAs") > 0)
		{
    		int exitCode0 = std::system("mkdir champVXAs");
    	}

    	if (NEAT::Globals::getSingleton()->getParameterValue("PrintCPPNs") > 0) int exitCode0 = std::system("mkdir CPPNs");

    	if (NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxX") > 0)
    	{
    		NEAT::Globals::getSingleton()->setParameterValue("MaxTotalVoxels",NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxX")*NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxY")*NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxZ") );	
    	}
    }

    NEAT::GeneticPopulation* SoftbotsExperiment::createInitialPopulation(int populationSize)
    {
        NEAT::GeneticPopulation* population = new NEAT::GeneticPopulation();

        vector<GeneticNodeGene> genes;

        // Include a bias node (set to 0.3 by default)
        genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));
        
        // THESE ARE THE INPUT NODES TO THE CPPN (in this case x,y,z,d):
        genes.push_back(GeneticNodeGene("x","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("y","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("z","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("d","NetworkSensor",0,false)); // d = distnace from center

		// THESE ARE THE OUTPUT NODES FROM THE CPPN:
		genes.push_back(GeneticNodeGene("OutputMaterialOrEmpty","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		genes.push_back(GeneticNodeGene("OutputMuscleOrTissue","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		genes.push_back(GeneticNodeGene("OutputPassiveStiff","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		genes.push_back(GeneticNodeGene("OutputPassiveSoft","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		genes.push_back(GeneticNodeGene("OutputActivePhase1","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		genes.push_back(GeneticNodeGene("OutputActivePhase2","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
        
        for (size_t a=0;a<populationSize;a++)
        {
            shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,true,1.0));
            population->addIndividual(individual);
        }

        return population;
    }

    double SoftbotsExperiment::processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual)
    {
		fitness = 0.000001; // minimum fitness value (must be greater than zero)
		fitness2 = 1.1; // worst fitness value (higher is better, max is 1)
		individual->setOrigFitness(fitness); // just record keeping, so that original fitness can record distnace after the actual fitness has been adjusted by penalties
		individual->setFitness2(fitness2);

		// Create Phenotype from CPPN ===========================================================================================================================

		NEAT::FastNetwork <double> network = individual->spawnFastPhenotypeStack<double>(); // this is the CPPN network

		// INITIALIZATION:
		voxelSize = NEAT::Globals::getSingleton()->getParameterValue("VoxelSize");  // size of voxels in CPPN space

		queue<Coords> queueToCheck;

		int voxelsFilled = 0;
		int voxelsChecked = 0;

		std::map<Coords,int>::iterator it;

		std::map<Coords,int> cppnOutput;
		std::map<Coords,int> cppnZeros;
		
		Coords thisCoord(0,0,0);
		Coords newCoord(0,0,0);

		int thisMaterial;

		// start with voxel (0,0,0)
		queueToCheck.push(Coords(0,0,0));

		// while voxels filled < max and queue is not empty:
		while ( not queueToCheck.empty() and voxelsFilled < NEAT::Globals::getSingleton()->getParameterValue("MaxTotalVoxels") and voxelsChecked < NEAT::Globals::getSingleton()->getParameterValue("MaxVoxelsChecked"))
		{
			// get output of network with inputs popped from queue, assign it appropriate output
			thisCoord = queueToCheck.front();
			queueToCheck.pop();
			thisMaterial = getMaterialAtThisVoxel(network, double(thisCoord.x)*voxelSize, double(thisCoord.y)*voxelSize, double(thisCoord.z)*voxelSize);  // translate grid coordinate to physical location, then query CPPN for output material
			voxelsChecked++;

			// If the voxel is filled, assign material and check for neighbors
			// or: If there is no cell at the origin, search around it for a filled voxel instead of immediately giving up
			if (thisMaterial > 0 or voxelsFilled == 0)
			{
				if (thisMaterial > 0) // but only record voxel as filled if we are in loop because of material present
				{
					cppnOutput[thisCoord] = thisMaterial;
					voxelsFilled++;
				}

				// check up and down neighbors, if they haven't been checked yet, add them to the queue to be checked
				for (int i=-1; i<2; i=i+2)
				{
					newCoord = Coords(thisCoord.x, thisCoord.y, thisCoord.z + i);
					if ( cppnOutput.find(newCoord)==cppnOutput.end() and cppnZeros.find(newCoord)==cppnZeros.end() ) // if the voxel hasn't been checked yet
					{
						queueToCheck.push(newCoord);
						cppnZeros[newCoord] = -1; // placeholder, so items are not added to queue more than once
					}
				}

				// check forward and back neighbors, if they haven't been checked yet, add them to the queue to be checked
				for (int i=-1; i<2; i=i+2)
				{
					newCoord = Coords(thisCoord.x, thisCoord.y + i, thisCoord.z);
					if ( cppnOutput.find(newCoord)==cppnOutput.end() and cppnZeros.find(newCoord)==cppnZeros.end() ) // if the voxel hasn't been checked yet
					{
						queueToCheck.push(newCoord);
						cppnZeros[newCoord] = -1; // placeholder, so items are not added to queue more than once
					}
				}

				// check left and right neighbors, if they haven't been checked yet, add them to the queue to be checked
				for (int i=-1; i<2; i=i+2)
				{
					newCoord = Coords(thisCoord.x + i, thisCoord.y, thisCoord.z);
					if ( cppnOutput.find(newCoord)==cppnOutput.end() and cppnZeros.find(newCoord)==cppnZeros.end() ) // if the voxel hasn't been checked yet
					{
						queueToCheck.push(newCoord);
						cppnZeros[newCoord] = -1; // placeholder, so items are not added to queue more than once
					}
				}
			}
						
			// if the voxel is not filled, record that it has been checked, but don't add it to list of filled voxels or check for neighbors
			else
			{
				cppnZeros[thisCoord] = thisMaterial;
			}
		}

		// std::cout << "voxels filled  = " << voxelsFilled << std::endl;

		// If the robot has no cells, it doesn't exist, and thus is immediately assigned the minimum fitness. 
		if (voxelsFilled == 0 or voxelsFilled < (int)round(NEAT::Globals::getSingleton()->getParameterValue("MaxTotalVoxels") * NEAT::Globals::getSingleton()->getParameterValue("MinPercentVoxelsFilled")))
		{
			std::cout << "Only "<< voxelsFilled <<" voxels filled (" << (int)round(NEAT::Globals::getSingleton()->getParameterValue("MaxTotalVoxels") * NEAT::Globals::getSingleton()->getParameterValue("MinPercentVoxelsFilled")) << " required), returning minimum fitness." << std::endl;
			
			std::ostringstream moveToGenFolderCmd;
			char genBuffer[100];
			sprintf(genBuffer, "%04i", genNum);
			
			moveToGenFolderCmd  << "touch " 
								<< "Gen_" << genBuffer 
								<< "/" << NEAT::Globals::getSingleton()->getOutputFilePrefix() 
								<< "--Gen_" << genBuffer
								<< "--distFit_" << "0.00000000"
								<< "--voxelFit_" << "1.10000000"
								<< "--md5_" << "notEnoughVoxels" << individual
								<< ".vxa";

			int exitCode8 = std::system(moveToGenFolderCmd.str().c_str());

			return fitness;
		}

		// Turn this list of coordinates into an array (required for vxa format)
		// Start by finding minimum and maximum coordinate values to initialize array
		int minIndexX =  9999999;
		int maxIndexX = -9999999;
		int minIndexY =  9999999;
		int maxIndexY = -9999999;
		int minIndexZ =  9999999;
		int maxIndexZ = -9999999;

		for (std::map<Coords,int>::iterator it=cppnOutput.begin(); it!=cppnOutput.end(); ++it)
		{
    		// std::cout << it->first << " => " << it->second << '\n';
			minIndexX = std::min(float(minIndexX),it->first.x);
			maxIndexX = std::max(float(maxIndexX),it->first.x);

			minIndexY = std::min(float(minIndexY),it->first.y);
			maxIndexY = std::max(float(maxIndexY),it->first.y);

			minIndexZ = std::min(float(minIndexZ),it->first.z);
			maxIndexZ = std::max(float(maxIndexZ),it->first.z);
    	}

    	// Calculate array size (+1 for inclusive endpoints)
    	num_x_voxels = maxIndexX-minIndexX+1;
    	num_y_voxels = maxIndexY-minIndexY+1;
    	num_z_voxels = maxIndexZ-minIndexZ+1;

    	// create actual array, initialized with all zeros (empty voxels)
    	vector< vector< vector< int > > > matrixForVoxelyze (num_x_voxels, std::vector<std::vector<int> >(num_y_voxels, std::vector<int>(num_z_voxels, 0)));

    	// loop through coordinates to populate it with the cells we found above  
    	for (std::map<Coords,int>::iterator it=cppnOutput.begin(); it!=cppnOutput.end(); ++it)
		{
    		matrixForVoxelyze[it->first.x-minIndexX][it->first.y-minIndexY][it->first.z-minIndexZ] = it->second;
    	}

    	// create a vxa file describing our soft robot and environment (to be executed by the VoxCad GUI -- or underlying libraries, Voxelyze)
    	writeVoxelyzeFile(matrixForVoxelyze); 


    	// Get md5sum of vxa file, and keep on record as archive, so that future individuals can have fitnesses looked up instead of evlaluated if they share the same phenotype
    	FILE* pipe = popen("md5sum voxelyzeInputFromCPPN.vxa", "r");
		
		if (!pipe) {cout << "ERROR 1, exiting." << endl << endl; return 0.000001;}
		char buffer[128];
		std::string result = "";
		while(!feof(pipe)) 
		{
			if(fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
		pclose(pipe);

		std::string md5sumString = result.substr(0,32);
		// cout << "md5sum: " << md5sumString << endl;
		individual->setThismd5(md5sumString);
		cout << "Individual: " << md5sumString << endl;

		// look up if individual has been previously evaluated
		if (fitnessLookup.find(md5sumString) != fitnessLookup.end())
		{
			pair<double, double> fits = fitnessLookup[md5sumString];
			origFitness = fits.first; // before any fitness penalties
			fitness = origFitness;
			individual->setOrigFitness(origFitness);
			fitness2 = fits.second; // after any fitness penaltiesint exitCode0 = std::system("mkdir champVXAs");
			individual->setFitness2(fitness2);
			cout << "This individual was already evaluated!" << endl;

			moveFitnessFile(individual);
			
			// return fitness;
			return origFitness;
		}

		std::cout << "voxels filled: " << voxelsFilled << std::endl;
		std::cout << "container size: (" << num_x_voxels << ", " << num_y_voxels << ", " << num_y_voxels << ")" << std::endl;

		// Evaluate Individual =====================================================================================================================================

    	// just for timekeeping
		clock_t start;
		clock_t end;
		start = clock();
		// cout << "starting voxelyze evaluation now" << endl;

		// System call to voxelyze to run simulation -- make sure that executable is already built, and that it lives in the path specified here
		FILE* input = popen("./voxelyze -f voxelyzeInputFromCPPN.vxa","r");
		// FILE* input = popen(callVoxleyzeCmd.str().c_str(),"r");


		// Read Fitness File ===========================================================================================================================================

		// continually check for return of fitness file (non-optimized, for a cleaner approach pass with sockets)int exitCode0 = std::system("mkdir champVXAs");
		bool doneEval = false;
		while (not doneEval)
		{
			end = clock();
			std::ifstream infile("softbotsOutput.xml"); // this is the name of the fitness file created (as we specified it when writing the vxa)
			// std::ifstream infile(outFileName.str().c_str());
			
			// if file exists, note how long it took to complete, and exit this loop by switching flag
			if (infile.is_open())
			{
				printf("voxelyze took %.6lf seconds\n", float(end-start)/CLOCKS_PER_SEC);
				sleep(0.1);
				doneEval = true;
			}
			// if the file doesn't exist, wait for a small period of time and check for it again
			else
			{
				sleep(0.01);
				// if the file hasn't been found in a long period of time, voxelyze may have become unstable and crashed, kill the simulations and assign minimum fitness so the whole program doesn't crash/hang.  
				if ( double(end-start)/CLOCKS_PER_SEC > 300.0) // amount of time set arbitrarily.  For a more scalable value, make function of the number of voxels
				{
					cout << "voxelyze hung after 300 seconds... assigning fitness of 0.000001"<<endl;

					// find process and kill it.  Please optimize this better.
					int exitCode3 = std::system("ps axu > /tmp/HnPsFile.txt"); // write system processes to file
					std::ifstream psfile("/tmp/HnPsFile.txt"); // read them from that file
					std::string thisLine;
					if (psfile.is_open())
		  			{
						while (std::getline(psfile, thisLine))
						{
							std::size_t foundvox = thisLine.find("./voxelyze"); // if a thread is running voxelyze
							if (foundvox!=std::string::npos)
							{
								if (atoi(thisLine.substr(foundvox-8,4).c_str()) >= 2) // if it has been running longer that 2 minutes
								{
									std::size_t foundsp = thisLine.find(" ");
									cout << ("kill "+thisLine.substr(foundsp,11)).c_str() << endl;
									int exitCode4 = std::system(("kill "+thisLine.substr(foundsp,11)).c_str()); // kill it
									int exitCode5 = std::system("killall <defunct>"); // kill any other defunct processes while we are here (as this crude clean up method may cause them)
								}
							}
						}
					}

					cout << endl;
					return 0.000001; // if we kill the simulation, just return the minimum fitness
				}
			}
			infile.close();
		}

		pclose(input); // clean up the process running the simulation
		
		// open the fitness file again (you could also just move your pointer back to the beginning of the file)
		std::ifstream infile("softbotsOutput.xml"); // this is the name of the fitness file created (as we specified it when writing the vxa)
		// std::ifstream infile(outFileName.str().c_str());

		std::string line;
		float FinalCOM_DistX;
		float FinalCOM_DistY;
		float FinalCOM_DistZ;

		// parse the file line by line to find the fitness values you are looking for (in this case diatance moved).  this could also be done by parsing the xml tree
		if (infile.is_open())
		{
			while (std::getline(infile, line))
			{
			    std::size_t foundx = line.find("<FinalCOM_DistX>");
			    if (foundx!=std::string::npos)
			    {
			    	FinalCOM_DistX = atof(line.substr(foundx+strlen("<FinalCOM_DistX>"),line.find("</")-(foundx+strlen("<FinalCOM_DistX>"))).c_str());
			    }
			    std::size_t foundy = line.find("<FinalCOM_DistY>");
			    if (foundy!=std::string::npos)
			    {
			    	FinalCOM_DistY = atof(line.substr(foundy+strlen("<FinalCOM_DistY>"),line.find("</")-(foundy+strlen("<FinalCOM_DistY>"))).c_str());
			    }
			    std::size_t foundz = line.find("<FinalCOM_DistZ>");
			    if (foundz!=std::string::npos)
			    {
			    	FinalCOM_DistZ = atof(line.substr(foundz+strlen("<FinalCOM_DistZ>"),line.find("</")-(foundz+strlen("<FinalCOM_DistZ>"))).c_str());
			    }
			}

			fitness = pow(pow(FinalCOM_DistX,2)+pow(FinalCOM_DistY,2),0.5);  // in this example we are only looking for displacement in the X-Y plane
			
			if ( NEAT::Globals::getSingleton()->getParameterValue("FitnessNormalizedBySize") )
			{
				// normalize the fitness from an absolute distance measure to display in the unity of "body lengths" (the largest dimension of the array holding the robot)
				fitness = fitness/(max(num_x_voxels,max(num_y_voxels,num_z_voxels))*voxelSize);
			}
			else
			{
				// normalize the fitness by voxel size only
				fitness = fitness/voxelSize;
			}

			// cout << "Original Fitness from voxelyze: " << fitness << endl;

			infile.close();
		}

		// if fitness is not greater than zero or is an absurdly large number, something probably went wrong.  Just assign minimum fitness and exit the evaluation.
		if (fitness < 0.000001) return 0.000001;
		if (fitness > 10000) return 0.000001;

		// keep a record of the actual distance traveled before any penalties are applied too
		origFitness = fitness;
		individual->setOrigFitness(origFitness);

		// adjust fitness by the penalty factor
		// fitness = fitness * pow(1.0 - calculateFitnessAdjustment( matrixForVoxelyze )/NEAT::Globals::getSingleton()->getParameterValue("MaxTotalVoxels"),NEAT::Globals::getSingleton()->getParameterValue("PenaltyExp"));
		fitness2 = calculateFitnessAdjustment(matrixForVoxelyze) / NEAT::Globals::getSingleton()->getParameterValue("MaxTotalVoxels");
		individual->setFitness2(fitness2);

		if (fitness < 0.000001) fitness = 0.000001;
		if (fitness > 10000) fitness = 0.000001;

		pair <double, double> fits (origFitness, fitness2);	
		fitnessLookup[md5sumString]=fits;

		moveFitnessFile(individual);

		// cout << "Adjusted Fitness: " << fitness << endl;

		// return fitness; 
		return origFitness; 
    }
	

    void SoftbotsExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
    {
    	
    	// create folders for vxa files to be recorded by generation
    	genNum = generation->getGenerationNumber() + 1;
    	char thisGenBuffer [50];
		sprintf(thisGenBuffer, "%04i", genNum);
		std::ostringstream mkGenDir;
		std::ostringstream mkCPPNDir;
		if (genNum < NEAT::Globals::getSingleton()->getParameterValue("AlsoSaveFirstGens") or genNum % (int)NEAT::Globals::getSingleton()->getParameterValue("SaveVXAEvery")==0)
		{
			mkGenDir << "mkdir -p Gen_" << thisGenBuffer;
			int exitCode5 = std::system(mkGenDir.str().c_str());
			// PRINT(thisGenBuffer);
			if (int(NEAT::Globals::getSingleton()->getParameterValue("PrintCPPNs")))
			{	
				mkCPPNDir << "mkdir -p CPPNs/Gen_" << thisGenBuffer;
				int exitCode6 = std::system(mkCPPNDir.str().c_str());
			}
		}			
		// PRINT(mkGenDir.str().c_str());
		
		// =========================================================================================================
		// process each individual within the group

		// bestFitnessThisGen = -999;
		// setBestFitnessThisGen(-999);
		for(int z=0;z< group.size();z++)
		{
			shared_ptr<NEAT::GeneticIndividual> individual = group[z];
			double fitness = 0;
			
			fitness = processEvaluation(individual);
					
			if(fitness > std::numeric_limits<double>::max())
			{
				cout << "error: the max fitness is greater than the size of the double. " << endl;
				cout << "max double size is: : " << std::numeric_limits<double>::max() << endl;
				exit(88);
			} 
			if (fitness < 0)
			{
				cout << "Fitness Less Than Zero!!!, it is: " << fitness << "\n";  
				exit(10);
			}
			
			#if SHAPES_EXPERIMENT_DEBUG        
				cout << "Individual Evaluation complete!\n";
				printf("fitness: %f\n", fitness);	
			#endif

			cout << "Distance: " << fitness << endl;
			cout << "\% Voxels: " << individual->getFitness2() << endl;
			// PRINT(fitness);
			// PRINT(individual->getFitness2());
			// cout << "Adjusted Fitness: " << fitness << endl;
	  
			individual->reward(fitness);	

			// if (true)	// to print cppn
			// {
			// 	printNetworkCPPN(individual);    
			// }		

			cout << endl;				
		}

		// // copy the best vxa file from this generation to the archive
		// if (NEAT::Globals::getSingleton()->getParameterValue("SaveAllChampVXAs"))
		// {
		// 	int exitCode0 = std::system("mv thisGenChampVXA/* champVXAs");
		// }
    }

    void SoftbotsExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
    {
        cout << "Sorry, this functionality was never coded up." << endl;
        exit(6);
    }

    
    void SoftbotsExperiment::printNetworkCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
	    // cout << "Printing cppn network" << endl;
	    std::ostringstream cppnFileName;

	    char genBuffer[100];
	    sprintf(genBuffer, "%04i", genNum);
	    char fit1Buffer[100];
		sprintf(fit1Buffer, "%.8lf", fitness);
		char fit2Buffer[100];
		sprintf(fit2Buffer, "%.8lf", individual->getFitness2());

		cppnFileName << "CPPNs/Gen_" << genBuffer << "/cppnFor--Gen_" << genBuffer << "--distFit_" << fit1Buffer << "--voxelFit_" << fit2Buffer << "--md5_" << individual->getThismd5() << ".txt";
		ofstream network_file;        
		network_file.open (cppnFileName.str().c_str(), ios::trunc );

		NEAT::FastNetwork <double> network = individual->spawnFastPhenotypeStack <double>();        //JMC: this creates the network CPPN associated with this individual used to produce the substrate (neural network)

		network_file << "num links:" << network.getLinkCount() << endl;
		network_file << "num nodes:" << network.getNodeCount() << endl;

		int numLinks = network.getLinkCount();
		int numNodes = network.getNodeCount();
		ActivationFunction activationFunction;

		//print out which node corresponds to which integer (e.g. so you can translate a fromNode of 1 to "x1"  
		map<string,int> localNodeNameToIndex = *network.getNodeNameToIndex();
		for( map<string,int>::iterator iter = localNodeNameToIndex.begin(); iter != localNodeNameToIndex.end(); iter++ ) {
		network_file << (*iter).first << " is node number: " << (*iter).second << endl;
		}

		for (size_t a=0;a<numLinks;a++)
		{
		  NetworkIndexedLink <double> link = *network.getLink(a);
		   
		  network_file << link.fromNode << "->" << link.toNode << " : " << link.weight << endl;
		}
		for (size_t a=0;a<numNodes;a++)
		{          
		  activationFunction = *network.getActivationFunction(a);           
		  network_file << " activation function " << a << ": ";
		  if(activationFunction == ACTIVATION_FUNCTION_SIGMOID) network_file << "ACTIVATION_FUNCTION_SIGMOID";
		  if(activationFunction == ACTIVATION_FUNCTION_SIN) network_file << "ACTIVATION_FUNCTION_SIN";
		  if(activationFunction == ACTIVATION_FUNCTION_COS) network_file << "ACTIVATION_FUNCTION_COS";
		  if(activationFunction == ACTIVATION_FUNCTION_GAUSSIAN) network_file << "ACTIVATION_FUNCTION_GAUSSIAN";
		  if(activationFunction == ACTIVATION_FUNCTION_SQUARE) network_file << "ACTIVATION_FUNCTION_SQUARE";
		  if(activationFunction == ACTIVATION_FUNCTION_ABS_ROOT) network_file << "ACTIVATION_FUNCTION_ABS_ROOT";
		  if(activationFunction == ACTIVATION_FUNCTION_LINEAR) network_file << "ACTIVATION_FUNCTION_LINEAR";
		  if(activationFunction == ACTIVATION_FUNCTION_ONES_COMPLIMENT) network_file << "ACTIVATION_FUNCTION_ONES_COMPLIMENT";
		  if(activationFunction == ACTIVATION_FUNCTION_END) network_file << "ACTIVATION_FUNCTION_END";
		  network_file << endl;
		}

		network_file.close();

		return;
    }

    Experiment* SoftbotsExperiment::clone()
    {
        SoftbotsExperiment* experiment = new SoftbotsExperiment(*this);
        return experiment;
    }
 
	
	bool SoftbotsExperiment::converged(int generation) {
		if(generation == convergence_step) { return true; }
		return false;
	}		

	bool SoftbotsExperiment::outOfBoundingBox(double x, double y, double z) {
		// check if voxel is out of bounding box parameter form Softbots.dat.  -1 is to account for the voxel at 0,0,0
		if ( NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxX") > 0)
		{
			if ( x < -(NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxX")-1)/2*voxelSize) { return true; }
			if ( x > NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxX")/2*voxelSize) { return true; }
		}
		if ( NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxY") > 0)
		{
			if ( y < -(NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxY")-1)/2*voxelSize) { return true; }
			if ( y > NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxY")/2*voxelSize) { return true; }
		}
		if ( NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxZ") > 0)
		{
			if ( z < -(NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxZ")-1)/2*voxelSize) { return true; }
			if ( z > NEAT::Globals::getSingleton()->getParameterValue("BoundingBoxZ")/2*voxelSize) { return true; }
		}
		return false;
	}		

	int SoftbotsExperiment::getMaterialAtThisVoxel(NEAT::FastNetwork <double> network, double x, double y, double z)
	{	
		network.reinitialize();	//reset CPPN
		network.setValue("x",x); //set the input values...
		network.setValue("y",y);
		network.setValue("z",z);
		network.setValue("d", sqrt(pow(double(x),2.0)+pow(double(y),2.0)+pow(double(z),2.0)) );
		network.setValue("Bias",0.3);                       
						
		network.update(); //run the CPPN network
		
		// convert output from CPPN to material types
		// material numbers are specified when we write the vxa file below, but as default (4 material sinusodial activation) they are set as:
		// 0: Empty Cell
		// 1: Passive_Soft
		// 2: Passive_Hard
		// 3: Active_+
		// 4: Active_-
		if ( outOfBoundingBox(x,y,z) ) {return 0;}
		if ( network.getValue("OutputMaterialOrEmpty") < 0) { return 0; } 
		else 
		{
			if ( network.getValue("OutputMuscleOrTissue") > 0)
			{
				if ( network.getValue("OutputPassiveSoft") > network.getValue("OutputPassiveStiff") ) { return 1; }
				else { return 2; }
			}
			else
			{
				if ( network.getValue("OutputActivePhase1") > network.getValue("OutputActivePhase2") ) { return 3; }
				else { return 4; }
			}
		}
	}

	double SoftbotsExperiment::calculateFitnessAdjustment( vector< vector< vector< int > > > matrixForVoxelyze )
	{
		// Penalty Type 0 == no penalty, just return a fitness adjustment factor of 1.0 to keep fitness the same
		if (int(NEAT::Globals::getSingleton()->getParameterValue("PenaltyType")) == 0) { return 1.0; }

		int penaltyCounter = 0;

		for (int z=0; z<num_z_voxels; z++)
		{
			for (int y=0; y<num_y_voxels; y++)
			{
				for (int x=0; x<num_x_voxels; x++) 
				{
					// Penalty Type 1 == cost for total number of voxels:
					if (int(NEAT::Globals::getSingleton()->getParameterValue("PenaltyType")) == 1)
					{
						if ( matrixForVoxelyze[x][y][z] != 0) {penaltyCounter++;}
					} 
					else 
					{
						// Penalty Type 1 == cost for number of actuated voxels (muscle cells only):
						if (int(NEAT::Globals::getSingleton()->getParameterValue("PenaltyType")) == 2)
						{
							if ( matrixForVoxelyze[x][y][z] == 3 or matrixForVoxelyze[x][y][z] == 4 ) {penaltyCounter++;}
						} 
						else
						{
							cout << "Penalty Type not supported, please double check SoftbotsExperiment.dat file" << endl; exit(0);
						}						
					}
				}
			}
		}

		// return pow(1.0 - penaltyCounter/NEAT::Globals::getSingleton()->getParameterValue("MaxTotalVoxels"),NEAT::Globals::getSingleton()->getParameterValue("PenaltyExp"));
		return (double)penaltyCounter;
	}

	void SoftbotsExperiment::moveFitnessFile( shared_ptr<const NEAT::GeneticIndividual> individual )
	{
		std::ostringstream moveToGenFolderCmd;
		std::ostringstream moveToChampFolderCmd;

		char genBuffer[100];
		sprintf(genBuffer, "%04i", genNum);
	    char fit1Buffer[100];
		sprintf(fit1Buffer, "%.8lf", fitness);
		char fit2Buffer[100];
		sprintf(fit2Buffer, "%.8lf", individual->getFitness2());

		if (NEAT::Globals::getSingleton()->getParameterValue("SaveAllChampVXAs") and fitness > getBestFitnessThisGen())
		{
			// bestFitnessThisGen = fitness;
			setBestFitnessThisGen(fitness);
			cout << "bestFitnessThisGen: " << bestFitnessThisGen << endl;
			// remove old champ vxa
			// int exitCode3 = std::system("rm thisGenChampVXA/*.vxa");
			// copy vxa to champ folder
			moveToChampFolderCmd<< "cp voxelyzeInputFromCPPN.vxa " 
								<< "champVXAs"
								<< "/" << NEAT::Globals::getSingleton()->getOutputFilePrefix() 
								<< "--Gen_" << genBuffer
								<< "--distFit_" << fit1Buffer
								<< "--voxelFit_" << fit2Buffer
								<< "--md5_" << individual->getThismd5()
								<< ".vxa";

			int exitCode3 = std::system(moveToChampFolderCmd.str().c_str());
		}

		if (genNum < NEAT::Globals::getSingleton()->getParameterValue("AlsoSaveFirstGens") or genNum % (int)NEAT::Globals::getSingleton()->getParameterValue("SaveVXAEvery")==0)
		{	
			// move vxa to generation folder
			moveToGenFolderCmd  << "mv voxelyzeInputFromCPPN.vxa " 
								<< "Gen_" << genBuffer 
								<< "/" << NEAT::Globals::getSingleton()->getOutputFilePrefix() 
								<< "--Gen_" << genBuffer
								<< "--distFit_" << fit1Buffer
								<< "--voxelFit_" << fit2Buffer
								<< "--md5_" << individual->getThismd5()
								<< ".vxa";

			int exitCode3 = std::system(moveToGenFolderCmd.str().c_str());

			// to print cppn
			if (int(NEAT::Globals::getSingleton()->getParameterValue("PrintCPPNs"))) {	printNetworkCPPN(individual); }
		} 
		else
		{
			// remove gen folder
			int exitCode3 = std::system("rm voxelyzeInputFromCPPN.vxa");
		}

		//delete fitness file
		int exitCode4 = std::system("rm -f softbotsOutput.xml");  
		
	}

    void SoftbotsExperiment::writeVoxelyzeFile( vector< vector< vector< int > > > matrixForVoxelyze )
	{
		
  		ofstream myfile;
  		myfile.open ("voxelyzeInputFromCPPN.vxa");
  		myfile << "\
<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n\
<VXA Version=\"1.0\">\n\
<Simulator>\n\
<Integration>\n\
<Integrator>0</Integrator>\n\
<DtFrac>0.9</DtFrac>\n\
</Integration>\n\
<Damping>\n\
<BondDampingZ>1</BondDampingZ>\n\
<ColDampingZ>0.8</ColDampingZ>\n\
<SlowDampingZ>0.01</SlowDampingZ>\n\
</Damping>\n\
<Collisions>\n\
<SelfColEnabled>1</SelfColEnabled>\n\
<ColSystem>3</ColSystem>\n\
<CollisionHorizon>2</CollisionHorizon>\n\
</Collisions>\n\
<Features>\n\
<FluidDampEnabled>0</FluidDampEnabled>\n\
<PoissonKickBackEnabled>0</PoissonKickBackEnabled>\n\
<EnforceLatticeEnabled>0</EnforceLatticeEnabled>\n\
</Features>\n\
<SurfMesh>\n\
<CMesh>\n\
<DrawSmooth>1</DrawSmooth>\n\
<Vertices/>\n\
<Facets/>\n\
<Lines/>\n\
</CMesh>\n\
</SurfMesh>\n\
<StopCondition>\n\
<StopConditionType>2</StopConditionType>\n\
<StopConditionValue>" << float(NEAT::Globals::getSingleton()->getParameterValue("NumActuationCycles"))/float(NEAT::Globals::getSingleton()->getParameterValue("ActuationsPerSecond")) /* from <TempPeriod> */ << "</StopConditionValue>\n\
<InitCmTime>" << float(NEAT::Globals::getSingleton()->getParameterValue("InitCmCycles"))/float(NEAT::Globals::getSingleton()->getParameterValue("ActuationsPerSecond")) /* from <TempPeriod> */ << "</InitCmTime>\n\
</StopCondition>\n\
<GA>\n\
<WriteFitnessFile>1</WriteFitnessFile>\n\
<FitnessFileName>softbotsOutput.xml</FitnessFileName>\n\
</GA>\n\
</Simulator>\n\
<Environment>\n\
<Fixed_Regions>\n\
<NumFixed>0</NumFixed>\n\
</Fixed_Regions>\n\
<Forced_Regions>\n\
<NumForced>0</NumForced>\n\
</Forced_Regions>\n\
<Gravity>\n\
<GravEnabled>1</GravEnabled>\n\
<GravAcc>-9.81</GravAcc>\n\
<FloorEnabled>1</FloorEnabled>\n\
</Gravity>\n\
<Thermal>\n\
<TempEnabled>1</TempEnabled>\n\
<TempAmp>39</TempAmp>\n\
<TempBase>25</TempBase>\n\
<VaryTempEnabled>1</VaryTempEnabled>\n\
<TempPeriod>" << 1.0/float(NEAT::Globals::getSingleton()->getParameterValue("ActuationsPerSecond")) << "</TempPeriod>\n\
</Thermal>\n\
</Environment>\n\
<VXC Version=\"0.93\">\n\
<Lattice>\n\
<Lattice_Dim>" << voxelSize << "</Lattice_Dim>\n\
<X_Dim_Adj>1</X_Dim_Adj>\n\
<Y_Dim_Adj>1</Y_Dim_Adj>\n\
<Z_Dim_Adj>1</Z_Dim_Adj>\n\
<X_Line_Offset>0</X_Line_Offset>\n\
<Y_Line_Offset>0</Y_Line_Offset>\n\
<X_Layer_Offset>0</X_Layer_Offset>\n\
<Y_Layer_Offset>0</Y_Layer_Offset>\n\
</Lattice>\n\
<Voxel>\n\
<Vox_Name>BOX</Vox_Name>\n\
<X_Squeeze>1</X_Squeeze>\n\
<Y_Squeeze>1</Y_Squeeze>\n\
<Z_Squeeze>1</Z_Squeeze>\n\
</Voxel>\n\
<Palette>\n\
<Material ID=\"1\">\n\
<MatType>0</MatType>\n\
<Name>Passive_Soft</Name>\n\
<Display>\n\
<Red>0</Red>\n\
<Green>0.57</Green>\n\
<Blue>0.44</Blue>\n\
<Alpha>1</Alpha>\n\
</Display>\n\
<Mechanical>\n\
<MatModel>0</MatModel>\n\
<Elastic_Mod>1e+007</Elastic_Mod>\n\
<Plastic_Mod>0</Plastic_Mod>\n\
<Yield_Stress>0</Yield_Stress>\n\
<FailModel>0</FailModel>\n\
<Fail_Stress>0</Fail_Stress>\n\
<Fail_Strain>0</Fail_Strain>\n\
<Density>1e+006</Density>\n\
<Poissons_Ratio>0.35</Poissons_Ratio>\n\
<CTE>0</CTE>\n\
<uStatic>1</uStatic>\n\
<uDynamic>0.5</uDynamic>\n\
</Mechanical>\n\
</Material>\n\
<Material ID=\"2\">\n\
<MatType>0</MatType>\n\
<Name>Passive_Hard</Name>\n\
<Display>\n\
<Red>0</Red>\n\
<Green>0.082</Green>\n\
<Blue>0.819</Blue>\n\
<Alpha>1</Alpha>\n\
</Display>\n\
<Mechanical>\n\
<MatModel>0</MatModel>\n\
<Elastic_Mod>5e+007</Elastic_Mod>\n\
<Plastic_Mod>0</Plastic_Mod>\n\
<Yield_Stress>0</Yield_Stress>\n\
<FailModel>0</FailModel>\n\
<Fail_Stress>0</Fail_Stress>\n\
<Fail_Strain>0</Fail_Strain>\n\
<Density>1e+006</Density>\n\
<Poissons_Ratio>0.35</Poissons_Ratio>\n\
<CTE>0</CTE>\n\
<uStatic>1</uStatic>\n\
<uDynamic>0.5</uDynamic>\n\
</Mechanical>\n\
</Material>\n\
<Material ID=\"3\">\n\
<MatType>0</MatType>\n\
<Name>Active_+</Name>\n\
<Display>\n\
<Red>1</Red>\n\
<Green>0</Green>\n\
<Blue>0</Blue>\n\
<Alpha>1</Alpha>\n\
</Display>\n\
<Mechanical>\n\
<MatModel>0</MatModel>\n\
<Elastic_Mod>1e+007</Elastic_Mod>\n\
<Plastic_Mod>0</Plastic_Mod>\n\
<Yield_Stress>0</Yield_Stress>\n\
<FailModel>0</FailModel>\n\
<Fail_Stress>0</Fail_Stress>\n\
<Fail_Strain>0</Fail_Strain>\n\
<Density>1e+006</Density>\n\
<Poissons_Ratio>0.35</Poissons_Ratio>\n\
<CTE>0.01</CTE>\n\
<uStatic>1</uStatic>\n\
<uDynamic>0.5</uDynamic>\n\
</Mechanical>\n\
</Material>\n\
<Material ID=\"4\">\n\
<MatType>0</MatType>\n\
<Name>Active_-</Name>\n\
<Display>\n\
<Red>0</Red>\n\
<Green>1</Green>\n\
<Blue>0</Blue>\n\
<Alpha>1</Alpha>\n\
</Display>\n\
<Mechanical>\n\
<MatModel>0</MatModel>\n\
<Elastic_Mod>1e+007</Elastic_Mod>\n\
<Plastic_Mod>0</Plastic_Mod>\n\
<Yield_Stress>0</Yield_Stress>\n\
<FailModel>0</FailModel>\n\
<Fail_Stress>0</Fail_Stress>\n\
<Fail_Strain>0</Fail_Strain>\n\
<Density>1e+006</Density>\n\
<Poissons_Ratio>0.35</Poissons_Ratio>\n\
<CTE>-0.01</CTE>\n\
<uStatic>1</uStatic>\n\
<uDynamic>0.5</uDynamic>\n\
</Mechanical>\n\
</Material>\n\
</Palette>\n\
<Structure Compression=\"ASCII_READABLE\">\n\
<X_Voxels>" << num_x_voxels << "</X_Voxels>\n\
<Y_Voxels>" << num_y_voxels << "</Y_Voxels>\n\
<Z_Voxels>" << num_z_voxels << "</Z_Voxels>\n\
<Data>\n\
<Layer><![CDATA[";

// write actual voxel array:
for (int z=0; z<num_z_voxels; z++)
{
	for (int y=0; y<num_y_voxels; y++)
	{
		for (int x=0; x<num_x_voxels; x++) 
		{
			myfile << matrixForVoxelyze[x][y][z];
		}
	}

	// line is this format are slides in the XY plane (concatenated rows in the Y direction), so the following line is for proper formatting:
	if (z<num_z_voxels-1) myfile << "]]></Layer>\n<Layer><![CDATA[";
}

		myfile << "]]></Layer>\n\
</Data>\n\
</Structure>\n\
</VXC>\n\
</VXA>";

  		myfile.close();
	}		


}
