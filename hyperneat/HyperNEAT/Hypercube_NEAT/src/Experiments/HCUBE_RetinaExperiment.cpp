/*
 *  retina_ecperiment.cpp
 *  modularity
 *
 *  Created by Benjamin Beckmann on 11/16/09.
 *  Copyright 2009 Michigan State University. All rights reserved.
 *
 */


/*
 *  RetinaExperiment.cpp
 *  v2Hyperneat
 *
 *  Created by Benjamin Beckmann on 8/12/09.
 *  Copyright 2009 Michigan State University. All rights reserved.
 *
 */

#include "Experiments/HCUBE_RetinaExperiment.h"

//#define POSTHOC_RECORD
//#define LEGSWING_INTERACTIVE_EVOLUTION
#define EUCLIDEAN_DISTANCE_FITNESS
//#define DOUBLE
#undef VISUALIZATION_ENABLED  // this removes off all drawstuff function calls
#define dDOUBLE //using ode double precision  

//#define REMOVE_SIGNALS  // turns off signal TX, all RX set to zero.


#include <algorithm>
#include <stdexcept>
#include <assert.h>
#include <boost/shared_ptr.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/timer.hpp>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <string>
#include <vector>


namespace HCUBE
{
	
	using namespace NEAT;
	
	RetinaExperiment::RetinaExperiment(string _experimentName)
	:	Experiment(_experimentName),
	NUM_SHEETS(6)
	{		
		numEnvironments = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("NumberOfEnvironments"));
		swithEnviromentStep = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("SwithEnviromentStep"));
		convergence_step = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("ConvergenceStep"));
		popSize = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("PopulationSize"));
		
		if(convergence_step < 0) {
			convergence_step = INT_MAX; // never switch
		}
		currentEnvironment = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("StartEnvironment"));
		timeInCurrentEnv = 0;
		
		generateSubstrate();
	}
	
	
	NEAT::GeneticPopulation* RetinaExperiment::createInitialPopulation(int populationSize) {
		GeneticPopulation *population = new GeneticPopulation();
		vector<GeneticNodeGene> genes;
		
		genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));  //?
		genes.push_back(GeneticNodeGene("sourceX","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("sourceY","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("sourceZ","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("targetX","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("targetY","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("targetZ","NetworkSensor",0,false));
		
		genes.push_back(GeneticNodeGene("output","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		
		for (int a=0;a<populationSize;a++)
		{
			shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,true,1.0));
			
			for (int b=0;b<0;b++)  //  TODO: this loop will never be entered?  Why is it here?
			{
				individual->testMutate();
			}
			
			population->addIndividual(individual);
		}
		
		cout << "Finished creating population\n";
		return population;  // TODO: comment out to increase speed.  Required for hybrid
	}
	
	void RetinaExperiment::generateSubstrate()
	{
		cout << "Generating substrate...";
		boost::object_pool<NEAT::NetworkNode> networkNodePool;
		boost::object_pool<NEAT::NetworkLink> networkLinkPool;
		
		vector<NEAT::NetworkNode *> nodes;
		vector<NEAT::NetworkLink *> links;
		
		nameLookup.clear();
		
		cout << "Creating nodes and links...\n";
		
		// INPUT (sheet 4)
		// lookup names are in the form x,y,sheet; relitive to top left node in retina
		nameLookup["004"] = new NetworkNode("LeftTopLeft");
		nodes.push_back(nameLookup["004"]);
		nameLookup["014"] = new NetworkNode("LeftBottomLeft");
		nodes.push_back(nameLookup["014"]);
		nameLookup["104"] = new NetworkNode("LeftTopRight");
		nodes.push_back(nameLookup["104"]);
		nameLookup["114"] = new NetworkNode("LeftBottomRight");
		nodes.push_back(nameLookup["114"]);
		nameLookup["204"] = new NetworkNode("RightTopLeft");
		nodes.push_back(nameLookup["204"]);
		nameLookup["214"] = new NetworkNode("RightBottomLeft");
		nodes.push_back(nameLookup["214"]);
		nameLookup["304"] = new NetworkNode("RightTopRight");
		nodes.push_back(nameLookup["304"]);
		nameLookup["314"] = new NetworkNode("RightBottomRight");
		nodes.push_back(nameLookup["314"]);
		
		
		// HIDDEN (sheet 3)
		nameLookup["003"] = new NetworkNode("003");
		nodes.push_back(nameLookup["003"]);
		nameLookup["013"] = new NetworkNode("013");
		nodes.push_back(nameLookup["013"]);
		nameLookup["103"] = new NetworkNode("103");
		nodes.push_back(nameLookup["103"]);
		nameLookup["113"] = new NetworkNode("113");
		nodes.push_back(nameLookup["113"]);
		nameLookup["203"] = new NetworkNode("203");
		nodes.push_back(nameLookup["203"]);
		nameLookup["213"] = new NetworkNode("213");
		nodes.push_back(nameLookup["213"]);
		nameLookup["303"] = new NetworkNode("303");
		nodes.push_back(nameLookup["303"]);
		nameLookup["313"] = new NetworkNode("313");
		nodes.push_back(nameLookup["313"]);
		
		// link sheet 4 to sheet 3
		for (int sourceNode = 0; sourceNode <= 7; ++sourceNode) {
			for (int targetNode = 8; targetNode <= 15; ++targetNode) {
				links.push_back(new NetworkLink(nodes[sourceNode], nodes[targetNode], 0.0));
			}
		}
		
		// HIDDEN (sheet 2)
		nameLookup["002"] = new NetworkNode("002");
		nodes.push_back(nameLookup["002"]);
		nameLookup["102"] = new NetworkNode("102");
		nodes.push_back(nameLookup["102"]);
		nameLookup["202"] = new NetworkNode("202");
		nodes.push_back(nameLookup["202"]);
		nameLookup["302"] = new NetworkNode("302");
		nodes.push_back(nameLookup["302"]);
		
		// link sheet 3 to sheet 2
		for (int sourceNode = 8; sourceNode <= 15; ++sourceNode) {
			for (int targetNode = 16; targetNode <= 19; ++targetNode) {
				links.push_back(new NetworkLink(nodes[sourceNode], nodes[targetNode], 0.0));
			}
		}
		
		// HIDDEN (sheet 1)
		nameLookup["001"] = new NetworkNode("leftOutput");
		nodes.push_back(nameLookup["001"]);
		nameLookup["101"] = new NetworkNode("rightOutput");
		nodes.push_back(nameLookup["101"]);
		
		// link sheet 2 to sheet 1
		for (int sourceNode = 16; sourceNode <= 19; ++sourceNode) {
			for (int targetNode = 20; targetNode <= 21; ++targetNode) {
				links.push_back(new NetworkLink(nodes[sourceNode], nodes[targetNode], 0.0));
			}
		}
		
		
		cout << "Creating FastNetwork\n";
		substrate = NEAT::FastNetwork<double>(nodes, links);
		cout << "done!\n";
	}
	
	// only need this function for the hybrid
	//	NEAT::FastNetwork<double> RetinaExperiment::populateAndReturnSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual) {
	//		populateSubstrate(individual, true);
	//		return substrate;
	//	}
	
	void RetinaExperiment::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual, bool toPrint)
	{
		
		NEAT::FastNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
		// connect sheet 4 to sheet 3
		for(int sourceX = 0; sourceX < 4; ++sourceX) {
			for(int sourceY = 0; sourceY < 2; ++sourceY) {
				for(int targetX = 0; targetX < 4; ++targetX) {
					for(int targetY = 0; targetY < 2; ++targetY) {
						
						double xOffsetSource = (2.0 * sourceX) / 3.0 - 1.0;
						double xOffsetTarget = (2.0 * targetX) / 3.0 - 1.0;					
						double yOffsetSource = sourceY * 2.0 - 1.0;
						double yOffsetTarget = targetY * 2.0 - 1.0;
						double zOffsetSource = 1;
						double zOffsetTarget = 0.5;
						
						network.reinitialize();
						network.setValue("sourceX", xOffsetSource);
						network.setValue("sourceY", yOffsetSource);
						network.setValue("sourceZ", zOffsetSource);
						network.setValue("targetX", xOffsetTarget);
						network.setValue("targetY", yOffsetTarget);
						network.setValue("targetZ", zOffsetTarget);
						network.setValue("Bias",0.3);
						//						cerr<< xOffsetSource << " "<< xOffsetTarget << " " << yOffsetSource << " " << yOffsetTarget << endl;
						network.update(10);
						
						double linkWeight = network.getValue("output");
						string sourceNode = toString(sourceX)+toString(sourceY)+"4";
						string targetNode = toString(targetX)+toString(targetY)+"3";
						//						cerr<< sourceNode<<" "<<targetNode<<" "<<linkWeight<< " " <<zeroOrNormalize(linkWeight)<<endl;
						
						substrate.getLink(nameLookup[sourceNode]->getName(), nameLookup[targetNode]->getName())->weight = zeroOrNormalize(linkWeight);
					}
				}
			}
		}
		
		
		// connect sheet 3 to sheet 2
		for(int sourceX = 0; sourceX < 4; ++sourceX) {
			for(int sourceY = 0; sourceY < 2; ++sourceY) {
				for(int targetX = 0; targetX < 4; ++targetX) {
					
					double xOffsetSource = (2.0 * sourceX) / 3.0 - 1.0;
					double xOffsetTarget = (2.0 * targetX) / 3.0 - 1.0;					
					double yOffsetSource = sourceY * 2.0 - 1.0;
					double yOffsetTarget = 0.0;
					double zOffsetSource = 0.5;
					double zOffsetTarget = 0.0;
					
					network.reinitialize();
					network.setValue("sourceX", xOffsetSource);
					network.setValue("sourceY", yOffsetSource);
					network.setValue("sourceZ", zOffsetSource);
					network.setValue("targetX", xOffsetTarget);
					network.setValue("targetY", yOffsetTarget);
					network.setValue("targetZ", zOffsetTarget);
					network.setValue("Bias",0.3);
					//					cerr<< xOffsetSource << " "<< xOffsetTarget << " " << yOffsetSource << " " << yOffsetTarget << endl;
					network.update(10);
					
					double linkWeight = network.getValue("output");
					string sourceNode = toString(sourceX)+toString(sourceY)+"3";
					string targetNode = toString(targetX)+"02";
					//					cerr<< sourceNode<<" "<<targetNode<<" "<<linkWeight<< " " <<zeroOrNormalize(linkWeight)<<endl;
					
					substrate.getLink(nameLookup[sourceNode]->getName(), nameLookup[targetNode]->getName())->weight = zeroOrNormalize(linkWeight);
				}
			}
		}
		
		
		
		// connect sheet 2 to sheet 1
		for(int sourceX = 0; sourceX < 4; ++sourceX) {
			for(int targetX = 0; targetX < 2; ++targetX) {
				
				double xOffsetSource = (2.0 * sourceX) / 3.0 - 1.0;
				double xOffsetTarget = (2.0 * targetX) - 0.5;					
				double yOffsetSource = 0.0;
				double yOffsetTarget = 0.0;
				double zOffsetSource = 0.0;
				double zOffsetTarget = -0.5;
				
				network.reinitialize();
				network.setValue("sourceX", xOffsetSource);
				network.setValue("sourceY", yOffsetSource);
				network.setValue("sourceZ", zOffsetSource);
				network.setValue("targetX", xOffsetTarget);
				network.setValue("targetY", yOffsetTarget);
				network.setValue("targetZ", zOffsetTarget);
				network.setValue("Bias",0.3);
				//				cerr<< xOffsetSource << " "<< xOffsetTarget << " " << yOffsetSource << " " << yOffsetTarget << endl;
				network.update(10);
				
				double linkWeight = network.getValue("output");
				string sourceNode = toString(sourceX)+"02";
				string targetNode = toString(targetX)+"01";
				//				cerr<< sourceNode<<" "<<targetNode<<" "<<linkWeight<< " " <<zeroOrNormalize(linkWeight)<<endl;
				
				substrate.getLink(nameLookup[sourceNode]->getName(), nameLookup[targetNode]->getName())->weight = zeroOrNormalize(linkWeight);
			}
		}
		
		
		//		printActualSubstrate();
	}
	
	
	double RetinaExperiment::processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual, bool postHocRecord = false) {

		cout << "This is hard coded to be Left and Right (but not output). Update me like Retina-FT to be able to do all the fitness functions that can be done." << endl;
		double fitness = 0.0;
		int i = 0;
		const int TRIALS = 256;
		
		bool a(false), b(false), c(false), d(false), e(false), f(false), g(false), h(false);
		double aInput(0.0), bInput(0.0), cInput(0.0), dInput(0.0),
		eInput(0.0), fInput(0.0), gInput(0.0), hInput(0.0);
		//double sumLeft = 0.0;
		//double sumRight = 0.0;
		
		if(timeInCurrentEnv%(popSize) == 0) {
			cout<<"Current Environment = "<<currentEnvironment<<endl;
		}
		
		populateSubstrate(individual);
		
		// initialize substrate inputs: -1.0==false and 1.0==true
		while (i < TRIALS) {
			
			// default to -1
			aInput = -1.0;
			bInput = -1.0;
			cInput = -1.0;
			dInput = -1.0;
			eInput = -1.0;
			fInput = -1.0;
			gInput = -1.0;
			hInput = -1.0;
			
			//unless true
			if (a) aInput = 1.0;
			if (b) bInput = 1.0;
			if (c) cInput = 1.0;
			if (d) dInput = 1.0;
			if (e) eInput = 1.0;
			if (f) fInput = 1.0;
			if (g) gInput = 1.0;
			if (h) hInput = 1.0;
			
			//			cerr << setw(3) << aInput <<
			//					setw(3) << aInput <<
			//					setw(3) << bInput <<
			//					setw(3) << cInput <<
			//					setw(3) << dInput <<
			//					setw(3) << eInput <<
			//					setw(3) << fInput <<
			//					setw(3) << gInput <<
			//					setw(3) << hInput <<
			//			endl;
			
			
			
			/*apply input to substrate*/
			// set substrate inputs
			substrate.reinitialize();
			substrate.setValue("LeftTopLeft",      aInput); //retina[0][0]
			substrate.setValue("LeftBottomLeft",   bInput); //retina[0][1]
			substrate.setValue("LeftTopRight",     cInput); //retina[1][0]
			substrate.setValue("LeftBottomRight",  dInput); //retina[1][1]
			substrate.setValue("RightTopLeft",     eInput); //retina[2][0]
			substrate.setValue("RightBottomLeft",  fInput); //retina[2][1]
			substrate.setValue("RightTopRight",    gInput); //retina[3][0]
			substrate.setValue("RightBottomRight", hInput); //retina[3][1]
			
			//for sumLeft and sumRight
			//sumLeft  = aInput + bInput + cInput + dInput;
			//sumRight = eInput + fInput + gInput + hInput;
			
			
			//update
			substrate.update(2);
			
			// retrieve output from substrate and scale to range 0.0-1.0
			double leftOutput = substrate.getValue("leftOutput");
			double rightOutput = substrate.getValue("rightOutput");
			
			//check for valid range
			assert(-1.0 <= leftOutput && leftOutput <= 1.0);
			assert(-1.0 <= rightOutput && rightOutput <= 1.0);
			
			// evaluate fitness in environment
			bool left(false), right(false);
			if ((isOn(aInput)  && isOn(bInput)  && isOn(cInput)  && isOn(dInput)) ||  // all 4
				(isOn(aInput)  && isOn(bInput)  && !isOn(cInput) && !isOn(dInput)) ||  // leftmost 2
				((isOn(aInput) || isOn(bInput)) && !isOn(cInput) && !isOn(dInput)) ||  // 1 of leftmost 2
				(isOn(aInput)  && isOn(bInput)  && isOn(cInput)  && !isOn(dInput)) ||  // 3 of 4
				(isOn(aInput)  && isOn(bInput)  && !isOn(cInput) && isOn(dInput)) ||  // 3 of 4
				(isOn(aInput)  && !isOn(bInput) && isOn(cInput)  && isOn(dInput)) ||  // 3 of 4
				(!isOn(aInput) && isOn(bInput)  && isOn(cInput)  && isOn(dInput))) {  // 3 of 4
				left = true;
			}
			
			if ((isOn(eInput)  && isOn(fInput)  && isOn(gInput)  && isOn(hInput)) ||  // all 4
				(!isOn(eInput) && !isOn(fInput) && isOn(gInput)  && isOn(hInput)) ||  // rightmost 2
				(!isOn(eInput) && !isOn(fInput) && (isOn(gInput) || isOn(hInput))) ||  // 1 of rightmost 2
				(isOn(eInput)  && isOn(fInput)  && isOn(gInput)  && !isOn(hInput)) ||  // 3 of 4
				(isOn(eInput)  && isOn(fInput)  && !isOn(gInput) && isOn(hInput)) ||  // 3 of 4
				(isOn(eInput)  && !isOn(fInput) && isOn(gInput)  && isOn(hInput)) ||  // 3 of 4
				(!isOn(eInput) && isOn(fInput)  && isOn(gInput)  && isOn(hInput))) {  // 3 of 4
				right = true;
			}
			
			//left side
			if (left and (leftOutput >=0)) 
			{
				fitness ++;
			}
			else if (!left and (leftOutput < 0))
			{
				fitness ++;
			}
			
			//right side
			if (right and (rightOutput >=0)) 
			{
				fitness ++;
			}
			else if (!right and (rightOutput < 0))
			{
				fitness ++;
			}
			
			
			++i;
			// update input for next round
			a = !a;
			b = i/2  %2;
			c = i/4  %2;
			d = i/8  %2;			
			e = i/16 %2;
			f = i/32 %2;
			g = i/64 %2;
			h = i/128%2;			
			
		}
		//		PRINT(fitness);
		
		if(postHocRecord) {
			// running in post hoc mode 
			return fitness;
		}
		
		++timeInCurrentEnv;		
		// check for environment switch
		if(timeInCurrentEnv%(swithEnviromentStep*popSize) == 0) {
			cerr << "switch" << endl;
			exit(9);
			currentEnvironment = (currentEnvironment+1)%numEnvironments;
		}
		//		return pow(2, fitness);
		return fitness;
	}
	
	void RetinaExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
	{
		shared_ptr<NEAT::GeneticIndividual> individual = group.front();
		/*{
		 cout << "Starting Evaluation on object:" << this << endl;
		 cout << "Running on individual " << individual << endl;
		 }*/
		
		individual->setFitness(0.0);
		individual->setUserData(shared_ptr<RetinaExperimentStats>(new RetinaExperimentStats()));
		
		double fitness=1; 		//You start with 1 point because fitness cannot be 0
		
		
		fitness += processEvaluation(individual,NULL);
		
		individual->reward(fitness);
		
		
#ifdef LEGSWING_EXPERIMENT_DEBUG
		cout << "Individual Evaluation complete!\n";
		cout << "Fitness bump (does not include 1 free fitness point) was: " << fitness;
		cout << "maxFitness is: " << maxFitness << endl;
#endif
	}
	
	void RetinaExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
	{
		//You get 1 points just for entering the game, wahooo!
		individual->setFitness(0.0);
		//individual->setUserData(shared_ptr<LegSwingStats>(new LegSwingStats()));		
		
		double fitness=0;
		
		double maxFitness = 1;
		
		int testCases=0;
		
		testCases++;
		
		fitness += processEvaluation(individual, true);
		printActualSubstrate();
		
		
		maxFitness += 42;
		
		cout << "TOTAL TEST CASES: " << testCases << endl;
		
		cout << "Individual Evaluation complete!\n";
		
		cout << maxFitness << endl;
		
		individual->reward(fitness);
	}
	
	Experiment* RetinaExperiment::clone()
	{
		RetinaExperiment* experiment = new RetinaExperiment(*this);
		
		return experiment;
	}
	
	void RetinaExperiment::printSubstrate()
	{
		cout << "Printing substrate is not implemented" << endl;
		ofstream substrate_file;        
		substrate_file.open ("substrateThatSolvedTheProblem.txt", ios::trunc );
		
		cout << "num links: " <<substrate.getLinkCount() << endl;
		substrate_file << "num links: " <<substrate.getLinkCount() << endl;
		substrate_file << "Printing substrate is not implemented";
		substrate_file.close();
		
		return;
	}
	
	void RetinaExperiment::printActualSubstrate()
	{
		cout << "Printing substrate" << endl;
		ofstream substrate_file;        
		substrate_file.open ("substrateThatSolvedTheProblem.txt", ios::trunc );
		
		int numLinks = substrate.getLinkCount();
		cout << "num links: " << numLinks << endl;
		substrate_file << "num links: " << numLinks << endl;
		
		cout << "num nodes: " << substrate.getNodeCount() << endl;
		substrate_file << "num nodes: " << substrate.getNodeCount() << endl;
		
		
		for (int counter=0;counter<numLinks;counter++)
		{
			if(counter > substrate.getLinkCount()) {
				cout << "error: you are trying to print a link that does not exist" << endl; exit(1);
			}
			
			substrate_file << "link num: " << counter << " " << linkIndexToNameLookup[counter] << " : ";
			
			substrate_file << substrate.getLink(counter)->fromNode<<" "<< substrate.getLink(counter)->toNode <<" " <<substrate.getLink(counter)->weight << endl;
		}
		substrate_file.close();
		
		return;
	}
	
	
	void RetinaExperiment::printCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
	{
		cout << "Printing cppn network" << endl;
		ofstream network_file;        
		network_file.open ("CPPN-ThatSolvedTheProblem.txt", ios::trunc );
		
		NEAT::FastNetwork <double> network = individual->spawnFastPhenotypeStack<double>();        //JMC: this creates the network CPPN associated with this individual used to produce the substrate (neural network)
		
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
		
		for (size_t a=0;a<(unsigned int)numLinks;a++)
		{
			NetworkIndexedLink<double> link = *network.getLink(a);
			
			network_file << link.fromNode << "->" << link.toNode << " : " << link.weight << endl;
		}
		for (size_t a=0;a<(unsigned int)numNodes;a++)
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
	
	double RetinaExperiment::zeroOrNormalize(double mWeightToAlter) {
		assert(-1.0 <= mWeightToAlter && mWeightToAlter<= 1.0);  // if this fails then the normalization is not valid.
		if (-0.1 < mWeightToAlter && mWeightToAlter < 0.1) {
			return 0.0;
		}
		return mWeightToAlter * 3.0;
	}
	
	bool RetinaExperiment::converged(int generation) {
		if(generation == convergence_step)
			return true;
		return false;
	}
	bool RetinaExperiment::isOn(double x) {
		if(x == 1.0)
			return true;
		return false;
	}
	
	bool RetinaExperiment::XOR(bool a, bool b) {
		return (a && !b) || (b && !a);
	}
	
}
