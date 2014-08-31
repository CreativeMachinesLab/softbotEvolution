// code based on legged.cpp by Kosei Demura (2007-5-19) (demura.net)

#ifdef COMPILE_ODE  
#include "Experiments/HCUBE_LegSwingExperiment.h"

#define POSTHOC_RECORD
//#define LEGSWING_INTERACTIVE_EVOLUTION
#define EUCLIDEAN_DISTANCE_FITNESS
#define DOUBLE

namespace HCUBE
{
	
	using namespace NEAT;
	
	LegSwingExperiment::LegSwingExperiment(string _experimentName)
	:	Experiment(_experimentName),
        numNodesX(numNodesXglobal),
        numNodesY(numNodesYglobal)
	{
		
		simulation_steps = (int) NEAT::Globals::getSingleton()->getParameterValue("SimulationSteps");
				
		convergence_step = (int) NEAT::Globals::getSingleton()->getParameterValue("ConvergenceStep");
		
		if(convergence_step < 0)
			convergence_step = INT_MAX; // never switch

		dimensionality = int(NEAT::Globals::getSingleton()->getParameterValue("dimensionality"));

		experimentType = int(NEAT::Globals::getSingleton()->getParameterValue("ExperimentType"));

#ifdef USE_NLEG_ROBOT
		hingeType = (int) NEAT::Globals::getSingleton()->getParameterValue("TorsoHingeType");
#endif
		
		fitness_function = (int) NEAT::Globals::getSingleton()->getParameterValue("FitnessFunction");		
		
		if(NEAT::Globals::getSingleton()->getParameterValue("LimitLegSwings") == 1) {
			maxnumDirectionChanges = int(.16*simulation_steps*(static_cast<double>(num_legs*BODY_SEGMENTS)/4.0));
		}
		else {
			maxnumDirectionChanges = INT_MAX;
		}
		
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
		
		generateSubstrate();
		
	}
	
	
	NEAT::GeneticPopulation* LegSwingExperiment::createInitialPopulation(int populationSize)
	{

		GeneticPopulation *population = new GeneticPopulation();
		vector<GeneticNodeGene> genes;
		
		genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("X1","NetworkSensor",0,false));
		if(dimensionality>=2){
			genes.push_back(GeneticNodeGene("Y1","NetworkSensor",0,false));
		}
		genes.push_back(GeneticNodeGene("X2","NetworkSensor",0,false));
		if(dimensionality>=2){
			genes.push_back(GeneticNodeGene("Y2","NetworkSensor",0,false));
		}
		if(dimensionality==3 or dimensionality==4){
			genes.push_back(GeneticNodeGene("Z1","NetworkSensor",0,false));
			genes.push_back(GeneticNodeGene("Z2","NetworkSensor",0,false));
		}

