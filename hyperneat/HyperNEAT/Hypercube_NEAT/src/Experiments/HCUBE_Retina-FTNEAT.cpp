/*
 *  retina_ecperiment.cpp
 *  modularity
 *
 *  Created by Benjamin Beckmann on 11/16/09.
 *  Copyright 2009 Michigan State University. All rights reserved.
 *
 */


/*
 *  ModularityExperiment.cpp
 *  v2Hyperneat
 *
 *  Created by Benjamin Beckmann on 8/12/09.
 *  Copyright 2009 Michigan State University. All rights reserved.
 *
 */

#include "Experiments/HCUBE_Retina-FTNEAT.h"

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
	
	RetinaFTExperiment::RetinaFTExperiment(string _experimentName)
	:	Experiment(_experimentName),
	NUM_SHEETS(5)
	{		
		numEnvironments = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("NumberOfEnvironments"));
		swithEnviromentStep = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("SwithEnviromentStep"));
		convergence_step = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("ConvergenceStep"));
		popSize = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("PopulationSize"));
		fitnessFunction = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("FitnessFunction"));
		cutLinksThatCrossAxisX = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("CutLinksThatCrossAxisX"));
		spreadInputsAxisX = NEAT::Globals::getSingleton()->getParameterValue("SpreadInputsAxisX");
		zeroOutHalfWidth = NEAT::Globals::getSingleton()->getParameterValue("ZeroOutHalfWidth");
		
		if(convergence_step < 0) {
			convergence_step = INT_MAX; // never switch
		}
		currentEnvironment = static_cast<int>(NEAT::Globals::getSingleton()->getParameterValue("StartEnvironment"));
		timeInCurrentEnv = 0;
		
		generateSubstrate();
	}
	
	
	NEAT::GeneticPopulation* RetinaFTExperiment::createInitialPopulation(int populationSize) {
		GeneticPopulation *population = new GeneticPopulation();
		vector<GeneticNodeGene> genes;
    vector<GeneticLinkGene> links;

    // INPUT (sheet 4)
		genes.push_back(GeneticNodeGene("004","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("014","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("104","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("114","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("204","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("214","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("304","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("314","NetworkSensor",0,false));
		
    // HIDDEN (sheet 3)
    genes.push_back(GeneticNodeGene("003","HiddenNode",0.25,false));
		genes.push_back(GeneticNodeGene("013","HiddenNode",0.25,false));
		genes.push_back(GeneticNodeGene("103","HiddenNode",0.25,false));
		genes.push_back(GeneticNodeGene("113","HiddenNode",0.25,false));
		genes.push_back(GeneticNodeGene("203","HiddenNode",0.25,false));
		genes.push_back(GeneticNodeGene("213","HiddenNode",0.25,false));
		genes.push_back(GeneticNodeGene("303","HiddenNode",0.25,false));
		genes.push_back(GeneticNodeGene("313","HiddenNode",0.25,false));
    
    // link sheet 4 to sheet 3
		for (int sourceNode = 0; sourceNode <= 7; ++sourceNode) {
			for (int targetNode = 8; targetNode <= 15; ++targetNode) {
				links.push_back(GeneticLinkGene(genes[sourceNode].getID(), genes[targetNode].getID()));
			}
		}
    
    // HIDDEN (sheet 2)
    genes.push_back(GeneticNodeGene("002","HiddenNode",0.5,false));
		genes.push_back(GeneticNodeGene("102","HiddenNode",0.5,false));
		genes.push_back(GeneticNodeGene("202","HiddenNode",0.5,false));
		genes.push_back(GeneticNodeGene("302","HiddenNode",0.5,false));
    
    // link sheet 3 to sheet 2
		for (int sourceNode = 8; sourceNode <= 15; ++sourceNode) {
			for (int targetNode = 16; targetNode <= 19; ++targetNode) {
				links.push_back(GeneticLinkGene(genes[sourceNode].getID(), genes[targetNode].getID()));
			}
		}
    
    // HIDDEN (sheet 1)
    genes.push_back(GeneticNodeGene("001","HiddenNode",0.75,false));
		genes.push_back(GeneticNodeGene("101","HiddenNode",0.75,false));

		// link sheet 2 to sheet 1
		for (int sourceNode = 16; sourceNode <= 19; ++sourceNode) {
			for (int targetNode = 20; targetNode <= 21; ++targetNode) {
				links.push_back(GeneticLinkGene(genes[sourceNode].getID(), genes[targetNode].getID()));
			}
		}
    // OUTPUT (sheet 0)
		genes.push_back(GeneticNodeGene("000","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		
    // link sheet 1 to sheet 0
		for (int sourceNode = 20; sourceNode <= 21; ++sourceNode) {
      links.push_back(GeneticLinkGene(genes[sourceNode].getID(), genes[22].getID()));
		}
		
		genes.push_back(GeneticNodeGene("BIAS","NetworkSensor",0,false));
		// link BIAS to every node
		for (int targetNode = 8; targetNode <= 22; ++targetNode) {
      links.push_back(GeneticLinkGene(genes[23].getID(), genes[targetNode].getID()));
		}
    
		for (int a=0;a<populationSize;a++)
		{
      shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,links));
			
			//for (int b=0;b<0;b++)  
			//{
			//	individual->testMutate();
			//}
			
			population->addIndividual(individual);
		}
		
		cout << "Finished creating population\n";
		return population;  // TODO: comment out to increase speed.  Required for hybrid
	}
	
	void RetinaFTExperiment::generateSubstrate()
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
		nameLookup["004"] = new NetworkNode("LeftTopLeft", false, ACTIVATION_FUNCTION_LINEAR);
		nodes.push_back(nameLookup["004"]);
		nameLookup["014"] = new NetworkNode("LeftBottomLeft", false, ACTIVATION_FUNCTION_LINEAR);
		nodes.push_back(nameLookup["014"]);
		nameLookup["104"] = new NetworkNode("LeftTopRight", false, ACTIVATION_FUNCTION_LINEAR);
		nodes.push_back(nameLookup["104"]);
		nameLookup["114"] = new NetworkNode("LeftBottomRight", false, ACTIVATION_FUNCTION_LINEAR);
		nodes.push_back(nameLookup["114"]);
		nameLookup["204"] = new NetworkNode("RightTopLeft", false, ACTIVATION_FUNCTION_LINEAR);
		nodes.push_back(nameLookup["204"]);
		nameLookup["214"] = new NetworkNode("RightBottomLeft", false, ACTIVATION_FUNCTION_LINEAR);
		nodes.push_back(nameLookup["214"]);
		nameLookup["304"] = new NetworkNode("RightTopRight", false, ACTIVATION_FUNCTION_LINEAR);
		nodes.push_back(nameLookup["304"]);
		nameLookup["314"] = new NetworkNode("RightBottomRight", false, ACTIVATION_FUNCTION_LINEAR);
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
		nameLookup["001"] = new NetworkNode("001");
		nodes.push_back(nameLookup["001"]);
		nameLookup["101"] = new NetworkNode("101");
		nodes.push_back(nameLookup["101"]);
		
		// link sheet 2 to sheet 1
		for (int sourceNode = 16; sourceNode <= 19; ++sourceNode) {
			for (int targetNode = 20; targetNode <= 21; ++targetNode) {
				links.push_back(new NetworkLink(nodes[sourceNode], nodes[targetNode], 0.0));
			}
		}
		
		// OUTPUT (sheet 0)
		nameLookup["000"] = new NetworkNode("output");
		nodes.push_back(nameLookup["000"]);
		
		// link sheet 1 to sheet 0
		for (int sourceNode = 20; sourceNode <= 21; ++sourceNode) {
			links.push_back(new NetworkLink(nodes[sourceNode], nodes[22], 0.0));
		}
		
		nameLookup["BIAS"] = new NetworkNode("BIAS", false, ACTIVATION_FUNCTION_LINEAR);
		nodes.push_back(nameLookup["BIAS"]);
		// link BIAS to every node
		for (int targetNode = 8; targetNode <= 22; ++targetNode) {
			links.push_back(new NetworkLink(nodes[23] ,nodes[targetNode], 0.0));
		}
		
		cout << "Creating FastNetwork\n";
		substrate = NEAT::FastNetwork<double>(nodes, links);
		cout << "done!\n";
	}
	
	// only need this function for the hybrid
	//	NEAT::FastNetwork<double> RetinaFTExperiment::populateAndReturnSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual) {
	//		populateSubstrate(individual, true);
	//		return substrate;
	//	}
	
	void RetinaFTExperiment::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual, bool toPrint)
	{
		NEAT::FastNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
    
    //TODO: copy each link value from network to substrate.
    
		// connect sheet 4 to sheet 3
		for(int sourceX = 0; sourceX < 4; ++sourceX) {
			for(int sourceY = 0; sourceY < 2; ++sourceY) {
				for(int targetX = 0; targetX < 4; ++targetX) {
					for(int targetY = 0; targetY < 2; ++targetY) {
						
						double xOffsetSource = (2.0 * sourceX) / 3.0 - 1.0;
						double xOffsetTarget = (2.0 * targetX) / 3.0 - 1.0;
						if (spreadInputsAxisX != 0.0) {
							if (fabs(xOffsetSource) != 1.0) {
								xOffsetSource *= 2.0;
							}
							if (fabs(xOffsetTarget) != 1.0) {
								xOffsetTarget *= 2.0;
							}
						}
												
						string sourceNode = toString(sourceX)+toString(sourceY)+"4";
						string targetNode = toString(targetX)+toString(targetY)+"3";

						double linkWeight;
						if(((xOffsetSource < 0.0 && xOffsetTarget > 0.0) || (xOffsetSource > 0.0 && xOffsetTarget < 0.0)) && cutLinksThatCrossAxisX > 0) {
							linkWeight = 0.0;
						} else {
							linkWeight = network.getLink(sourceNode, targetNode)->weight;
						}
            //						cerr<< sourceNode<<" "<<targetNode<<" "<<linkWeight<< " " <<zeroOrNormalize(linkWeight)<<endl;
						
						substrate.getLink(nameLookup[sourceNode]->getName(), nameLookup[targetNode]->getName())->weight = zeroOrNormalize(linkWeight);
            
            
            //BIAS						
						sourceNode = "BIAS";
						targetNode = toString(targetX)+toString(targetY)+"3";
						linkWeight = network.getLink(sourceNode, targetNode)->weight;
						
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
					if (spreadInputsAxisX != 0.0) {
						if (fabs(xOffsetSource) != 1.0) {
							xOffsetSource *= 2.0;
						}
						if (fabs(xOffsetTarget) != 1.0) {
							xOffsetTarget *= 2.0;
						}
					}
					
					string sourceNode = toString(sourceX)+toString(sourceY)+"3";
					string targetNode = toString(targetX)+"02";
					double linkWeight;
					if(((xOffsetSource < 0.0 && xOffsetTarget > 0.0) || (xOffsetSource > 0.0 && xOffsetTarget < 0.0)) && cutLinksThatCrossAxisX > 0) {
						linkWeight = 0.0;
					} else {
						linkWeight = network.getLink(sourceNode, targetNode)->weight;
					}
          //					cerr<< sourceNode<<" "<<targetNode<<" "<<linkWeight<< " " <<zeroOrNormalize(linkWeight)<<endl;
					
					substrate.getLink(nameLookup[sourceNode]->getName(), nameLookup[targetNode]->getName())->weight = zeroOrNormalize(linkWeight);
          
					//BIAS
					sourceNode = "BIAS";
					targetNode = toString(targetX)+"02";
					linkWeight = network.getLink(sourceNode, targetNode)->weight;
					
					substrate.getLink(nameLookup[sourceNode]->getName(), nameLookup[targetNode]->getName())->weight = zeroOrNormalize(linkWeight);					
				}
			}
		}
		
		
		// connect sheet 2 to sheet 1
		for(int sourceX = 0; sourceX < 4; ++sourceX) {
			for(int targetX = 0; targetX < 2; ++targetX) {
				
				double xOffsetSource = (2.0 * sourceX) / 3.0 - 1.0;
				double xOffsetTarget = (2.0 * targetX) - 0.5;
				if (spreadInputsAxisX != 0.0) {
					if (fabs(xOffsetSource) != 1.0) {
						xOffsetSource *= 2.0;
					}
				}
				
				string sourceNode = toString(sourceX)+"02";
				string targetNode = toString(targetX)+"01";
				double linkWeight;
				if(((xOffsetSource < 0.0 && xOffsetTarget > 0.0) || (xOffsetSource > 0.0 && xOffsetTarget < 0.0)) && cutLinksThatCrossAxisX > 0) {
					linkWeight = 0.0;
				} else {
					linkWeight = network.getLink(sourceNode, targetNode)->weight;
				}
        //				cerr<< sourceNode<<" "<<targetNode<<" "<<linkWeight<< " " <<zeroOrNormalize(linkWeight)<<endl;
				
				substrate.getLink(nameLookup[sourceNode]->getName(), nameLookup[targetNode]->getName())->weight = zeroOrNormalize(linkWeight);
        
				//BIAS
				sourceNode = "BIAS";
				targetNode = toString(targetX)+"01";
				linkWeight = network.getLink(sourceNode, targetNode)->weight;
				
				substrate.getLink(nameLookup[sourceNode]->getName(), nameLookup[targetNode]->getName())->weight = zeroOrNormalize(linkWeight);									
			}
		}
		
		
		// connect sheet 1 to sheet 0
		for(int sourceX = 0; sourceX < 2; ++sourceX) {
			
			double xOffsetSource = (2.0 * sourceX) - 0.5;
			double xOffsetTarget = 0.0;
			string sourceNode = toString(sourceX)+"01";
			string targetNode = "000";
			double linkWeight;
			if(((xOffsetSource < 0.0 && xOffsetTarget > 0.0) || (xOffsetSource > 0.0 && xOffsetTarget < 0.0)) && cutLinksThatCrossAxisX > 0) {
				linkWeight = 0.0;
			} else {
				linkWeight = network.getLink(sourceNode, targetNode)->weight;
			}
      //			cerr<< sourceNode<<" "<<targetNode<<" "<<linkWeight<< " " <<zeroOrNormalize(linkWeight)<<endl;
			
			substrate.getLink(nameLookup[sourceNode]->getName(), nameLookup[targetNode]->getName())->weight = zeroOrNormalize(linkWeight);
		}
		
		//BIAS
		string sourceNode = "BIAS";
		string targetNode = "000";
		double linkWeight = network.getLink(sourceNode, targetNode)->weight;
		
		substrate.getLink(nameLookup[sourceNode]->getName(), nameLookup[targetNode]->getName())->weight = zeroOrNormalize(linkWeight);					
		
    //				printActualSubstrate();
	}
	
	
	double RetinaFTExperiment::processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual, bool postHocRecord = false) {

		const int ITERATIONS(256);
		double fitness = 0.0;
		int i = 0;
		bool a(false), b(false), c(false), d(false), e(false), f(false), g(false), h(false);
		double retina[4][2];
		
		//if(timeInCurrentEnv%(popSize) == 0) {
		//	cout<<"Current Environment = "<<currentEnvironment<<endl;
		//}
		
		populateSubstrate(individual);
    
		while (i < ITERATIONS) {
			// initialize substrate inputs
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 2; ++k) {
					retina[j][k] = -3.0;
				}
			}
      
			if (a) retina[0][0] = 3.0;
			if (b) retina[0][1] = 3.0;
			if (c) retina[1][0] = 3.0;
			if (d) retina[1][1] = 3.0;
			if (e) retina[2][0] = 3.0;
			if (f) retina[2][1] = 3.0;
			if (g) retina[3][0] = 3.0;
			if (h) retina[3][1] = 3.0;			
			/*apply input to substrate*/
			// set substrate inputs
			substrate.reinitialize();
			substrate.setValue("LeftTopLeft",      retina[0][0]);
			substrate.setValue("LeftBottomLeft",   retina[0][1]);
			substrate.setValue("LeftTopRight",     retina[1][0]);
			substrate.setValue("LeftBottomRight",  retina[1][1]);
			substrate.setValue("RightTopLeft",     retina[2][0]);
			substrate.setValue("RightBottomLeft",  retina[2][1]);
			substrate.setValue("RightTopRight",    retina[3][0]);
			substrate.setValue("RightBottomRight", retina[3][1]);
			substrate.setValue("BIAS", 3.0);
			
			//update to left and right outputs
			substrate.update(2);
      
			//get left and right output
			double leftOutput  = substrate.getValue("001")/2.0+0.5;
			double rightOutput = substrate.getValue("101")/2.0+0.5;
      //			cerr<<leftOutput<<" "<<rightOutput<<endl;
			//update to final output
			substrate.update(1);
      
			// retrieve output from substrate and scale to range 0.0-1.0
			double out = substrate.getValue("output");
			double output = out/2.0+0.5;
			
			//check for valid range
			assert(0.0 <= output && output <= 1.0);
			
			// evaluate fitness in environment
			bool solution;
			bool left(false), right(false);
			if (( isOn(retina[0][0]) &&  isOn(retina[0][1])  &&  isOn(retina[1][0]) &&  isOn(retina[1][1])) ||  // all 4
					( isOn(retina[0][0]) &&  isOn(retina[0][1])  && !isOn(retina[1][0]) && !isOn(retina[1][1])) ||  // leftmost 2
					((isOn(retina[0][0]) ||  isOn(retina[0][1])) && !isOn(retina[1][0]) && !isOn(retina[1][1])) ||  // 1 of leftmost 2
					( isOn(retina[0][0]) &&  isOn(retina[0][1])  &&  isOn(retina[1][0]) && !isOn(retina[1][1])) ||  // 3 of 4
					( isOn(retina[0][0]) &&  isOn(retina[0][1])  && !isOn(retina[1][0]) &&  isOn(retina[1][1])) ||  // 3 of 4
					( isOn(retina[0][0]) && !isOn(retina[0][1])  &&  isOn(retina[1][0]) &&  isOn(retina[1][1])) ||  // 3 of 4
					(!isOn(retina[0][0]) &&  isOn(retina[0][1])  &&  isOn(retina[1][0]) &&  isOn(retina[1][1]))) {  // 3 of 4
				left = true;
			}
      
			if (( isOn(retina[2][0]) &&  isOn(retina[2][1]) &&  isOn(retina[3][0]) &&  isOn(retina[3][1]))  ||  // all 4
					(!isOn(retina[2][0]) && !isOn(retina[2][1]) &&  isOn(retina[3][0]) &&  isOn(retina[3][1]))  ||  // rightmost 2
					(!isOn(retina[2][0]) && !isOn(retina[2][1]) && (isOn(retina[3][0]) ||  isOn(retina[3][1]))) ||  // 1 of rightmost 2
					( isOn(retina[2][0]) &&  isOn(retina[2][1]) &&  isOn(retina[3][0]) && !isOn(retina[3][1]))  ||  // 3 of 4
					( isOn(retina[2][0]) &&  isOn(retina[2][1]) && !isOn(retina[3][0]) &&  isOn(retina[3][1]))  ||  // 3 of 4
					( isOn(retina[2][0]) && !isOn(retina[2][1]) &&  isOn(retina[3][0]) &&  isOn(retina[3][1]))  ||  // 3 of 4
					(!isOn(retina[2][0]) &&  isOn(retina[2][1]) &&  isOn(retina[3][0]) &&  isOn(retina[3][1]))) {  // 3 of 4
				right = true;
			}
			if (currentEnvironment == 0) { // in G1
				solution = left && right;
			} else if (currentEnvironment == 1) { // in G2
				solution = left || right;
			} else {
				cerr<<"Unknown environment " << currentEnvironment << " Exitting.\n";
				exit(-1);
			}
			
			if(fitnessFunction == 0) { // use only final output
				if (solution) {
					fitness += output; // solution is true so closer to 1.0 is better
				} else {
					fitness += 1.0 - output; // solution is false so closer to 0.0 is better
				}
			} else if (fitnessFunction == 1) { // use only left and right output
				
				double tmpFitness(0.0);
				if (left) {
					tmpFitness += leftOutput;
				} else {
					tmpFitness += 1.0 - leftOutput;
				}
				if (right) {
					tmpFitness += rightOutput;
				} else {
					tmpFitness += 1.0 - rightOutput;
				}
				fitness += (tmpFitness/2.0);
				
			} else if (fitnessFunction == 2) { // use just jeft
				if (left) {
					fitness += leftOutput;
				} else {
					fitness += 1.0 - leftOutput;
				}
			} else if (fitnessFunction == 3) { // user just right
				if (right) {
					fitness += rightOutput;
				} else {
					fitness += 1.0 - rightOutput;
				}				
			} else if (fitnessFunction == 4) { // use left and right output, and final output (creates building blocks)
			  
			  // bump tmpFitness for scores on Left and Right
			  double tmpFitness(0.0);
			  if (left) {
			    tmpFitness += leftOutput;
			  } else {
			    tmpFitness += 1.0 - leftOutput;
			  }
			  if (right) {
			    tmpFitness += rightOutput;
			  } else {
			    tmpFitness += 1.0 - rightOutput;
			  }
			  
			  // bump fitness for final output
			  if (solution) {
			    tmpFitness += output; // solution is true so closer to 1.0 is better
			  } else {
			    tmpFitness += 1.0 - output; // solution is false so closer to 0.0 is better
			  }
			  
			  fitness += (tmpFitness/3.0); // three chances to get +1, so divide by three
			  
			}
			else{
			  cout << "You did not specify a valid fitness function" << endl;
			  exit(1);

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
		
		if(postHocRecord) {
			// running in post hoc mode 
			return fitness;
		}
		
		++timeInCurrentEnv;		
		// check for environment switch
		if(timeInCurrentEnv%(swithEnviromentStep*popSize) == 0) {
			currentEnvironment = (currentEnvironment+1)%numEnvironments;
		}
		return pow(fitness,2);
	}
	
	void RetinaFTExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
	{
		shared_ptr<NEAT::GeneticIndividual> individual = group.front();
		/*{
		 cout << "Starting Evaluation on object:" << this << endl;
		 cout << "Running on individual " << individual << endl;
		 }*/
		
		//You get 1 point just for entering the game, wahooo!
		individual->setFitness(0.0);
		individual->setUserData(shared_ptr<RetinaFTNEATStats>(new RetinaFTNEATStats()));
		
		double fitness=0;
		
		double maxFitness = 1;  //starts at one because their fitness starts at one 
		
		fitness += processEvaluation(individual,NULL);
    
		maxFitness += 99999;
		
		individual->reward(fitness);
		
		static bool haveReportedMaxFitness = false;
		if(!haveReportedMaxFitness){
			cout << "maxFitness is: " << maxFitness << endl;
			haveReportedMaxFitness = true;
		}
		
#ifdef LEGSWING_EXPERIMENT_DEBUG
		cout << "Individual Evaluation complete!\n";
		cout << "Fitness bump (does not include 1 free fitness point) was: " << fitness;
		cout << "maxFitness is: " << maxFitness << endl;
#endif
	}
	
	void RetinaFTExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
	{
		//You get 1 points just for entering the game, wahooo!
		individual->setFitness(0.0);
		//individual->setUserData(shared_ptr<LegSwingStats>(new LegSwingStats()));		
		
		double fitness=0;
		
		double maxFitness = 1;
		
		int testCases=0;
		
		testCases++;
		
		fitness += processEvaluation(individual,true);
		printActualSubstrate();
		
		
		maxFitness += 42;
		
		cout << "TOTAL TEST CASES: " << testCases << endl;
		
		cout << "Individual Evaluation complete!\n";
		
		cout << "fitness: " << fitness << endl;
		
		individual->reward(fitness);
	}
	
	Experiment* RetinaFTExperiment::clone()
	{
		RetinaFTExperiment* experiment = new RetinaFTExperiment(*this);
		
		return experiment;
	}
	
	void RetinaFTExperiment::printSubstrate()
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
	
	void RetinaFTExperiment::printActualSubstrate()
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
	
	
	void RetinaFTExperiment::printCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
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
	
	double RetinaFTExperiment::zeroOrNormalize(double mWeightToAlter) {
		assert(-3.0 <= mWeightToAlter && mWeightToAlter<= 3.0);  // if this fails then the normalization is not valid.
		if ((-3.0*zeroOutHalfWidth) < mWeightToAlter && mWeightToAlter < zeroOutHalfWidth*3.0) { // was -0.1 to 0.1
			return 0.0;
		}
		return mWeightToAlter;
	}
	
	bool RetinaFTExperiment::converged(int generation) {
		if(generation == convergence_step)
			return true;
		return false;
	}
	
	bool RetinaFTExperiment::isOn(double x) {
		if(x > 0.0)
			return true;
		return false;
	}
	
	bool RetinaFTExperiment::XOR(bool a, bool b) {
		return (a && !b) || (b && !a);
	}
	
	
}
