// code based on legged.cpp by Kosei Demura (2007-5-19) (demura.net)

#ifdef COMPILE_ODE
#include "Experiments/HCUBE_LegSwingExperiment-Pneat.h"
#include "NEAT_Random.h"
#include <cmath>

#define LEGSWING_PNEAT_HACK (0)
#define POSTHOC_RECORD_PNEAT
//#define LEGSWING_PNEAT_INTERACTIVE_EVOLUTION
//#define SUBSTRATE_PNEAT_DEBUG (1)


namespace HCUBE
{
	using namespace NEAT;
	
	LegSwingPneatExperiment::LegSwingPneatExperiment(string _experimentName)
	:
	Experiment(_experimentName),
	numNodesX(numNodesXglobal),
	numNodesY(numNodesYglobal),
	Hybrid_FTMutateOnlyProbability(-1.0),
	Hybrid_FTMutateLinkProbability(-1.0)
	{
		simulation_steps = (int) NEAT::Globals::getSingleton()->getParameterValue("SimulationSteps");
		
#ifdef USE_NLEG_ROBOT
		hingeType = (int) NEAT::Globals::getSingleton()->getParameterValue("TorsoHingeType");
#endif
		
		fitness_function = (int) NEAT::Globals::getSingleton()->getParameterValue("FitnessFunction");
		
		if(NEAT::Globals::getSingleton()->getParameterValue("LimitLegSwings") == 1) {
			maxnumDirectionChanges = int (.16*simulation_steps*(static_cast<double>(num_legs*BODY_SEGMENTS)/4.0));
		}
		else {
			maxnumDirectionChanges = INT_MAX;
		}

		experimentType = int(NEAT::Globals::getSingleton()->getParameterValue("ExperimentType"));

		
		int numJointsWithError = (int) NEAT::Globals::getSingleton()->getParameterValue("NumJointsWithError");
		
		double widthOfErrorRange = NEAT::Globals::getSingleton()->getParameterValue("WidthOfErrorRange");
		
		if(currentJointsWithError < numJointsWithError) {
			for(int segment = 0; segment < BODY_SEGMENTS; ++segment) {
				for(int leg = 0; leg < num_legs; ++leg) {
					for(int joint = 0; joint < num_joints; ++joint) {
						jointError[segment][leg][joint] = 0.0;
					}
				}
			}
			
			for(; currentJointsWithError < numJointsWithError; ++currentJointsWithError) {
				int randomSegment = NEAT::Globals::getSingleton()->getRandom().getRandomInt(BODY_SEGMENTS);
				int randomLeg = NEAT::Globals::getSingleton()->getRandom().getRandomInt(num_legs);
				int randomJoint = NEAT::Globals::getSingleton()->getRandom().getRandomInt(num_joints);
				
				if(jointError[randomSegment][randomLeg][randomJoint] == 0.0)
					jointError[randomSegment][randomLeg][randomJoint] = NEAT::Globals::getSingleton()->getRandom().getRandomDouble(-0.5*widthOfErrorRange, 0.5*widthOfErrorRange);
				else
					--currentJointsWithError; // hack.  this is adaquate for small numJointsWithError;
			}
		}
		if(_experimentName == "HYBRID") {
			Hybrid_FTMutateOnlyProbability = NEAT::Globals::getSingleton()->getParameterValue("Hybrid-FT-MutateOnlyProbability");
			Hybrid_FTMutateLinkProbability =	NEAT::Globals::getSingleton()->getParameterValue("Hybrid-FT-MutateLinkProbability");
		}
		
		
		generateSubstrate();
	}
	