		genes.push_back(GeneticNodeGene("Output_ab","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
#ifdef HIDDEN_LAYER
		genes.push_back(GeneticNodeGene("Output_bc","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
#endif
		
		
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
		genes.push_back(GeneticNodeGene("Bias_b","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		genes.push_back(GeneticNodeGene("Bias_c","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
#endif
		for (int a=0;a<populationSize;a++)
		{
			shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,true,1.0));
			
			for (int b=0;b<0;b++)  // this loop will never be entered?  Why is it here?
			{
				individual->testMutate();
			}
			
			population->addIndividual(individual);
		}
		
		cout << "Finished creating population\n";
		return population;
	}
	
	void LegSwingExperiment::generateSubstrate()
	{
		cout << "Generating substrate...";
		boost::object_pool<NEAT::NetworkNode> networkNodePool;
		boost::object_pool<NEAT::NetworkLink> networkLinkPool;
		
//		int numSheets;
		
#ifdef HIDDEN_LAYER
		numSheets = 3;
#else
		numSheets = 2;
#endif
		
		NEAT::NetworkNode *nodes = NULL;
		NEAT::NetworkLink *links = NULL;
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
		double *nodeBiases = NULL;
#endif
		try
		{			
			nodes = (NEAT::NetworkNode*)malloc(sizeof(NEAT::NetworkNode)*numNodesX*numNodesY*numSheets+sizeof(NEAT::NetworkNode));			
			links = (NEAT::NetworkLink*)malloc(sizeof(NEAT::NetworkLink)*numNodesX*numNodesY*numNodesX*numNodesY*(numSheets-1)+(20*sizeof(NEAT::NetworkLink)));

#if LEGSWING_EXPERIMENT_ENABLE_BIASES
			nodeBiases = new double[numNodesX*numNodesY*numSheets];
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
		
		//#ifdef LEGSWING_EXPERIMENT_DEBUG
		//		static ofstream debugFile("LEGSWING_EXPERIMENT_DEBUG.txt");
		//#endif
		
		for (int y1=0;y1<numNodesY;y1++)
		{
			for (int x1=0;x1<numNodesX;x1++)
			{
				for (int z=0;z<numSheets;z++)
				{
					Node node(x1,y1,z);
					//#ifdef LEGSWING_EXPERIMENT_DEBUG
					//					debugFile << "creating node (" << x1 << "," << y1 << "," << z << ") with name " << (toString(x1)+string("/")+toString(y1) + string("/") + toString(z)) << endl;
					//#endif
					nameLookup[node] = (toString(x1)+string("/")+toString(y1) + string("/") + toString(z));
					new(&nodes[nodeCounter]) NetworkNode(nameLookup[node]);
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
					nodeBiases[nodeCounter]=0.0;
#endif
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
						for (int z1=0;z1<(numSheets - 1);z1++)
						{
							int z2 = z1+1;
							
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
		

		
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
		cout << "Creating FastBiasNetwork\n";
		substrate = NEAT::FastBiasNetwork<double>(nodes,nodeCounter,links,linkCounter,nodeBiases);
#else
		cout << "Creating FastNetwork\n";
		substrate = NEAT::FastNetwork<double>(nodes,nodeCounter,links,linkCounter);
#endif
		
		
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
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
		delete[] nodeBiases;
#endif
	}
	
	NEAT::FastNetwork<double> LegSwingExperiment::populateAndReturnSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual) {
		populateSubstrate(individual, true);
		return substrate;
	}
	
	void LegSwingExperiment::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual, bool toPrint)
	{
		
#ifdef HIDDEN_LAYER
		double cppnLinkWeightFromInputToHiddenSheet = 0.0;
		double cppnLinkWeightFromHiddenToOutputSheet = 0.0;
		double substrateLinkWeightFromInputToHiddenSheet = 0.0;
		double substrateLinkWeightFromHiddenToOutputSheet = 0.0;
#else
		double cppnLinkWeightFromInputToOutputSheet = 0.0;
		double substrateLinkWeightFromInputToOutputSheet = 0.0;
#endif
		
		
		
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
		NEAT::FastBiasNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
#else
		NEAT::FastNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
#endif
		
			
		int linkCounter=0;
		for (int y1=0;y1<numNodesY;y1++)
		{
			for (int x1=0;x1<numNodesX;x1++)
			{
				for (int y2=0;y2<numNodesY;y2++)
				{
					for (int x2=0;x2<numNodesX;x2++)
					{
					    double x1normal;
						double x2normal;
						double y1normal;
						double y2normal;

						x1normal = normalize(x1,numNodesX);
						x2normal = normalize(x2,numNodesX);
						y1normal = normalize(y1,numNodesY);
						y2normal = normalize(y2,numNodesY);
							
													
					  //now set the cppn values
						network.reinitialize();
						network.setValue("X1",x1normal);
						network.setValue("X2",x2normal);
						network.setValue("Y1",y1normal);
						network.setValue("Y2",y2normal);
												
						network.setValue("Bias",0.3);
						
						network.update();
						
						
#ifdef HIDDEN_LAYER
						cppnLinkWeightFromInputToHiddenSheet = double(network.getValue("Output_ab"));						
						cppnLinkWeightFromHiddenToOutputSheet = double(network.getValue("Output_bc"));
#else  //no hidden
						cppnLinkWeightFromInputToOutputSheet = double(network.getValue("Output_ab"));						
#endif						
						
						
#ifdef LEGSWING_EXPERIMENT_DEBUG
						if(toPrint) {
							debugFile << "Setting substrate link from ( x1: " << setw(4) << x1
							<< ", y1: " << setw(4) << y1 << ") to "
							<< "( x2: " << setw(4) << x2
							<< ", y2: " << setw(4) << y2 << ")" << endl;
						}
#endif
						//cout << endl;
						
						Node fromInputNode (x1, y1, 0); // z=0 cause its the input layer
						Node *ptr_toNode;
						
#ifdef HIDDEN_LAYER
						Node toHiddenNode  (x2, y2, 1); 
						Node fromHiddenNode(x1, y1, 1);
						Node toOutputNode  (x2, y2, 2);
						
						Node fromOutputNode (x1, y1, 2);
						Node toInputNode (x2, y2, 0);
						ptr_toNode = &toHiddenNode;
						
#else
						Node toOutputNode  (x2, y2, 1);
						Node fromOutputNode (x1, y1, 1);
						Node toInputNode (x2, y2, 0);
						ptr_toNode = &toOutputNode;
#endif

#ifdef LEGSWING_EXPERIMENT_DEBUG
//						debugFile << "Setting substrate link from ( x1: " << setw(4) << x1
//						<< ", y1: " << setw(4) << y1 << ") to "
//						<< "( x2: " << setw(4) << x2
//						<< ", y2: " << setw(4) << y2 << ")" << endl;
						/*
						debugFile << "Setting substrate link x/y/z AB from: " << setw(4) << nameLookup[fromInputNode]
						<< " to " << nameLookup[*ptr_toNode]
						<< " to " << nameLookup[toHiddenNode]
                        << " and BC from: " << nameLookup[fromHiddenNode] << " to " << nameLookup[toOutputNode] << endl;
*/
#endif
						
						//assert

						//get the cppn output and set it to the relevant link the substrate
#ifdef HIDDEN_LAYER
						substrateLinkWeightFromInputToHiddenSheet  = zeroOrNormalize(cppnLinkWeightFromInputToHiddenSheet);
						substrateLinkWeightFromHiddenToOutputSheet = zeroOrNormalize(cppnLinkWeightFromHiddenToOutputSheet);
						substrate.getLink(nameLookup[fromInputNode],nameLookup[*ptr_toNode])->weight = substrateLinkWeightFromInputToHiddenSheet;
						linkCounter++;
						substrate.getLink(nameLookup[fromHiddenNode],nameLookup[toOutputNode])->weight = substrateLinkWeightFromHiddenToOutputSheet;
						linkCounter++;
#else
						substrateLinkWeightFromInputToOutputSheet = zeroOrNormalize(cppnLinkWeightFromInputToOutputSheet);
						substrate.getLink(nameLookup[fromInputNode],nameLookup[*ptr_toNode])->weight = substrateLinkWeightFromInputToOutputSheet;
						linkCounter++;
#endif

						if(toPrint) {
#ifdef LEGSWING_EXPERIMENT_DEBUG
						
							debugFile << " AB: " << setw(22) << setprecision(20) << substrate.getLink(nameLookup[fromInputNode],nameLookup[*ptr_toNode])->weight;
#ifdef HIDDEN_LAYER
							debugFile << " BC: " << setw(22) << setprecision(20) << substrate.getLink(nameLookup[fromHiddenNode],nameLookup[toOutputNode])->weight;
#endif
#endif
						}
						
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
						if (x2==0&&y2==0)
						{
							double nodeBias;
							
							nodeBias = double(network.getValue("Bias_b"));
							
							substrate.setBias(
																nameLookup[Node(x1,y1,1)],
																nodeBias
																);
#ifdef LEGSWING_EXPERIMENT_DEBUG                  
							if(toPrint)
								debugFile << " biasB: " << nodeBias;
#endif
							
							nodeBias = double(network.getValue("Bias_c"));
							
							
							substrate.setBias(
																nameLookup[Node(x1,y1,2)],
																nodeBias
																);
#ifdef LEGSWING_EXPERIMENT_DEBUG                                              
							if(toPrint)
								debugFile << " biasC: " << nodeBias;
#endif
						}
#endif
						
#ifdef LEGSWING_EXPERIMENT_DEBUG                                              
						if(toPrint)
							debugFile << endl;
#endif
					}
				}
			}
		}
		//exit(3); //@end
#ifdef LEGSWING_EXPERIMENT_DEBUG
		if(toPrint)
			debugFile << "done!\n";
#endif
	

  
	}
  
       
	double LegSwingExperiment::processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual, bool postHocRecord = false)
	
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
		hingeType = (int) NEAT::Globals::getSingleton()->getParameterValue("TorsoHingeType");
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
            {
				dsSimulationLoop(0,NULL,800,480,&fn);
            }
		}
		else          
			while(shouldIKeepRunningTheSimulation()) simLoop(false);
		
#else
		while(shouldIKeepRunningTheSimulation()) simLoop(false);
#endif 
		
		dCloseODE(); // should be after dWorldDestroy
		dSpaceDestroy(space);
		dWorldDestroy(world);
		if(postHocRecord)
		{
			std::system("rm frame0001.ppm"); // first frame is from the wrong viewpoint
		}; 
		
#ifdef LEGSWING_EXPERIMENT_DEBUG
		cout << "Trial Complete. "<< endl;
#endif
		
		//OVERRIDING FITNESS FOR INTERACTIVE EVOLUTION
#ifdef LEGSWING_INTERACTIVE_EVOLUTION
		if(rewardOrg) return 1000*rewardOrg;
		else return 1;
#endif
		
		if(numDirectionChanges>maxnumDirectionChanges) return 0.001;
		
		dReal fitness = ODE_fitness();
		if(visualize or postHoc) printf("****totalFitness %f: \n", fitness+1);

		printGeneticPertubationEffects(individual, fitness+1);

		
		
		return fitness;
	}
	
	void LegSwingExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
	{
		shared_ptr<NEAT::GeneticIndividual> individual = group.front();
		/*{
		 cout << "Starting Evaluation on object:" << this << endl;
		 cout << "Running on individual " << individual << endl;
		 }*/
		
		//You get 1 point just for entering the game, wahooo!
		individual->setFitness(1);
		//individual->setUserData(shared_ptr<LegSwingStats>(new LegSwingStats()));
		
		populateSubstrate(individual);
		
		double fitness=0;
		
		double maxFitness = 1;  //starts at one because their fitness starts at one 
		
		fitness += processEvaluation(individual,NULL);
		
		//cout<<"YOU ARE NOT EVALUATING CREATURE!!!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
		//fitness++;
		
		maxFitness += 4200;
		
		individual->reward(fitness); //TODO: You are bumping fitness twice, giving you incorrect scores in the genchamp file, no?
		individual->setDirectionChanges(numDirectionChanges);
		
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
	
	void LegSwingExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
	{
		visualize = true;
		//visualize = false;
		postHoc = true;       
		
		
		//You get 1 points just for entering the game, wahooo!
		individual->setFitness(1);
		//individual->setUserData(shared_ptr<LegSwingStats>(new LegSwingStats()));
		
		
		populateSubstrate(individual);
		printActualSubstrate();
		
		
		double fitness=0;
		
		double maxFitness = 1;
		
		int testCases=0;
		
		testCases++;
		
#ifdef POSTHOC_RECORD
		printf("generating movie files. patience."); 
		fitness += processEvaluation(individual,true);
		std::system("convert -quality 100 *.ppm run.mpeg");
		std::system("rm *.ppm");
#else
		fitness += processEvaluation(individual,NULL);
#endif
		
		
		maxFitness += 42;
		
		cout << "TOTAL TEST CASES: " << testCases << endl;
		
		cout << "Individual Evaluation complete!\n";
		
		cout << maxFitness << endl;
		
		individual->reward(fitness);
		individual->setDirectionChanges(numDirectionChanges);
		
		
		//if (fitness >= maxFitness*.95)
		//{
		//		cout << "PROBLEM DOMAIN SOLVED!!!\n";
		//}
	}
	
	Experiment* LegSwingExperiment::clone()
	{
		LegSwingExperiment* experiment = new LegSwingExperiment(*this);
		
		return experiment;
	}
	
	void LegSwingExperiment::printSubstrate()
	{
		cout << "Printing substrate" << endl;
		cout << "JMC: I think this function was hard coded for a 2-sheeted setup and does not work well. Use PrintActualSubstrate() instead." << endl;
		exit(3);
		ofstream substrate_file;        
		substrate_file.open ("substrateThatSolvedTheProblem.txt", ios::trunc );
		
		cout << "num links: " <<substrate.getLinkCount() << endl;
		substrate_file << "num links: " <<substrate.getLinkCount() << endl;
		
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

	void LegSwingExperiment::printActualSubstrate()
	{
		cout << "Printing substrate" << endl;
		ofstream substrate_file;        
		substrate_file.open ("substrateThatSolvedTheProblem.txt", ios::trunc );
		
        int numLinks = substrate.getLinkCount();
		//cout << "num links: " << numLinks << endl;
		//substrate_file << "num links: " << numLinks << endl;
	
        //cout << "num nodes: " << substrate.getNodeCount() << endl;
        //substrate_file << "num nodes: " << substrate.getNodeCount() << endl;


		for (int counter=0;counter<numLinks;counter++)
		{
              if(counter > substrate.getLinkCount()){ cout << "error: you are trying to print a link that does not exist" << endl; exit(1);}
              //substrate_file << "counter: " << counter << " " << linkIndexToNameLookup[counter] << " :" << substrate.getLink(counter)->fromNode << " --> " << substrate.getLink(counter)->toNode << ": ";
              //substrate_file << "link num: " << counter << " " << linkIndexToNameLookup[counter] << " : ";
              
              substrate_file << substrate.getLink(counter)->weight << endl;
		}
		substrate_file.close();
		
		return;
	}
    	
	double LegSwingExperiment::normalize(const int & r_xyVal, const int & r_numNodesXorY) 
	{
		// turn the xth or yth node into its coordinates on a grid from -1 to 1, e.g. x values (1,2,3,4,5) become (-1, -.5 , 0, .5, 1)
		// this works with even numbers, and for x or y grids only 1 wide/tall, which was not the case for the original
		// e.g. see findCluster for the orignal versions where it was not a funciton and did not work with odd or 1 tall/wide #s
		double coord;
		
		double increment = 2.0/(r_numNodesXorY-1);
		
		if(r_numNodesXorY==1) coord = 0;
		else                  coord = -1+(r_xyVal*increment);
		
		return(coord);    
	}
	
	
	void LegSwingExperiment::printCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
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
	
	double LegSwingExperiment::zeroOrNormalize(double mWeightToAlter){
		assert(-1.0 <= mWeightToAlter && mWeightToAlter<= 1.0 );  // if this fails then the normalization is not valid.
		return mWeightToAlter * 3.0;
	}
	
	bool LegSwingExperiment::converged(int generation) {
		if(generation == convergence_step)
			return true;
		return false;
	}	
}
#endif //COMPILE_ODE
