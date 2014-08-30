#include "Experiments/HCUBE_TargetWeightsNoGeomExperiment.h"
#include <iomanip> 


#define TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG (0)

namespace HCUBE
{
    using namespace NEAT;

    NEAT::GeneticPopulation* TargetWeightsNoGeomExperiment::createInitialPopulation(int populationSize)
    {
        NEAT::GeneticPopulation* population = new NEAT::GeneticPopulation();

        vector<GeneticNodeGene> genes;
        vector<GeneticLinkGene> links;

        genes.reserve(10000);
        links.reserve(10000);

        int coordY1=0;
        int coordX1=0;
        int coordY2=0;
        int coordX2=0;

        cout << "Creating Nodes\n";

        for (int y1=0;y1<numNodesY;y1++)
        {
            for (int x1=0;x1<numNodesX;x1++)
            {
                coordX1 = mapXYvalToGridCoord(x1, numNodesX);  //see function for description
                coordY1 = mapXYvalToGridCoord(y1, numNodesY);
                
                #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
                cout << "adding gene " << string("s1:") + toString(coordY1)+string("/")+toString(coordX1) << endl;
                #endif
                genes.push_back(
                    GeneticNodeGene(
                        string("s1:") + toString(coordY1)+string("/")+toString(coordX1),
                        "NetworkInputNode",
                        0,
                        false
                    )
                );
            }
        }

        for (int y2=0;y2<numNodesY;y2++)
        {
            for (int x2=0;x2<numNodesX;x2++)
            {
                coordX2 = mapXYvalToGridCoord(x2, numNodesX)+100;  //see function for description
                coordY2 = mapXYvalToGridCoord(y2, numNodesY)+100;
                
                #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
                cout << "adding gene " << string("s2:") + toString(coordY2)+string("/")+toString(coordX2) << endl;
                #endif
                genes.push_back(
                    GeneticNodeGene(
                        string("s2:") + toString(coordY2)+string("/")+toString(coordX2),
                        "NetworkOutputNode",
                        0,
                        false
                    )
                );
            }
        }
        cout << "Creating links\n";

        int createdLinksCounter = 0;

        for(int fromNode=0;fromNode<numNodesPerSheet;fromNode++)
        {
            //the nodes in the second sheet start at numNodesPerSheet, and stop at numNodesPerSheet+numNodesPerSheet
            for(int toNode=numNodesPerSheet;toNode<numNodesPerSheet + numNodesPerSheet;toNode++) 
                {
                    try
                    {
//                        #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
//                        cout << "from node: " << fromNode << "    to node: " << toNode;
//                        cout << " creating link " << createdLinksCounter << "   ";
//                        #endif
//                        cout << "Creating gene link from "
//                        << genes[fromNode].getName() << " to "
//                        << genes[toNode].getName() << endl;
                        links.push_back(
                            GeneticLinkGene(
                                genes[fromNode].getID(),
                                genes[toNode].getID()
                            )
                        );
                        createdLinksCounter++;

                        #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
                        cout << endl;
                        #endif
                    }
                    catch (...)
                    {
                        cout << "AN EXCEPTION HAS OCCURED!\n";
                        CREATE_PAUSE(string("Line Number: ")+toString(__LINE__));
                    }                    
                }
        }

        
        
        for (size_t a=0;a<populationSize;a++)
        {
            //cout << "Creating Individual\n";
            shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,links));

            //for (size_t b=0;b<0;b++)  // this was uncommented in jason's original code, but I think since he has b<0 it never fired. setting it to >0 would probably testMutate it
            //{            
            //    individual->testMutate();
            //}

