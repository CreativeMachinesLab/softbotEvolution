#include <iomanip> 
#include <sys/stat.h>
#include "Experiments/HCUBE_TargetWeightsExperiment.h"

#define TARGETWEIGHTS_EXPERIMENT_DEBUG (0)


namespace HCUBE
{
    using namespace NEAT;

    TargetWeightsExperiment::TargetWeightsExperiment(string _experimentName)
    :   Experiment(_experimentName)
    {
        numNodesX = int(NEAT::Globals::getSingleton()->getParameterValue("numNodesX"));
        numNodesY = int(NEAT::Globals::getSingleton()->getParameterValue("numNodesY"));
        numSheets = 2; //@JMC: not sure if this code will work for three sheets

        numNodesPerSheet = numNodesX*numNodesY;
        
        cout << " experiment name is TargetWeights.\n";
			
	convergence_step = int(NEAT::Globals::getSingleton()->getParameterValue("ConvergenceStep"));
	if(convergence_step < 0)
		convergence_step = INT_MAX; // never switch
			
        
        generateSubstrate();
        generateTargetWeights();
        //generateSpecificPatternPool();

    }

    NEAT::GeneticPopulation* TargetWeightsExperiment::createInitialPopulation(int populationSize)
    {
        NEAT::GeneticPopulation* population = new NEAT::GeneticPopulation();

        vector<GeneticNodeGene> genes;

        genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("X1","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Y1","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("X2","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Y2","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Output","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));

