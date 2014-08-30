// code based on legged.cpp by Kosei Demura (2007-5-19) (demura.net)
  
#include "Experiments/HCUBE_SpiderRobotExperiment.h"

#define POSTHOC_RECORD
//#define SPIDERROBOT_INTERACTIVE_EVOLUTION
#define EUCLIDEAN_DISTANCE_FITNESS
#define DOUBLE


#define HIDDEN_LAYER


#include "HCUBE_Defines.h"

#define dDOUBLE //using ode double precision  



#define LEGSWING_ODE_DEBUG (0)

//////////

namespace HCUBE
{
	
	using namespace NEAT;
	
	SpiderRobotExperiment::SpiderRobotExperiment(string _experimentName)
	:	Experiment(_experimentName),
        numNodesX(3),
        numNodesY(4),
		numSheets(3)
	{
						
		convergence_step = (int) NEAT::Globals::getSingleton()->getParameterValue("ConvergenceStep");
		
		if(convergence_step < 0)
			convergence_step = INT_MAX; // never switch

//#ifdef USE_NLEG_ROBOT
//		hingeType = (int) NEAT::Globals::getSingleton()->getParameterValue("TorsoHingeType");
//#endif
				
		
		generateSubstrate();
		
	}
	
	
	NEAT::GeneticPopulation* SpiderRobotExperiment::createInitialPopulation(int populationSize)
	{
    
		GeneticPopulation *population = new GeneticPopulation();
		vector<GeneticNodeGene> genes;
		
		genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("X1","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("Y1","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("X2","NetworkSensor",0,false));
		genes.push_back(GeneticNodeGene("Y2","NetworkSensor",0,false));

		genes.push_back(GeneticNodeGene("Output_ab","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
#ifdef HIDDEN_LAYER
		genes.push_back(GeneticNodeGene("Output_bc","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
#endif
		
#ifdef RECURRENT
		genes.push_back(GeneticNodeGene("Output_recurrence", "NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
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
	
	void SpiderRobotExperiment::generateSubstrate()
	{
		cout << "Generating substrate...";
		boost::object_pool<NEAT::NetworkNode> networkNodePool;
		boost::object_pool<NEAT::NetworkLink> networkLinkPool;
		
		
		
		NEAT::NetworkNode *nodes = NULL;
		NEAT::NetworkLink *links = NULL;
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
		double *nodeBiases = NULL;
#endif
		try
		{			

//#define USE_EXTRA_NODE
#ifndef USE_EXTRA_NODE
			nodes = (NEAT::NetworkNode*)malloc(sizeof(NEAT::NetworkNode)*numNodesX*numNodesY*numSheets);
			
#ifdef RECURRENT
			links = (NEAT::NetworkLink*)malloc(sizeof(NEAT::NetworkLink)*numNodesX*numNodesY*numNodesX*numNodesY*(numSheets));

#else
			links = (NEAT::NetworkLink*)malloc(sizeof(NEAT::NetworkLink)*numNodesX*numNodesY*numNodesX*numNodesY*(numSheets -1));
#endif
			
#else
			nodes = (NEAT::NetworkNode*)malloc(sizeof(NEAT::NetworkNode)*numNodesX*numNodesY*numSheets+sizeof(NEAT::NetworkNode));
			
#ifdef RECURRENT
			links = (NEAT::NetworkLink*)malloc(sizeof(NEAT::NetworkLink)*numNodesX*numNodesY*numNodesX*numNodesY*(numSheets)+(20*sizeof(NEAT::NetworkLink)));
#else
			links = (NEAT::NetworkLink*)malloc(sizeof(NEAT::NetworkLink)*numNodesX*numNodesY*numNodesX*numNodesY*(numSheets-1)+(20*sizeof(NEAT::NetworkLink)));
#endif

#endif

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
		
		//#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
		//		static ofstream debugFile("SPIDERROBOT_EXPERIMENT_DEBUG.txt");
		//#endif
		
		for (int y1=0;y1<numNodesY;y1++)
		{
			for (int x1=0;x1<numNodesX;x1++)
			{
				for (int z=0;z<numSheets;z++)
				{
					Node node(x1,y1,z);
					//#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
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




#ifdef USE_EXTRA_NODE
		Node node(5,3,0);
		nameLookup[node] = (toString(5)+string("/")+toString(3) + string("/") + toString(0));
		cout << "\n\n***The name of the extra node is: " << nameLookup[node] << endl << endl; 

		new(&nodes[nodeCounter]) NetworkNode(nameLookup[node]);

#if LEGSWING_EXPERIMENT_ENABLE_BIASES
		nodeBiases[nodeCounter]=0.0;
#endif		

		nodeCounter++;
#endif





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
				    for (int z1=0;z1<(numSheets - 1);z1++)
#endif
				    {
					int z2 = z1+1;
					//for (int z2=z1+1;z2<=z1+1;z2++)
					{
					    //#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
					    //debugFile << "creating link no. " << linkCounter << " which is ( " << y1 << "," << x1 << "," << z1 << ") to ( " << y2 << "," << x2 << "," << z2 << ")\n";   
					    
					    //#endif
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
		


#ifdef USE_EXTRA_NODE

		
		for (int y = 0; y < 4; y++)
		{
		    for (int x = 0; x < 5; x++)
		    {
			
			try
			{

			    //cout << "creating link no. " << linkCounter << " which is ( " << 5 << "," << 3 << "," << 0 << ") to ( " << y << "," << x << "," << 1 << ")\n";   

			new (&links[linkCounter]) NetworkLink(
			    &nodes[5*numNodesX*numSheets + 3*numSheets + 0],
			    &nodes[y*numNodesX*numSheets + x*numSheets + 1],
			    0.0
			    );

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
		
#endif


		//#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
		//		debugFile.close();
		//#endif
		
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
		cout << "Creating FastBiasNetwork\n";
		substrate = NEAT::FastBiasNetwork<double>(
																							nodes,
																							nodeCounter,
																							links,
																							linkCounter,
																							nodeBiases
																							);
#else
		cout << "Creating FastNetwork\n";
		substrate = NEAT::FastNetwork<double>(
																					nodes,
																					nodeCounter,
																					links,
																					linkCounter
																					);
#endif
		
		
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
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
		delete[] nodeBiases;
#endif
	}
	
	NEAT::FastNetwork<double> SpiderRobotExperiment::populateAndReturnSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual) {
		populateSubstrate(individual, true);
		return substrate;
	}
	
	void SpiderRobotExperiment::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual, bool toPrint)
	{
#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
		static ofstream debugFile("SPIDERROBOT_EXPERIMENT_DEBUG.txt");
		
		if(toPrint) {
			//			static ofstream debugFile("SPIDERROBOT_EXPERIMENT_DEBUG.txt");w
			debugFile << "Populating substrate...\n";
		}
#endif
	
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
		
		////////////////////////////////////////////
		//NEAT::FastNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
		
		
		
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
		NEAT::FastBiasNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
#else
		NEAT::FastNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
#endif
		
		//printCPPN(individual);
		
		//<start> use this if evolving the period of the sine
		//int xInt =numNodesX+10; //I am setting this to be off the grid, so this node will not be constrained (as much)
		//// by what they are doing in the geometric region that the substrate network is formed from
		//int yInt =numNodesY+10;   
		//int x1norm = xInt;
		//int y1norm = yInt;
		//xInt = numNodesX+11;
		//yInt = numNodesY+11;
		//int x2norm = xInt;
		//int y2norm = yInt;
		//
		//network.reinitialize();
		//network.setValue("X1",x1norm);
		//network.setValue("Y1",y1norm);
		//network.setValue("X2",x2norm);
		//network.setValue("Y2",y2norm);
		//network.setValue("Bias",0.3);
		//network.update();
		//timeStepDivisor = 1000.0*double(network.getValue("Output_ab"));
		//if(visualize)printf("just got a timeStepDivisor: %f\n", timeStepDivisor);
		//<end>
		////////////////////////////////////////////
		
		
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
						
#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
						if(toPrint) {
							debugFile << "Setting CPPN input values to X1: " << setw(4) << x1normal
							<< " X2: " << setw(4) << x2normal;
							if(dimensionality>=2){
								debugFile << " Y1: " << setw(4) << y1normal
								<< " Y2: " << setw(4) << y2normal;
							}
							if(dimensionality==3 or dimensionality==4){
								debugFile << " Z1: " << setw(4) << z1normal
								<< " Z2: " << setw(4) << z2normal;
							}
							debugFile << endl;
						}
#endif
						
						network.setValue("Bias",0.3);
#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
						//						debugFile << " Bias: " << setw(2) << " .3";
#endif
						network.update();
						
						
#ifdef HIDDEN_LAYER
						cppnLinkWeightFromInputToHiddenSheet = double(network.getValue("Output_ab"));						
						cppnLinkWeightFromHiddenToOutputSheet = double(network.getValue("Output_bc"));
#else  //no hidden
						cppnLinkWeightFromInputToOutputSheet = double(network.getValue("Output_ab"));						
#endif						
#ifdef RECURRENT
						cppnLinkWeightFromOutputToInputSheet = double(network.getValue("Output_recurrence"));
#endif
						
						
#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
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

#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
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
#ifndef USE_EXTRA_NODE
#ifdef RECURRENT
						//fixme: make me
#else
						if ((substrate.getLink(linkCounter)) == substrate.getLink(nameLookup[fromInputNode],nameLookup[*ptr_toNode])) {}
						else {cout << "not the same" << endl; exit(9);}
#endif
#endif
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
#ifdef RECURRENT
						substrateLinkWeightFromOutputToInputSheet = zeroOrNormalize(cppnLinkWeightFromOutputToInputSheet);
						substrate.getLink(nameLookup[fromOutputNode],nameLookup[toInputNode])->weight = substrateLinkWeightFromOutputToInputSheet;
#endif
						if(toPrint) {
#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
						
							debugFile << " AB: " << setw(22) << setprecision(20) << substrate.getLink(nameLookup[fromInputNode],nameLookup[*ptr_toNode])->weight;
#ifdef HIDDEN_LAYER
							debugFile << " BC: " << setw(22) << setprecision(20) << substrate.getLink(nameLookup[fromHiddenNode],nameLookup[toOutputNode])->weight;
#endif
#ifdef RECURRENT
							debugFile << " RECUR: " << setw(22) << setprecision(20) << substrate.getLink(nameLookup[fromOutputNode],nameLookup[toInputNode])->weight;
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
#ifdef SPIDERROBOT_EXPERIMENT_DEBUG                  
							if(toPrint)
								debugFile << " biasB: " << nodeBias;
#endif
							
							nodeBias = double(network.getValue("Bias_c"));
							
							
							substrate.setBias(
																nameLookup[Node(x1,y1,2)],
																nodeBias
																);
#ifdef SPIDERROBOT_EXPERIMENT_DEBUG                                              
							if(toPrint)
								debugFile << " biasC: " << nodeBias;
#endif
						}
#endif
						
#ifdef SPIDERROBOT_EXPERIMENT_DEBUG                                              
						if(toPrint)
							debugFile << endl;
#endif
					}
				}
			}
		}
		//exit(3); //@end
#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
		if(toPrint)
			debugFile << "done!\n";
#endif
	

  
#ifdef USE_EXTRA_NODE
		for (int y2 = 0; y2 < numNodesY; y2++)
		  {
		    for (int x2 = 0; x2 < numNodesX; x2++)
		      {
			int 
			  x1 = 3,
			  y1 = 5;
			
			double 
			  x1normal,
			  x2normal,
			  y1normal,
			  y2normal;
			
			x1normal = normalize(x1,numNodesX);
			x2normal = normalize(x2,numNodesX);
			y1normal = normalize(y1,numNodesY);
			y2normal = normalize(y2,numNodesY);
			
			network.reinitialize();
			network.setValue("X1",x1normal);
			network.setValue("X2",x2normal);
			network.setValue("Y1",y1normal);
			network.setValue("Y2",y2normal);
			
			network.setValue("Bias",0.3);
			
			network.update();
			
			double cppnLinkWeightFromInputToHiddenSheet = double(network.getValue("Output_ab"));
	  
			Node fromInputNode (x1, y1, 0);
			Node toHiddenNode  (x2, y2, 1); 
			
			substrateLinkWeightFromInputToHiddenSheet  = zeroOrNormalize(cppnLinkWeightFromInputToHiddenSheet);
			substrate.getLink(nameLookup[fromInputNode],nameLookup[toHiddenNode])->weight = substrateLinkWeightFromInputToHiddenSheet;
			linkCounter++;
			
		      }
		  }
#endif
	}
  
       
	double SpiderRobotExperiment::processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual, bool postHocRecord = false)
	
	{
		
//		substrate_pointer = &substrate; //delthis? and move everything to substrate?
		
		double substrateOutputVal;
		double sineOfTimeStep;
		
		
		//create empty inputs 2D vector
		vector <double> rowVector; 
		vector <vector <double> > inputs;
		for(int col=0; col< numNodesX;col++)
		{
			rowVector.push_back(0.0);
		}
		for(int row=0;row< numNodesY;row++)
		{
			inputs.push_back(rowVector);
		}
			
		ofstream jangle_file;        
		jangle_file.open ("spiderJointAngles.txt", ios::trunc );
		
		for(int stepNum=0;stepNum < 480;stepNum++)
		{
			substrate.reinitialize();
			substrate.dummyActivation();


			sineOfTimeStep = sin(double(stepNum)/6.0)*M_PI;
			inputs[0][2] = sineOfTimeStep; //TODO add sine wave value here 
			inputs[1][2] = 0.0; //this value remains zero
			inputs[2][2] = 0.0; //this value remains zero
			inputs[3][2] = 0.0; //this value remains zero

			//set the values to the substrate
			for(int row=0;row< numNodesY;row++) //for legs
			{
				for(int col=0; col< numNodesX;col++) //for columns in a row (first col is inner joint, second is outer, third is sine for the first row)
				{									
					//set angles from previous iteration
					substrate.setValue(nameLookup[HCUBE::Node(col,row,0)], inputs[row][col]);
				}
			}
			
			cout << "inputting...." << endl;
			print_vector_of_vectors(inputs);
			
			substrate.update(numSheets-1);

			
			cout << "output joints" << endl;
			for(int row=0;row< numNodesY;row++) //for legs
			{
				for(int col=0; col< numNodesX;col++) //for columns in a row (first col is inner joint, second is outer, third is sine for the first row)
				{									
					//set angles from previous iteration
					substrateOutputVal = substrate.getValue(nameLookup[HCUBE::Node(col,row,2)]);
					if(col <=1) //only print the leg values
					{
						jangle_file << substrateOutputVal << " ";
					}
					inputs[row][col] = substrateOutputVal;
				}
			}
			jangle_file << endl;
			cout << "inputs post-update" << endl;
			print_vector_of_vectors(inputs);
			//write them to a file
			cout << "**********************" << endl;

		}
		jangle_file.close();      
			
		cout << "Trial Complete. "<< endl;
		

#ifdef SPIDERROBOT_INTERACTIVE_EVOLUTION
		//OVERRIDING FITNESS FOR INTERACTIVE EVOLUTION
		if(rewardOrg) return 1000*rewardOrg;
		else return 1;
#endif
		
//		if(numDirectionChanges>maxnumDirectionChanges) return 0.001;
		
		
		double fitness;  //JMC SPIDER FILL IN
		cin >> fitness;
		PRINT(fitness);
		exit(2);

		//printGeneticPertubationEffects(individual, fitness+1);

			
		return fitness;
	}
	
	void SpiderRobotExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
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
		
		static bool haveReportedMaxFitness = false;
		if(!haveReportedMaxFitness){
			cout << "maxFitness is: " << maxFitness << endl;
			haveReportedMaxFitness = true;
		}
		
#ifdef SPIDERROBOT_EXPERIMENT_DEBUG
		cout << "Individual Evaluation complete!\n";
		cout << "Fitness bump (does not include 1 free fitness point) was: " << fitness;
		cout << "maxFitness is: " << maxFitness << endl;
#endif
	}
	
	void SpiderRobotExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
	{
		//visualize = false;
		
		
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
		int result = std::system("convert -quality 100 *.ppm run.mpeg");
		result = std::system("rm *.ppm");
		(void) result;
#else
		fitness += processEvaluation(individual,NULL);
#endif
		
		
		maxFitness += 42;
		
		cout << "TOTAL TEST CASES: " << testCases << endl;
		
		cout << "Individual Evaluation complete!\n";
		
		cout << maxFitness << endl;
		
		individual->reward(fitness);
		
		
		//if (fitness >= maxFitness*.95)
		//{
		//		cout << "PROBLEM DOMAIN SOLVED!!!\n";
		//}
	}
	
	Experiment* SpiderRobotExperiment::clone()
	{
		SpiderRobotExperiment* experiment = new SpiderRobotExperiment(*this);
		
		return experiment;
	}
	
	void SpiderRobotExperiment::printSubstrate()
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

	void SpiderRobotExperiment::printActualSubstrate()
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
    	
	double SpiderRobotExperiment::normalize(const int & r_xyVal, const int & r_numNodesXorY) 
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
	
	
	void SpiderRobotExperiment::printCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
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
	
	double SpiderRobotExperiment::zeroOrNormalize(double mWeightToAlter){
		assert(-1.0 <= mWeightToAlter && mWeightToAlter<= 1.0 );  // if this fails then the normalization is not valid.
		return mWeightToAlter * 3.0;
	}
	
	bool SpiderRobotExperiment::converged(int generation) {
		if(generation == convergence_step)
			return true;
		return false;
	}	
	
    void SpiderRobotExperiment::print_vector_of_vectors(const vector<vector<double> > c_vector_of_vectors){
		cout << endl;
		for(int i=0; i< c_vector_of_vectors.size();i++) {//print rows in reverse order (since x=0 is the lowest)                 
			for(int j=0;j<c_vector_of_vectors[i].size();j++){
				cout << c_vector_of_vectors[i][j] << " ";
			}
			cout << endl;
		}
    }
	
}