            population->addIndividual(individual);
        }

        return population;
    }

	NEAT::GeneticPopulation* TargetWeightsNoGeomExperiment::createInitialPopulation(shared_ptr<NEAT::GeneticPopulation> CPPN_pop, shared_ptr<Experiment> HyperNEAT_experiment) {
		// TODO: copy target weights
		target_weights = HyperNEAT_experiment->getTargetWeights();
		
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
			
			for (int y1=0;y1<numNodesY;y1++) {
				for (int x1=0;x1<numNodesX;x1++) {
					for (int y2=0;y2<numNodesY;y2++) {
						for (int x2=0;x2<numNodesX;x2++) {
							int coordX1 = mapXYvalToGridCoord(x1, numNodesX);  //see function for description
							int coordY1 = mapXYvalToGridCoord(y1, numNodesY);
							int coordX2 = mapXYvalToGridCoord(x2, numNodesX)+100;  
							int coordY2 = mapXYvalToGridCoord(y2, numNodesY)+100;
							
							string sourceString = nameLookup[pair<double,double>(coordY1,coordX1)];
							string targetString   = nameLookup[pair<double,double>(coordY2,coordX2)];
							
							GeneticNodeGene* sourceNode = PNEAT_individaul->getNode(sourceString);
							GeneticNodeGene* targetNode = PNEAT_individaul->getNode(targetString);
							
							GeneticLinkGene* PNEAT_link;
							PNEAT_link = PNEAT_individaul->getLink(sourceNode->getID(), targetNode->getID());
							PNEAT_link->setWeight(HyperNEAT_substrate.getLink(sourceString, targetString)->weight);							
						}
					}
				}
			}			
		}
		return PNEAT_population;
	}
	
	
	void TargetWeightsNoGeomExperiment::generateSubstrate()
    {
        cout << "Generating substrate...\n";
        boost::object_pool<NEAT::NetworkNode> networkNodePool;
        boost::object_pool<NEAT::NetworkLink> networkLinkPool;
        vector<NEAT::NetworkNode *> nodes;
        vector<NEAT::NetworkLink *> links;
        vector<double> nodeBiases;

        nameLookup.clear();
        
        int counter=0;
        int coordY1=0;
        int coordX1=0;
        int coordY2=0;
        int coordX2=0;

        for (int y1=0;y1<numNodesY;y1++)
        {
            for (int x1=0;x1<numNodesX;x1++)
            {
                coordX1 = mapXYvalToGridCoord(x1, numNodesX);  //see function for description
                coordY1 = mapXYvalToGridCoord(y1, numNodesY);
                
                // print out info on the nodes
                cout << "node #: " << counter;
                cout << "    node name: " << string("s1:") + toString(coordY1)+string("/")+toString(coordX1) << endl;

                // build up the name string (e.g.'s1:0/0')
                nameLookup[pair<double,double>(coordY1,coordX1)] = string("s1:") + toString(coordY1)+string("/")+toString(coordX1); //s1 = 'sheet one'

                // create a node with that name
                nodes.push_back(networkNodePool.construct(nameLookup[pair<double,double>(coordY1,coordX1)]));
                nodeBiases.push_back(0.0);
                counter++;

            }
        }
        // set up the second sheet of neurons (the +100 is so that the nameLookUp index value is different for each sheet
        // e.g. sheet 1, neuron -2/-2 is named "s1:-2/-2" in nameLookUp, but is indexed by nameLookUp[-2/-2], but
        //      sheet 2, neruon -2/-2 is named "s2:-2/-2" in nameLookUp, but is indexed by nameLookUp[98/102] 
        for (int y2=0;y2<numNodesY;y2++) //set up the input sheet of neurons
        {
            for (int x2=0;x2<numNodesX;x2++)
            {
            
                coordX2 = mapXYvalToGridCoord(x2, numNodesX) + 100;  //see function for description
                coordY2 = mapXYvalToGridCoord(y2, numNodesY) + 100;

                // print out info on the nodes
                cout << "node #: " << counter;
                cout << " namelookup (intY,intX): (" << coordY2 << "," << coordX2 << ")";
                cout << "    node name: " << string("s2:") + toString(coordY2)+string("/")+toString(coordX2) << endl;

                // build up the name string (e.g.'s1:0/0')
                nameLookup[pair<double,double>(coordY2,coordX2)] = string("s2:") + toString(coordY2)+string("/")+toString(coordX2); //s1 = 'sheet one'

                // create a node with that name
                nodes.push_back(networkNodePool.construct(nameLookup[pair<double,double>(coordY2,coordX2)]));
                counter++;
            }
        }

        cout << "Creating links...\n";

        int createdLinksCounter = 0;
        
        for(int fromNode=0;fromNode<numNodesPerSheet;fromNode++) 
        {
            for(int toNode=numNodesPerSheet;toNode<numNodesPerSheet + numNodesPerSheet;toNode++)  //the nodes in the second sheet start at numNodesPerSheet, and stop at numNodesPerSheet+numNodesPerSheet
                {
                    try
                    {
                        #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
                        cout << "from node: " << fromNode << "    to node: " << toNode;
                        cout << " creating link " << createdLinksCounter;
                        #endif
                        links.push_back(networkLinkPool.construct(nodes[fromNode],nodes[toNode],0.0));
                        //links[createdLinksCounter] =  NetworkLink(&nodes[fromNode],&nodes[toNode],0.0);
                        createdLinksCounter++;
                        #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
                        cout << endl;
                        #endif
                    }
                    catch (...)
                    {
                        cout << "AN EXCEPTION HAS OCCURED!\n";
                        CREATE_PAUSE(string("Line Number: ")+toString(__LINE__));
                    }                    
                }
        }
        cout << "Creating FastNetwork\n";

        substrate = NEAT::FastNetwork<double>(nodes,links);

        // 
        // jmc I don't know what these values mean and why they are hard coded. they don't show up in the Geom experiments
        // I don't think zeroing out one link will make a big difference, since those weights will be controlled by evolution anyway, so I am commenting it out
        //                        substrate.getLink(                          
        //                            nameLookup[pair<double,double>(-5,-5)],
        //                            nameLookup[pair<double,double>(-5,-4)]
        //                        )->weight = 0.0;
        cout << "done!\n";
    }

    void TargetWeightsNoGeomExperiment::generateTargetWeights()
    {
        int percentTargetsRepeated = (int) NEAT::Globals::getSingleton()->getParameterValue("percentTargetsRepeated");
        cout << "percentTargetsRepeated: " << percentTargetsRepeated << endl;
                        
        for (int z=0;z<numNodesPerSheet*numNodesPerSheet;z++)
        {
            target_weights.push_back(NEAT::Globals::getSingleton()->getRandom().getRandomDouble(-1,1));
        }

        int num_repeated_weights = target_weights.size() * percentTargetsRepeated/100;
        double repeated_weight = NEAT::Globals::getSingleton()->getRandom().getRandomDouble(-1,1);

        cout << "num_repeated_weights is: " << num_repeated_weights << endl;
        cout << "target_weights.size() is: " << target_weights.size() << endl;
        cout << "repeated_weight is: " << repeated_weight << endl; 
        

        int counter =0;
        int tempRandIndex = 0;
        vector <int> indicesToSetToRepeatedWeight;
        while(counter<num_repeated_weights)
        {
            tempRandIndex = 
                NEAT::Globals::getSingleton()->getRandom().getRandomInt(int(target_weights.size()));
            if( find (indicesToSetToRepeatedWeight.begin(), indicesToSetToRepeatedWeight.end(), tempRandIndex)
                == indicesToSetToRepeatedWeight.end())
            {
                indicesToSetToRepeatedWeight.push_back(tempRandIndex);
                counter++;
            }
            
        }
        
        #if TARGETWEIGHTS_EXPERIMENT_DEBUG        
        cout << "the " << indicesToSetToRepeatedWeight.size() << " indices to be set to random weights are" << endl;
        #endif
        
        for(int z=0;z<indicesToSetToRepeatedWeight.size();z++)
        {
            #if TARGETWEIGHTS_EXPERIMENT_DEBUG        
            cout << indicesToSetToRepeatedWeight[z] << endl;
            #endif
            target_weights[indicesToSetToRepeatedWeight[z]] = repeated_weight;
        }
                        
        #if TARGETWEIGHTS_EXPERIMENT_DEBUG
        cout << "the weights are: " << endl;
        for(int q=0;q<target_weights.size();q++)
        {
            cout << "target weight [ " << q << "] " << target_weights[q] << endl;                      
        }              
        #endif
        return;
    }


    int TargetWeightsNoGeomExperiment::mapXYvalToGridCoord(const int & r_xyVal, const int & r_numNodesXorY) 
    {
        // turn the xth or yth node into its coordinates on a grid from -1 to 1, e.g. x values (1,2,3,4,5) become (-1, -.5 , 0, .5, 1)
        // this works with even numbers, and for x or y grids only 1 wide/tall, which was not the case for the original
        // e.g. see findCluster for the orignal versions where it was not a funciton and did not work with odd or 1 tall/wide #s
        int coord;
                
        if(r_numNodesXorY==1) coord = 0;
        else                  coord = r_xyVal - r_numNodesXorY/2 ;
        
        if(r_numNodesXorY%2==0) //take care of even numbered grids
        {
            if(coord>=0) coord++;  //which require us to skip over 0
        } 

        return(coord);    
    }


    void TargetWeightsNoGeomExperiment::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
        #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
        cout << "Populating substrate..." << endl;
        #endif

        NEAT::FastNetwork <double> network = individual->spawnFastPhenotypeStack<double>();
        double cppnLinkWeight = 0.0;
        double substrateLinkWeight = 0.0;

        int coordY1;
        int coordX1;
        int coordY2;
        int coordX2;


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
                        coordX1 = mapXYvalToGridCoord(x1, numNodesX);  //see function for description
                        coordY1 = mapXYvalToGridCoord(y1, numNodesY);
                        coordX2 = mapXYvalToGridCoord(x2, numNodesX)+100;  
                        coordY2 = mapXYvalToGridCoord(y2, numNodesY)+100;
                        
                        string fromNodeName = nameLookup[pair<double,double>(coordY1,coordX1)];
                        string toNodeName   = nameLookup[pair<double,double>(coordY2,coordX2)];
                        
                        cppnLinkWeight = 
                            network.getLink(                                                 //network = cppn 
                                fromNodeName,                                                //used to be coded as  nameLookup[pair<double,double>(y1-numNodesY/2,x1-numNodesX/2)],
                                toNodeName                                                   //used to be coded as  nameLookup[pair<double,double>(y2-numNodesY/2,x2-numNodesX/2)]
                            )->weight;
                        substrateLinkWeight = cppnLinkWeight;   //JMC: ..then normalize it (0 to 3) and set it as the weight in the substrate (NN)
                    
                        #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
                        cout << "coordY1: " << coordY1 << "  coordX1: " << coordX1 << "  coordY2: " << coordY2 << "  coordX2: " << coordX2;
                        cout << " setting link from " << fromNodeName << " to " << toNodeName << " to the value: " << substrateLinkWeight << endl;
                        #endif

                        //set this new value to be the weight 
                        substrate.getLink(                                                   //substrate = final network we evaluate fitness on   
                            fromNodeName,
                            toNodeName
                        )->weight = substrateLinkWeight;
                    }
                }
            }
        }
    }
    double TargetWeightsNoGeomExperiment::processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual)
    {
        #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
        cout << "JMC: evaluating fitness in TargetWeightsNoGeomExperiment::processEvaluation\n";
        #endif
        
        #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
        cout << "Initializing substrate\n";
        #endif

        substrate.reinitialize();
        substrate.dummyActivation();
        
        int numLinks = substrate.getLinkCount();
        static int numLinksFirstTimeThrough = numLinks;
        assert(numLinksFirstTimeThrough == numLinks);  //numLinks should never change in FT-NEAT

        double distanceFromTarget;
        double sumDistances = 0;

        if(numLinks!=target_weights.size()) {cout << "error: size of target_weights vector does not equal numLinks"; exit(30);}
                
        for (size_t a=0;a<numLinks;a++)
        {
            NetworkIndexedLink <double> link = *substrate.getLink(a);

            distanceFromTarget = abs(link.weight - target_weights[a]);
            sumDistances += distanceFromTarget;
                                     
            #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
            cout << link.fromNode << "->" << link.toNode << " linkWeight: " << setw(10) << link.weight
                 << " target: " << setw(10) << target_weights[a] <<  " abs(diff): " << distanceFromTarget << endl;
            #endif

        }
        #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
        cout << "average dist from target: " << setprecision(22) << sumDistances/numLinks << endl;
        #endif
        
        return sumDistances;  // this is what will be used to boost fitness
    }

    void TargetWeightsNoGeomExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
    {
		shared_ptr<NEAT::GeneticIndividual> individual = group.front();

        static bool needToCheckParams = true;
        if(needToCheckParams)
        {
            //These values should be true for FT-NEAT 
            assert(NEAT::Globals::getSingleton()->getParameterValue("MutateAddNodeProbability") == 0.0);
            assert(NEAT::Globals::getSingleton()->getParameterValue("MutateAddLinkProbability") == 0.0);
            assert(NEAT::Globals::getSingleton()->getParameterValue("MutateDemolishLinkProbability") == 0.0);
            needToCheckParams=false;
        }


        #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG
          cout << "in TargetWeightsNoGeomExperiment.cpp::processIndividual, processing individual:  " << individual << endl;
        #endif
        double fitness =0;
        int rangeOfSubstrateLinkValues = 6; // e.g. [-1,1] range = 2, [-3,3] = 6; 
        double maxFitness = numNodesPerSheet*numNodesPerSheet*rangeOfSubstrateLinkValues; //for every link the worst you can do is be rangeOfSubstrateLinkValues away, so we subtract dist from that

        populateSubstrate(individual);
        
        individual->setFitness(0);

        double sumOfDistances = processEvaluation(individual);
        
        fitness = maxFitness-sumOfDistances;
        
        if(maxFitness > std::numeric_limits<double>::max())
        {
            cout << "error: the max fitness is greater than the size of the double. " << endl;
            cout << "max double size is: : " << std::numeric_limits<double>::max() << endl;
            cout << "max fitness is: " << maxFitness << endl;
            exit(88);
        } 

        #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG        
            cout << "Individual Evaluation complete!\n";
            cout << "maxFitness: " << maxFitness << endl;
            cout << "fitness: " << fitness << endl;
        #endif
  
        individual->reward(pow(2,double(fitness)));

        if (fitness >= maxFitness)
        {
//            cout << "PRINTING EVERY PERFECT SOLUTION SUBSTRATE" << endl;
//            printSubstrate(individual);        
//            printNetworkCPPN(individual);    
            #if TARGET_WEIGHTS_NOGEOM_EXPERIMENT_DEBUG        
            cout << "PROBLEM DOMAIN SOLVED!!!\n";

            #endif
            ifstream inp;
            string myFileName = "substrateThatSolvedTheProblem.txt";
            inp.open(myFileName.c_str(), ifstream::in);
            inp.close();

            if(inp.fail())          //file does not yet exist, which means this is the first org to fully solve the problem
            {
              inp.clear(ios::failbit);
              cout << "PERFECT SOLUTION FOUND: JUST PRINTING SUBSTRATE AND CPPN ONCE ++++++++++++++++++++++++++++++++++++++++++++++" << endl;
              //print network and substrate
              printSubstrate(individual);        
              printNetworkCPPN(individual);    

            }
        }
        
        if (fitness < 0)
        {
            cout << "Fitness Less Than Zero!!!, it is: " << fitness << "\n";  
            exit(10);
        }
        
    }

    void TargetWeightsNoGeomExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
    {
        /*{
            mutex::scoped_lock scoped_lock(*Globals::ioMutex);
            cout << "Starting Evaluation on object:" << this << endl;
            cout << "Running on individual " << individual << endl;
        }*/

        cout << "Sorry, this was never coded up for Post Hoc. You'll have to do that now. That is, if you need to. I don't think this code ever gets called. Even doing a post-hoc analysis seems to call processIndividual" << endl;
        exit(6);

        //You get 10 points just for entering the game, wahooo!
        individual->setFitness(10);
        
        populateSubstrate(individual);

        double fitness=0;

        double maxFitness = 0;


        //cout << "Individual Evaluation complete!\n";

        //cout << maxFitness << endl;

        individual->reward(fitness);

        if (fitness >= maxFitness*.95)
        {
            cout << "PROBLEM DOMAIN SOLVED!!!\n";
        }
    }

    void TargetWeightsNoGeomExperiment::printSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
      cout << "Printing substrate" << endl;
      ofstream substrate_file;        
      substrate_file.open ("substrateThatSolvedTheProblem.txt", ios::trunc );
      
      //cout << "num links: " <<substrate.getLinkCount() << endl;
      substrate_file << "#num links: " <<substrate.getLinkCount() << endl;
      
      int counter=0;

      //version with # signs in front (comments)
      for (int z=0;z<substrate.getLinkCount();z++)
      {
          if(counter > substrate.getLinkCount()){ cout << "error: you are trying to print a link that does not exist" << endl; exit(1);}
          substrate_file << "#fromNode: " << setw(3) << substrate.getLink(counter)->fromNode << " toNode: " << setw(3) << substrate.getLink(counter)->toNode << "   link value: ";
          substrate_file << substrate.getLink(counter)->weight << endl;
          
          counter++;
      }

      counter = 0;
      //version without # signs in front
      for (int z=0;z<substrate.getLinkCount();z++)
      {
          if(counter > substrate.getLinkCount()){ cout << "error: you are trying to print a link that does not exist" << endl; exit(1);}
          substrate_file << substrate.getLink(counter)->weight << endl;
          
          counter++;
      }
      substrate_file.close();

      return;
    }
    
    void TargetWeightsNoGeomExperiment::printNetworkCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
      cout << "Printing cppn network" << endl;
      ofstream network_file;        
      network_file.open ("networkCPPN-ThatSolvedTheProblem.txt", ios::trunc );
      
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

    Experiment* TargetWeightsNoGeomExperiment::clone()
    {
        cout << "Experiment cloned\n";
        TargetWeightsNoGeomExperiment* experiment = new TargetWeightsNoGeomExperiment(*this);

        cout << this << " cloned to " << experiment << endl;

        return experiment;
    }


    void TargetWeightsNoGeomExperiment::print_vector_of_vectors(const vector<vector<int> > c_vector_of_vectors){
      cout << endl;
      for(int i=0; i< c_vector_of_vectors.size();i++) {//print rows in reverse order (since x=0 is the lowest)                 
        for(int j=0;j<c_vector_of_vectors[i].size();j++){
          cout << c_vector_of_vectors[i][j] << " ";
        }
        cout << endl;
      }
    }

    void TargetWeightsNoGeomExperiment::print_vector_of_vectors(const vector<vector<double> > c_vector_of_vectors){
      cout << endl;
      for(int i=0; i< c_vector_of_vectors.size();i++) {//print rows in reverse order (since x=0 is the lowest)                 
        for(int j=0;j<c_vector_of_vectors[i].size();j++){
          cout << c_vector_of_vectors[i][j] << " ";
        }
        cout << endl;
      }
    }

}