        for (size_t a=0;a<populationSize;a++)
        {
            shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,true,1.0));

            //for (size_t b=0;b<0;b++)  // this was uncommented in jason's original code, but I think since he has b<0 it never fired. setting it to >0 would probably testMutate it
            //{            
            //    individual->testMutate();
            //}

            population->addIndividual(individual);
        }

        return population;
    }

    void TargetWeightsExperiment::generateSubstrate()
    {
        cout << "Generating substrate...";
        boost::object_pool<NEAT::NetworkNode> networkNodePool;
        boost::object_pool<NEAT::NetworkLink> networkLinkPool;

        NEAT::NetworkNode *nodes = NULL;
        NEAT::NetworkLink *links = NULL;

        try
        {
//			nodes = (NEAT::NetworkNode*)malloc(sizeof(NEAT::NetworkNode)*numNodesX*numNodesY*numSheets);
//			links = (NEAT::NetworkLink*)malloc(sizeof(NEAT::NetworkLink)*numNodesX*numNodesY*numNodesX*numNodesY*(numSheets -1));

            nodes = new NEAT::NetworkNode[(numNodesX*numNodesY)+(numNodesX*numNodesY)];
            links = new NEAT::NetworkLink[(numNodesX*numNodesY)*(numNodesX*numNodesY)];
        }
        catch (std::exception e)
        {
            cout << e.what() << endl;
			CREATE_PAUSE(string("Line Number: ")+toString(__LINE__));
        }

        nameLookup.clear();

        cout << "Creating nodes...\n";

        int nodeCounter=0;
        int coordY1=0;
        int coordX1=0;
        int coordY2=0;
        int coordX2=0;


        for (int y1=0;y1<numNodesY;y1++) //set up the input sheet of neurons
        {
            for (int x1=0;x1<numNodesX;x1++)
            {
            
                coordX1 = mapXYvalToGridCoord(x1, numNodesX);  //see function for description
                coordY1 = mapXYvalToGridCoord(y1, numNodesY);
                
                Node node (coordX1, coordY1, 0); //z=0 cause it is the input sheet
                string nodeName = string("s1:") + toString(coordY1)+string("/")+toString(coordX1);

                // print out info on the nodes
                cout << "node #: " << nodeCounter;
                cout << "    node name: " << nodeName << endl;

                // build up the name string (e.g.'s1:0/0')
                nameLookup[node] = nodeName; //s1 = 'sheet one'

                // create a node with that name
                new(&nodes[nodeCounter]) NetworkNode(nameLookup[node]);
                nodeCounter++;

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

                Node node (coordX2, coordY2, 1); //z=0 cause it is the input sheet
                string nodeName = string("s2:") + toString(coordY2)+string("/")+toString(coordX2);

                // print out info on the nodes
                cout << "node #: " << nodeCounter;
                cout << " namelookup (intY,intX): (" << coordY2 << "," << coordX2 << ")";
                
                cout << "    node name: " << nodeName << endl;

                // build up the name string (e.g.'s1:0/0')
                nameLookup[node] = nodeName; //s2 = 'sheet two'

                // create a node with that name
                new(&nodes[nodeCounter]) NetworkNode(nameLookup[node]);
                nodeCounter++;
            }
        }

        cout << "Creating links...\n";

        int linkCounter=0;
        
        for(int fromNode=0;fromNode<numNodesPerSheet;fromNode++)
        {
            for(int toNode=numNodesPerSheet;toNode<numNodesPerSheet + numNodesPerSheet;toNode++)  //the nodes in the second sheet start at numNodesPerSheet, and stop at numNodesPerSheet+numNodesPerSheet
                {
                    try
                    {
                        new (&links[linkCounter]) NetworkLink(
                                                                &nodes[fromNode],
                                                                &nodes[toNode],
                                                                0.0
                                                                );
                        linkCounter++;
                        cout << "from node: " << fromNode << "    to node: " << toNode << endl;
                    }
                    catch (...)
                    {
                        cout << "AN EXCEPTION HAS OCCURED!\n";
                        CREATE_PAUSE(string("Line Number: ")+toString(__LINE__));
                    }                    
                }
        }


        cout << "Creating FastNetwork\n";

        substrate = NEAT::FastNetwork<double>(
                        nodes,
                        (numNodesPerSheet)+(numNodesPerSheet),
                        links,
                        (numNodesPerSheet)*(numNodesPerSheet)
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

        delete[] nodes;
        delete[] links;
    }


    void TargetWeightsExperiment::generateTargetWeights()
    {
        int percentTargetsRepeated = int(NEAT::Globals::getSingleton()->getParameterValue("percentTargetsRepeated"));
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
            cout << "counter: " << counter << endl;
            tempRandIndex = 
                NEAT::Globals::getSingleton()->getRandom().getRandomInt(int(target_weights.size()));
                cout << "tempRandIndex: " << tempRandIndex << endl;
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

    int TargetWeightsExperiment::mapXYvalToGridCoord(const int & r_xyVal, const int & r_numNodesXorY) 
    {
        // turn the xth or yth node into its coordinates on a grid x values (1,2,3,4,5) become (-2, -1 , 0, 1, 2)
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

    double TargetWeightsExperiment::mapXYvalToNormalizedGridCoord(const int & r_xyVal, const int & r_numNodesXorY) 
    {
        // turn the xth or yth node into its coordinates on a grid from -1 to 1, e.g. x values (1,2,3,4,5) become (-1, -.5 , 0, .5, 1)
        // this works with even numbers, and for x or y grids only 1 wide/tall, which was not the case for the original
        // e.g. see findCluster for the orignal versions where it was not a funciton and did not work with odd or 1 tall/wide #s
        double coord;
                
        if(r_numNodesXorY==1) coord = 0;
        else                  coord = -1 + ( r_xyVal * 2.0/(r_numNodesXorY-1) );

        return(coord);    
    }

	NEAT::FastNetwork<double> TargetWeightsExperiment::populateAndReturnSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual) {
		populateSubstrate(individual);
		return substrate;
	}
	
    void TargetWeightsExperiment::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual) 
    {
        //cout << "Populating substrate...";
        NEAT::FastNetwork <double> network = individual->spawnFastPhenotypeStack<double>();        //JMC: this network is the CPPN used to produce the substrate (neural network)
        //progress_timer t;
        
        int linkCount=0;

        int counter=0;
        double x1Val,y1Val,x2Val,y2Val;
        
        for (int y1=0;y1<numNodesY;y1++)
        {
            //cout << "loop: " << endl;
            for (int x1=0;x1<numNodesX;x1++)
            {
                for (int y2=0;y2<numNodesY;y2++)
                {
                    for (int x2=0;x2<numNodesX;x2++)
                    {
                    
                        x1Val = mapXYvalToNormalizedGridCoord(x1, numNodesX);  //see function for description
                        y1Val = mapXYvalToNormalizedGridCoord(y1, numNodesY);
                        x2Val = mapXYvalToNormalizedGridCoord(x2, numNodesX);  //see function for description
                        y2Val = mapXYvalToNormalizedGridCoord(y2, numNodesY);
                        
                        //cout << "x1: " << setw(5)  << setprecision(2) << x1Val << "  y1: " << setw(5) << y1Val << "  x2: " << setw(5) << setprecision(2) << x2Val << "  y2: " << setw(5) << y2Val << endl;
                        network.reinitialize();
                        network.setValue("X1",x1Val);
                        network.setValue("Y1",y1Val);
                        network.setValue("X2",x2Val);
                        network.setValue("Y2",y2Val);

                        network.setValue("Bias",0.3);                       //JMC: we have just set the inputs to the     to be x1,y1,x2,y2,bias 
                        
                        
                        //cout << "RUNNING UPDATE ON CPPN----------------------------------------------------------------------------------------" << endl;
                        network.update();                                   //JMC: on this line we run the CPPN network...  
                        //cout << "DONE RUNNING UPDATE ON CPPN----------------------------------------------------------------------------------------" << endl;
                                                                              

                        double greyVal = network.getValue("Output");        //JMC: and here we get the CPPN output (which is the weight of the connection between the two)

                        //substrate.getLink(counter)->weight = ( greyVal*3.0 );

                        //for the target weight experiment we do not want to make it easier for them to get zero nor do we need to normalize the greyValue, so just use it
                        
                        substrate.getLink(counter)->weight = greyVal;
                        linkCount++;

                        counter++;
                    }
                }
            }
        }

        //cout << "done!\n";

        //cout << "Number of expressed links: " << linkCount << endl;
    }

    double TargetWeightsExperiment::processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual)
    {
        #if TARGETWEIGHTS_EXPERIMENT_DEBUG
        cout << "JMC: evaluating fitness in TargetWeightsExperiment::processEvaluation\n";
        #endif

        #if TARGETWEIGHTS_EXPERIMENT_DEBUG
        cout << "Initializing substrate\n";
        #endif

        substrate.reinitialize();
        substrate.dummyActivation();
        
        int numLinks = substrate.getLinkCount();
        double distanceFromTarget;
        double sumDistances = 0;

        if(numLinks!=target_weights.size()) {cout << "error: size of target_weights vector does not equal numLinks"; exit(30);}
        
        for (size_t a=0;a<numLinks;a++)
        {
            NetworkIndexedLink <double> link = *substrate.getLink(a);

            distanceFromTarget = abs(link.weight - target_weights[a]);
            sumDistances += distanceFromTarget;
                                     
            #if TARGETWEIGHTS_EXPERIMENT_DEBUG
            cout << link.fromNode << "->" << link.toNode << " linkWeight: " << setw(10) << link.weight
                 << " target: " << setw(10) << target_weights[a] <<  " diff: " << distanceFromTarget << endl;
            #endif

        }
        #if TARGETWEIGHTS_EXPERIMENT_DEBUG
        cout << "average dist from target: " << setprecision(22) << sumDistances/numLinks << endl;
        #endif
        
        return sumDistances;  // this is what will be used to boost fitness
    }

    void TargetWeightsExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
    {
		shared_ptr<NEAT::GeneticIndividual> individual = group.front();

        #if TARGETWEIGHTS_EXPERIMENT_DEBUG
          cout << "in TargetWeightsExperiment.cpp::processIndividual, processing individual:  " << individual << endl;
        #endif
        double fitness =0;
        int rangeOfSubstrateLinkValues = 6; // e.g. [-1,1] range = 2, [-3,3] = 6; 
        double maxFitness = numNodesPerSheet*numNodesPerSheet*rangeOfSubstrateLinkValues; //for every link the worst you can do is be rangeOfSubstrateLinkValues away, so we subtract dist from that

        populateSubstrate(individual);
        
        double sumOfDistances = processEvaluation(individual);
        

        fitness = maxFitness-sumOfDistances;
        
        if(maxFitness > std::numeric_limits<double>::max())
        {
            cout << "error: the max fitness is greater than the size of the double. " << endl;
            cout << "max double size is: : " << std::numeric_limits<double>::max() << endl;
            cout << "max fitness is: " << maxFitness << endl;
            exit(88);
        } 

        #if TARGETWEIGHTS_EXPERIMENT_DEBUG        
            cout << "Individual Evaluation complete!\n";
            cout << "maxFitness: " << maxFitness << endl;
            cout << "fitness: " << fitness << endl;
        #endif
  
        individual->reward(pow(2,double(fitness)));

        if (fitness >= maxFitness)
        {
            #if TARGETWEIGHTS_EXPERIMENT_DEBUG        
            cout << "PROBLEM DOMAIN SOLVED!!!\n";
            #endif
            ifstream inp;
            string myFileName = "substrateThatSolvedTheProblem.txt";
            inp.open(myFileName.c_str(), ifstream::in);
            inp.close();
            if(inp.fail())          //file does not yet exist, which means this is the first org to fully solve the problem
            {
              inp.clear(ios::failbit);
              cout << "JUST PRINTING THIS ONCE ++++++++++++++++++++++++++++++++++++++++++++++" << endl;
              printSubstrate(individual);        
              printNetworkCPPN(individual);    

            }
            //print network and substrate
        }
        if (fitness < 0)
        {
            cout << "Fitness Less Than Zero!!!, it is: " << fitness << "\n";    //note: the org originally got 10 points just for showing up, so unless this is >-9 it should be fine
            exit(10);
        }
    }

    void TargetWeightsExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
    {
        /*{
            mutex::scoped_lock scoped_lock(*Globals::ioMutex);
            cout << "Starting Evaluation on object:" << this << endl;
            cout << "Running on individual " << individual << endl;
        }*/

        cout << "Sorry, this was never coded up for ShapeRecognitioV1. You'll have to do that now." << endl;
        exit(6);
        
        //in jason's code, the got 10 points just for entering the game, wahooo!
        individual->setFitness(0);

        populateSubstrate(individual);

        double fitness=0;

        double maxFitness = 0;

        //bool solved=true; @jmc: was uncommented in the original and produced an error forn not being used

        for (int y1=0;y1<numNodesY;y1+=sizeMultiplier)
        {
            //cout << '*';
            for (int x1=0;x1<numNodesX;x1+=sizeMultiplier)
            {
                //cout << '_';
                for (int y1Big=0;y1Big<numNodesY;y1Big+=sizeMultiplier)
                {
                    //cout << '.';
                    for (int x1Big=0;x1Big<numNodesX;x1Big+=sizeMultiplier)
                    {
                        int smallRadius;
                        int bigRadius;

                        if (sizeMultiplier==1)
                        {
                            smallRadius=0;
                            bigRadius=1;
                        }
                        else if (sizeMultiplier==3)
                        {
                            smallRadius=1;
                            bigRadius=4;
                        }
                        else if (sizeMultiplier==5)
                        {
                            smallRadius=2;
                            bigRadius=7;
                        }
                        else if (sizeMultiplier==9)
                        {
                            smallRadius=4;
                            bigRadius=13;
                        }
                        else
                        {
                            throw string("Unsupported size multiplier!");
                        }

                        int dist = smallRadius+1+bigRadius+1;

                        if ( ((x1-x1Big)*(x1-x1Big) + (y1-y1Big)*(y1-y1Big)) < (dist*dist))
                            continue;

                        if (y1Big-bigRadius<0)
                            continue;
                        else if (y1Big+bigRadius>=numNodesY)
                            continue;

                        if (x1Big-bigRadius<0)
                            continue;
                        else if (x1Big+bigRadius>=numNodesX)
                            continue;

                        if (y1-smallRadius<0)
                            continue;
                        else if (y1+smallRadius>=numNodesY)
                            continue;

                        if (x1-smallRadius<0)
                            continue;
                        else if (x1+smallRadius>=numNodesX)
                            continue;

                        fitness += processEvaluation(individual);

                        maxFitness += 30;

                    }
                }
            }
        }

        //cout << "Individual Evaluation complete!\n";

        //cout << maxFitness << endl;

        individual->reward(fitness);

        if (fitness >= maxFitness*.95)
        {
            cout << "PROBLEM DOMAIN SOLVED!!!\n";
        }
    }

    void TargetWeightsExperiment::printSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
      cout << "Printing substrate" << endl;
      ofstream substrate_file;        
      substrate_file.open ("substrateThatSolvedTheProblem.txt", ios::trunc );
      
      //cout << "num links: " <<substrate.getLinkCount() << endl;
      //substrate_file << "num links: " <<substrate.getLinkCount() << endl;
      
      int counter=0;
      for (int y1val=0;y1val<numNodesY;y1val++)
      {
          for (int x1val=0;x1val<numNodesX;x1val++)
          {
              for (int y2val=0;y2val<numNodesY;y2val++)
              {
                  for (int x2val=0;x2val<numNodesX;x2val++)
                  {
                      if(counter > substrate.getLinkCount()){ cout << "error: you are trying to print a link that does not exist" << endl; exit(1);}
                      //substrate_file << "fromNode: " << substrate.getLink(counter)->fromNode << " toNode: " << substrate.getLink(counter)->toNode;
//                      substrate_file << " from (x1val,y1val) to (x2val,y2val): (" << x1val << "," << y1val << ") to (" << x2val << "," << y2val << "): " ;
                      substrate_file << substrate.getLink(counter)->weight << endl;
                      
                      counter++;
                  }
              }
          }
      }
      substrate_file.close();

      return;
    }
    
    void TargetWeightsExperiment::printNetworkCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
      cout << "Printing cppn network" << endl;
      ofstream network_file;        
      network_file.open ("networkCPPN-ThatSolvedTheProblem.txt", ios::trunc );
      
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

    Experiment* TargetWeightsExperiment::clone()
    {
        TargetWeightsExperiment* experiment = new TargetWeightsExperiment(*this);

        return experiment;
    }
    


    void TargetWeightsExperiment::print_vector_of_vectors(const vector<vector<int> > c_vector_of_vectors){
      cout << endl;
      for(int i=0; i< c_vector_of_vectors.size();i++) {//print rows in reverse order (since x=0 is the lowest)                 
        for(int j=0;j<c_vector_of_vectors[i].size();j++){
          cout << c_vector_of_vectors[i][j] << " ";
        }
        cout << endl;
      }
    }

    void TargetWeightsExperiment::print_vector_of_vectors(const vector<vector<double> > c_vector_of_vectors){
      cout << endl;
      for(int i=0; i< c_vector_of_vectors.size();i++) {//print rows in reverse order (since x=0 is the lowest)                 
        for(int j=0;j<c_vector_of_vectors[i].size();j++){
          cout << c_vector_of_vectors[i][j] << " ";
        }
        cout << endl;
      }
    }

	bool TargetWeightsExperiment::converged(int generation) {
		if(generation == convergence_step)
			return true;
		return false;
	}		
}