	GeneticPopulation* LegSwingPneatExperiment::createInitialPopulation(int populationSize)
	{
		GeneticPopulation* population = new GeneticPopulation();
		
		vector<GeneticNodeGene> genes;
		vector<GeneticLinkGene> links;
		
//		int numSheets;
#ifdef HIDDEN_LAYER
		numSheets = 3;
#else
		numSheets = 2;
#endif
		
		genes.reserve(numNodesX*numNodesY*numSheets);  //three sheets (input, hidden, output)
		
#ifdef RECURRENT
		links.reserve(numNodesX*numNodesY*numNodesX*numNodesY*(numSheets)); //all links between each node in the from sheet and the to sheet, which happens twice (input to hidden, hidden to output), plus the recurrent loop
#else
		links.reserve(numNodesX*numNodesY*numNodesX*numNodesY*(numSheets -1)); //all links between each node in the from sheet and the to sheet, which happens twice (input to hidden, hidden to output)
#endif
		
		vector <int> inputNodeIndexes;
#ifdef HIDDEN_LAYER
		vector <int> hiddenNodeIndexes;
#endif
		vector <int> outputNodeIndexes;
		
		cout << "Creating Nodes\n";
		int nodeCounter =0;
		
		for (int x1=0;x1<numNodesX;x1++)
		{
			for (int y1=0;y1<numNodesY;y1++)
			{
				for (int z=0;z<numSheets;z++)
				{                    
					string name = toString(x1)+string("/")+toString(y1)+ string("/") + toString(z);
					
//#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
//					printf("createInitialPop: creating node numbered: %i:", nodeCounter);
//					cout << name << endl;
//#endif
					
					if(z==0) genes.push_back(GeneticNodeGene(name,"NetworkSensor",0,false));     
					else     genes.push_back(GeneticNodeGene(name,"NetworkOutputNode",1,false)); //they don't have a NetworkHidden, so I am running with this
					
					if(z==0) inputNodeIndexes.push_back(nodeCounter);
#ifdef HIDDEN_LAYER
					else if (z==1) hiddenNodeIndexes.push_back(nodeCounter);
					else if (z==2) outputNodeIndexes.push_back(nodeCounter);
#else
					else if (z==1) outputNodeIndexes.push_back(nodeCounter);
#endif
					
					nodeCounter++;
				}
			}
		}
		cout << "numNodesPerOrg:" << nodeCounter << endl;
		
		cout << "Creating links\n";
		int createPopLinkCounter = 0;
		
#ifdef HIDDEN_LAYER
		
		//create links between each input sheet node and each hidden sheet node
		for(int z=0;z<inputNodeIndexes.size();z++){
			for(int q=0;q<hiddenNodeIndexes.size();q++){
				int fromNodeIndex = inputNodeIndexes[z];
				int toNodeIndex   = hiddenNodeIndexes[q];
				
//#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
//				cout << "createInitialPop::Creating link from "
//				<< genes[fromNodeIndex].getName() << " to "
//				<< genes[toNodeIndex].getName() << endl;
//#endif
				links.push_back(
												GeneticLinkGene(
																				genes[fromNodeIndex].getID(),
																				genes[toNodeIndex].getID()
																				)
												);
				createPopLinkCounter++;                
			}
		}
		//create links between each (hidden sheet OR Input if no hidden sheets) node and each output sheet node
		for(int z=0;z<hiddenNodeIndexes.size();z++){
			for(int q=0;q<outputNodeIndexes.size();q++){
				int fromNodeIndex = hiddenNodeIndexes[z];
				int toNodeIndex   = outputNodeIndexes[q];
//#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
//				cout << "createInitialPop::Creating link from "
//				<< genes[fromNodeIndex].getName() << " to "
//				<< genes[toNodeIndex].getName() << endl;
//#endif
				links.push_back(
												GeneticLinkGene(
																				genes[fromNodeIndex].getID(),
																				genes[toNodeIndex].getID()
																				)
												);
				createPopLinkCounter++;                
			}
		}
#else
		//create links between each input sheet node and each output sheet node
		for(int z=0;z<inputNodeIndexes.size();z++){
			for(int q=0;q<outputNodeIndexes.size();q++){
				int fromNodeIndex = inputNodeIndexes[z];
				int toNodeIndex   = outputNodeIndexes[q];
				
//#if LEGSWING_EXPERIMENT_PNEAT_DEBUG
//				cout << "createInitialPop::Creating link from "
//				<< genes[fromNodeIndex].getName() << " to "
//				<< genes[toNodeIndex].getName() << endl;
//#endif
				links.push_back(
												GeneticLinkGene(
																				genes[fromNodeIndex].getID(),
																				genes[toNodeIndex].getID()
																				)
												);
				createPopLinkCounter++;                
			}
		}
#endif
		
#ifdef RECURRENT
		//create link from output to input sheet
		for(int z=0;z<outputNodeIndexes.size();z++){
			for(int q=0;q<inputNodeIndexes.size();q++){
				int fromNodeIndex = outputNodeIndexes[z];
				int toNodeIndex   = inputNodeIndexes[q];
				
//#if LEGSWING_EXPERIMENT_PNEAT_DEBUG
//				cout << "createInitialPop::Creating link from "
//				<< genes[fromNodeIndex].getName() << " to "
//				<< genes[toNodeIndex].getName() << endl;
//#endif
				links.push_back(
												GeneticLinkGene(
																				genes[fromNodeIndex].getID(),
																				genes[toNodeIndex].getID()
																				)
												);
				createPopLinkCounter++;                
			}
		}
#endif
		
		
		printf("created %i links in createPopulation\n", createPopLinkCounter);
		for (size_t a=0;a<populationSize;a++)
		{
			//cout << "Creating Individual\n";
			shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,links));
#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
			printf("indLinkCount: %i\n", individual->getLinksCount());
#endif
			
			individual->testMutate();
			
			population->addIndividual(individual);
		}
		
		return population;
	}
	
	NEAT::GeneticPopulation* LegSwingPneatExperiment::createInitialPopulation(shared_ptr<NEAT::GeneticPopulation> CPPN_pop, shared_ptr<Experiment> HyperNEAT_experiment) {
		shared_ptr<GeneticGeneration> HyperNEAT_lastGeneration = CPPN_pop->getGeneration();
		GeneticPopulation* PNEAT_population = createInitialPopulation(HyperNEAT_lastGeneration->getIndividualCount()); // initialize P-NEAT population
		shared_ptr<GeneticGeneration> PNEAT_lastGeneration = PNEAT_population->getGeneration();
		
		shared_ptr<GeneticIndividual> bestIndividual = CPPN_pop->getBestIndividualOfGeneration(-1);
		// TODO: copy best individual into P-NEAT
		
		for(int i = 0; i < HyperNEAT_lastGeneration->getIndividualCount(); ++i) {
			
			// copy all link values from HyperNEAT indivdual to P-NEAT individaul
			shared_ptr<GeneticIndividual> HyperNEAT_individaul = HyperNEAT_lastGeneration->getIndividual(i);
			shared_ptr<GeneticIndividual> PNEAT_individaul = PNEAT_lastGeneration->getIndividual(i);
			PNEAT_individaul->setFitness(HyperNEAT_individaul->getFitness());
			PNEAT_individaul->setParent1Fitness(HyperNEAT_individaul->getParent1Fitness());
			PNEAT_individaul->setDirectionChanges(HyperNEAT_individaul->getDirectionChanges());
			
			NEAT::FastNetwork<double> HyperNEAT_substrate = HyperNEAT_experiment->populateAndReturnSubstrate(HyperNEAT_individaul);
			// input to hidden
			for (int fromX = 0; fromX < numNodesX; ++fromX) {
				for (int fromY = 0; fromY < numNodesY; ++fromY) {
					for (int toX = 0; toX < numNodesX; ++toX) {
						for (int toY = 0; toY < numNodesY; ++toY) {
							
							string sourceString(toString(fromX)+string("/")+toString(fromY)+ string("/") + toString(0));
							GeneticNodeGene* sourceNode = PNEAT_individaul->getNode(sourceString);
							string targetString(toString(toX)+string("/")+toString(toY)+ string("/") + toString(1));
							GeneticNodeGene* targetNode = PNEAT_individaul->getNode(targetString);
							
							GeneticLinkGene* PNEAT_link;
							PNEAT_link = PNEAT_individaul->getLink(sourceNode->getID(), targetNode->getID());
							PNEAT_link->setWeight(HyperNEAT_substrate.getLink(sourceString, targetString)->weight);

							//PNEAT_individaul->setFitness(HyperNEAT_individaul->getFitness());
						}
					}
				}
			}
			
			// hidden to output
			for (int fromX = 0; fromX < numNodesX; ++fromX) {
				for (int fromY = 0; fromY < numNodesY; ++fromY) {
					for (int toX = 0; toX < numNodesX; ++toX) {
						for (int toY = 0; toY < numNodesY; ++toY) {
							
							string sourceString(toString(fromX)+string("/")+toString(fromY)+ string("/") + toString(1));
							GeneticNodeGene* sourceNode = PNEAT_individaul->getNode(sourceString);
							string targetString(toString(toX)+string("/")+toString(toY)+ string("/") + toString(2));
							GeneticNodeGene* targetNode = PNEAT_individaul->getNode(targetString);
							
							GeneticLinkGene* PNEAT_link;
							PNEAT_link = PNEAT_individaul->getLink(sourceNode->getID(), targetNode->getID());
							PNEAT_link->setWeight(HyperNEAT_substrate.getLink(sourceString, targetString)->weight);
							
							//PNEAT_individaul->setFitness(HyperNEAT_individaul->getFitness());
						}
					}
				}
			}
		}
		return PNEAT_population;
	}
	
	
	void LegSwingPneatExperiment::generateSubstrate()
	{
//		int numSheets;
#ifdef HIDDEN_LAYER
		numSheets = 3;
#else
		numSheets = 2;
#endif
		
		cout << "Generating substrate...";
		boost::object_pool<NEAT::NetworkNode> networkNodePool;
		boost::object_pool<NEAT::NetworkLink> networkLinkPool;
		
		NEAT::NetworkNode *nodes = NULL;
		NEAT::NetworkLink *links = NULL;
		
		try
		{
			nodes = (NEAT::NetworkNode*)malloc(sizeof(NEAT::NetworkNode)*numNodesX*numNodesY*numSheets);
#ifdef RECURRENT
			links = (NEAT::NetworkLink*)malloc(sizeof(NEAT::NetworkLink)*numNodesX*numNodesY*numNodesX*numNodesY*(numSheets));
#else
			links = (NEAT::NetworkLink*)malloc(sizeof(NEAT::NetworkLink)*numNodesX*numNodesY*numNodesX*numNodesY*(numSheets - 1));
#endif
		}
		catch (std::exception e)
		{
			cout << e.what() << endl;
			CREATE_PAUSE(string("Line Number: ")+toString(__LINE__));
		}
		
		nameLookup.clear();
		
		cout << "Creating nodes...\n";
		
		int nodeCounter=0;
		
		for (int y1=0;y1<numNodesY;y1++)
		{
			for (int x1=0;x1<numNodesX;x1++)
			{
				for (int z=0;z<numSheets;z++)
				{
					
					Node node(x1,y1,z);
					string name = toString(x1)+string("/")+toString(y1) + string("/") + toString(z);
#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
					cout << "creating node (" << x1 << "," << y1 << "," << z << ") with name " << name << endl;
#endif
					nameLookup[node] = name;                                                            
					
					new(&nodes[nodeCounter]) NetworkNode(nameLookup[node]);
					nodeCounter++;
					
					/*
					 if (
					 y1==(numNodesY/2)
					 )
					 {
					 (*(nodes.end()-1))->setUpdate(false);
					 }
					 else
					 {
					 (*(nodes.end()-1))->setUpdate(true);
					 }
					 */
				}
			}
		}
		
		cout << "Creating links...\n";
		
		int linkCounter=0;
		
		for (int y1=0;y1<numNodesY;y1++)
		{
			for (int x1=0;x1<numNodesX;x1++)
			{
				for (int y2=0;y2<numNodesY;y2++)
				{
					for (int x2=0;x2<numNodesX;x2++)
					{
#ifdef RECURRENT
						for (int z1=0;z1<(numSheets);z1++)
							
#else
							for (int z1=0;z1<(numSheets -1);z1++)
#endif
							{
								int z2 = z1+1;
								//for (int z2=z1+1;z2<=z1+1;z2++)
								{
#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
									cout << "creating link no. " << linkCounter << " which is ( " << y1 << "," << x1 << "," << z1 << ") to ( " << y2 << "," << x2 << "," << z2 << ")\n";   
#endif
									string linkName = string("(x,y,z) from: ") +toString(x1)+string(",")+toString(y1)+string(",")+toString(z1)
                                                            +string(" to: ")   +toString(x2)+string(",")+toString(y2)+string(",")+toString(z2);
                                    linkIndexToNameLookup[linkCounter] = linkName;
									try
									{
										if(z2 < numSheets)
										{
											new (&links[linkCounter]) NetworkLink(
																														&nodes[y1*numNodesX*numSheets + x1*numSheets + z1],
																														&nodes[y2*numNodesX*numSheets + x2*numSheets + z2],
																														0.0
																														);
										}
										//LHZ - z2 will only = numSheets when recurrence is turned on - when it is, we should 
										//loop the link back to the input
										else
										{
											new (&links[linkCounter]) NetworkLink(
																														&nodes[y1*numNodesX*numSheets + x1*numSheets + z1],
																														&nodes[y2*numNodesX*numSheets + x2*numSheets],
																														0.0
																														);
										}
										
										linkCounter++;
									}
									catch (const char *c)
									{
										cout << c << endl;
										CREATE_PAUSE(string("Line Number: ")+toString(__LINE__));
									}
									catch (string s)
									{
										cout << s << endl;
										CREATE_PAUSE(string("Line Number: ")+toString(__LINE__));
									}
									catch (std::exception e)
									{
										cout << e.what() << endl;
										CREATE_PAUSE(string("Line Number: ")+toString(__LINE__));
									}
									catch (...)
									{
										cout << "AN EXCEPTION HAS OCCURED!\n";
										CREATE_PAUSE(string("Line Number: ")+toString(__LINE__));
									}
								}
							}
					}
				}
			}
		}
		
		cout << "Creating FastNetwork\n";
		
		substrate = NEAT::FastNetwork<double>(
																					nodes,
																					nodeCounter,
																					links,
																					linkCounter
																					);
		
		/*
		 while (!nodes.empty())
		 {
		 delete nodes[0];
		 nodes.erase(nodes.begin());
		 }
		 
		 while (!links.empty())
		 {
		 delete links[0];
		 links.erase(links.begin());
		 }
		 */
		cout << "done!\n";

		for (int a=0;a<nodeCounter;a++)
		{
			nodes[a].~NetworkNode();
		}
		
		for (int a=0;a<linkCounter;a++)
		{
			links[a].~NetworkLink();
		}
		
		free(nodes);
		free(links);
	}
	 
	void LegSwingPneatExperiment::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual)
	{
		NEAT::FastNetwork <double> network = individual->spawnFastPhenotypeStack<double>();
				
#ifdef HIDDEN_LAYER
		double cppnLinkWeightFromInputToHiddenSheet = 0.0;
		double cppnLinkWeightFromHiddenToOutputSheet = 0.0;
		double substrateLinkWeightFromInputToHiddenSheet = 0.0;
		double substrateLinkWeightFromHiddenToOutputSheet = 0.0;
#else
		double cppnLinkWeightFromInputToOutputSheet = 0.0;
		double substrateLinkWeightFromInputToOutputSheet = 0.0;
#endif 
#ifdef RECURRENT
		double cppnLinkWeightFromOutputToInputSheet = 0.0;
		double substrateLinkWeightFromOutputToInputSheet = 0.0;
#endif
		
		
#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
        static ofstream debugFile("LEGSWING_EXPERIMENT_PNEAT_DEBUG.txt");
		debugFile << "Populating substrate...\n";
#endif
		        
		//jmc in this NoGeom/P-NEAT/direct encoding control, the CPPN (network) and the subtrate (neural network we eventually test on the task) are the same. 
		//jmc I am pretty sure the evolution occurs on the CPPN, and here we create a substrate identical to the cppn, later we evaluate fitness on the substrate
		for (int y1=0;y1<numNodesY;y1++)
		{
			for (int x1=0;x1<numNodesX;x1++)
			{
				for (int y2=0;y2<numNodesY;y2++)
				{
					for (int x2=0;x2<numNodesX;x2++)
					{
						
						/*
						 cout << "Getting link from " <<
						 nameLookup[pair<int,int>(y1-numNodesY/2,x1-numNodesX/2)] <<
						 " to " <<
						 nameLookup[pair<int,int>(y2-numNodesY/2,x2-numNodesX/2)] << endl;
						 */
						
						Node fromInputNode (x1, y1, 0); // z=0 cause its the input layer
						
#ifdef HIDDEN_LAYER
						Node toHiddenNode  (x2, y2, 1); 
						Node fromHiddenNode(x1, y1, 1);
						Node toOutputNode  (x2, y2, 2);
						
#ifdef RECURRENT
						Node fromOutputNode (x1, y1, 2);
						Node toInputNode (x2, y2, 0);
#endif
						
#else 
						Node toOutputNode  (x2, y2, 1);
#ifdef RECURRENT
						Node fromOutputNode (x1, y1, 1);
						Node toInputNode (x2, y2, 0);
#endif
#endif
						
						//trying to make thing easier for P-Neat to make zeros
#ifdef HIDDEN_LAYER
						cppnLinkWeightFromInputToHiddenSheet  = network.getLink(nameLookup[fromInputNode],nameLookup[toHiddenNode])->weight;  //network = cppn 
						cppnLinkWeightFromHiddenToOutputSheet = network.getLink(nameLookup[fromHiddenNode],nameLookup[toOutputNode])->weight;  //network = cppn 
						                        
						substrateLinkWeightFromInputToHiddenSheet  = zeroOrNormalize(cppnLinkWeightFromInputToHiddenSheet);
						substrateLinkWeightFromHiddenToOutputSheet = zeroOrNormalize(cppnLinkWeightFromHiddenToOutputSheet);
						
						//set this new value to be the weight in the substrate (the final network we evaluate fitness on)   
						substrate.getLink(nameLookup[fromInputNode],nameLookup[toHiddenNode])->weight = substrateLinkWeightFromInputToHiddenSheet; 
						substrate.getLink(nameLookup[fromHiddenNode],nameLookup[toOutputNode])->weight = substrateLinkWeightFromHiddenToOutputSheet; 
						Node *ptr_toNode = &toHiddenNode;
						(void) ptr_toNode; //to get rid of unused variable warning
						
						
#else
						cppnLinkWeightFromInputToOutputSheet = network.getLink(nameLookup[fromInputNode],nameLookup[toOutputNode])->weight;  //network = cppn 
						substrateLinkWeightFromInputToOutputSheet = zeroOrNormalize(cppnLinkWeightFromInputToOutputSheet);
						
						substrate.getLink(nameLookup[fromInputNode],nameLookup[toOutputNode])->weight = substrateLinkWeightFromInputToOutputSheet; 
						Node *ptr_toNode = &toOutputNode;
#endif
						
#ifdef RECURRENT
						cppnLinkWeightFromOutputToInputSheet = network.getLink(nameLookup[fromOutputNode],nameLookup[toInputNode])->weight;
						substrateLinkWeightFromOutputToInputSheet = zeroOrNormalize(cppnLinkWeightFromOutputToInputSheet);
#endif
						
#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
						debugFile << "Setting substrate link from ( x1: " << setw(4) << x1
						<< ", y1: " << setw(4) << y1 << ") to "
						<< "( x2: " << setw(4) << x2
						<< ", y2: " << setw(4) << y2 << ")" << endl;
#endif
#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
						debugFile << " AB: " << setw(22) << setprecision(20) << substrate.getLink(nameLookup[fromInputNode],nameLookup[*ptr_toNode])->weight;
#ifdef HIDDEN_LAYER
						debugFile << " BC: " << setw(22) << setprecision(20) << substrate.getLink(nameLookup[fromHiddenNode],nameLookup[toOutputNode])->weight;
#endif
#ifdef RECURRENT
						debugFile << " RECUR: " << setw(22) << setprecision(20) << substrate.getLink(nameLookup[fromOutputNode],nameLookup[toInputNode])->weight;
#endif
						debugFile <<endl;
#endif
						
						
					}
				}
			}
		}
#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
		debugFile << "done!\n";
#endif

//        cout << "PERFORMING DARK MAGIC. COPYING NETWORK TO SUBSTRATE. CHANGE ME?" << endl;
//        PRINT(substrate.getNodeCount());
//        substrate = network;
//        PRINT(substrate.getNodeCount());
		
	}
	
	double LegSwingPneatExperiment::processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual, bool postHocRecord = false)
	{
		
		resetRelevantGlobals();
		if(dInitODE2(0) == 0) {
			cerr<<"ODE did not initialize correctly\n";
			exit(-1);
		}
		setDrawStuff(); 
		
		world        = dWorldCreate();
		space        = dHashSpaceCreate(0);
		contactgroup = dJointGroupCreate(0);
		ground       = dCreatePlane(space,0,0,1,0);
		dRandSetSeed(42);
		dWorldSetGravity(world, 0, 0, -9.8);
		dWorldSetCFM(world, 1e-4);
#ifdef USE_NLEG_ROBOT
		makeRobot_Nleg();
#else
		makeRobot(); 
#endif
		substrate_pointer = &substrate;
        static bool doOnce = true;
		if(doOnce){
			nameLookupGlobal.clear();
			nameLookupGlobal = nameLookup;
			doOnce = false;
		}

		
#ifndef NOVIZ
		if(visualize){
			if(postHocRecord){
				printf("generative movie files, patience....\n");
				dsSimulationLoop(0,NULL,800,480,&fn,true);
			}
			else	
				dsSimulationLoop(0,NULL,800,480,&fn);
		}
		else          
			while(shouldIKeepRunningTheSimulation()) simLoop(false);
		
#else
		while(shouldIKeepRunningTheSimulation()) simLoop(false);
#endif 
		
		dCloseODE();
		dSpaceDestroy(space);
		dWorldDestroy(world);
		if(postHocRecord)
		{
			std::system("rm frame0001.ppm"); // first frame is from the wrong viewpoint
		}; 
		
#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
		cout << "Trial Complete. "<< endl;
#endif
		
		//OVERRIDING FITNESS FOR INTERACTIVE EVOLUTION
#ifdef LEGSWING_INTERACTIVE_EVOLUTION
		if(rewardOrg) return 1000*rewardOrg;
		else return 1;
#endif
		
		if(numDirectionChanges>maxnumDirectionChanges) return 0.001;
		
		dReal fitness = ODE_fitness();
        if(visualize or postHoc) printf("****totalDistance %f: ", sqrt(log2(fitness)));
		if(visualize or postHoc) printf("****totalFitness %f: \n", fitness+1);

		printGeneticPertubationEffects(individual, fitness+1);
		
		return fitness;
		
	}
	
	
	void LegSwingPneatExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
	{
		shared_ptr<NEAT::GeneticIndividual> individual = group.front();
		/*{
		 cout << "Starting Evaluation on object:" << this << endl;
		 cout << "Running on individual " << individual << endl;
		 }*/

        static bool needToCheckParams = true;
        if(needToCheckParams)
        {
            //These values should be true for FT-NEAT 
            assert(NEAT::Globals::getSingleton()->getParameterValue("MutateAddNodeProbability") == 0.0);
            assert(NEAT::Globals::getSingleton()->getParameterValue("MutateAddLinkProbability") == 0.0);
            assert(NEAT::Globals::getSingleton()->getParameterValue("MutateDemolishLinkProbability") == 0.0);
            needToCheckParams=false;
        }

		
		//You get 1 point just for entering the game, wahooo!
		individual->setFitness(1);
		individual->setDirectionChanges(0);
		//individual->setUserData(shared_ptr<LegSwingPneatStats>(new LegSwingPneatStats()));
		
		populateSubstrate(individual);
		
		double fitness=0;
		
		double maxFitness = 1;  //starts at one because their fitness starts at one 
		
		fitness += processEvaluation(individual,false);
		maxFitness += 4200;
		
		individual->reward(fitness);
		individual->setDirectionChanges(numDirectionChanges);

		static bool haveReportedMaxFitness = false;
		if(!haveReportedMaxFitness){
			cout << "maxFitness is: " << maxFitness << endl;
			haveReportedMaxFitness = true;
		}
		
#ifdef LEGSWING_EXPERIMENT_PNEAT_DEBUG
		cout << "Individual Evaluation complete!\n";
		cout << "Fitness bump (does not include 1 free fitness point) was: " << fitness;
		cout << "maxFitness is: " << maxFitness << endl;
#endif
	}
	
	
	void LegSwingPneatExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
	{

		
		visualize = true;
//		visualize = false;
		postHoc = true;
		

		//You get 1 points just for entering the game, wahooo!
		individual->setFitness(1);
		individual->setDirectionChanges(0);
		//individual->setUserData(shared_ptr<LegSwingPneatStats>(new LegSwingPneatStats()));
				
		populateSubstrate(individual);
        printActualSubstrate();
        //printSubstrate();
		
		double fitness=0;
		
		double maxFitness = 1;
		
		int testCases=0;
		
		testCases++;
		
#ifdef POSTHOC_RECORD_PNEAT
		printf("generating movie files. patience."); 
		fitness += processEvaluation(individual,true);
		//std::system("convert -quality 100 *.ppm run.mpeg");
		//std::system("rm *.ppm");
#else
		fitness += processEvaluation(individual,NULL);
#endif
		
		
		maxFitness += 42;
		
		cout << "TOTAL TEST CASES: " << testCases << endl;
		
		cout << "Individual Evaluation complete!\n";
		
		cout << maxFitness << endl;
		
		individual->reward(fitness);
		individual->setDirectionChanges(numDirectionChanges);
		
		if (fitness >= maxFitness*.95)
		{
			cout << "PROBLEM DOMAIN SOLVED!!!\n";
		}
	}
	
	
	void LegSwingPneatExperiment::printSubstrate()
	{
		cout << "Printing substrate" << endl;
		ofstream substrate_file;        
		substrate_file.open ("substrateThatSolvedTheProblem.txt", ios::trunc );
		
		cout << "num links: " <<substrate.getLinkCount() << endl;
		substrate_file << "num links: " <<substrate.getLinkCount() << endl;
	
        cout << "num nodes: " << substrate.getNodeCount() << endl;
        substrate_file << "num nodes1: " << substrate.getNodeCount() << endl;
            	
		int counter=0;
		for (int y1=0;y1<numNodesY;y1++)
		{
			for (int x1=0;x1<numNodesX;x1++)
			{
				for (int y2=0;y2<numNodesY;y2++)
				{
					for (int x2=0;x2<numNodesX;x2++)
					{
						if(counter > substrate.getLinkCount()){ cout << "error: you are trying to print a link that does not exist" << endl; exit(1);}
						
						substrate_file << "from (x1,y1) to (x2,y2): (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << "): " ;
						substrate_file << substrate.getLink(counter)->weight << endl;
						counter++;
					}
				}
			}
		}
		substrate_file.close();
		
		return;
	}
	void LegSwingPneatExperiment::printActualSubstrate()
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
              if(counter > substrate.getLinkCount()){ cout << "error: you are trying to print a link that does not exist" << endl; exit(1);}
//              substrate_file << "counter: " << counter << " " << linkIndexToNameLookup[counter] << " :" << substrate.getLink(counter)->fromNode << " --> " << substrate.getLink(counter)->toNode << ": ";
              substrate_file << "link num: " << counter << " " << linkIndexToNameLookup[counter] << " : ";
              
              substrate_file << substrate.getLink(counter)->weight << endl;
		}
		substrate_file.close();
		
		return;
	}
	
	void LegSwingPneatExperiment::printCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
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
		
		for (size_t a=0;a<numLinks;a++)
		{
			NetworkIndexedLink<double> link = *network.getLink(a);
			
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
	
	Experiment* LegSwingPneatExperiment::clone()
	{
		cout << "Experiment cloned\n";
		LegSwingPneatExperiment* experiment = new LegSwingPneatExperiment(*this);
		
		cout << this << " cloned to " << experiment << endl;
		
		return experiment;
	}
	
	double LegSwingPneatExperiment::zeroOrNormalize(double mWeightToAlter){  //FT-NEAT genome links are -3 to 3, so no need to scale them up, and we are not zeroing out, so no need to do that. 
		return mWeightToAlter;
	}
}
#endif //COMPILE_ODE
