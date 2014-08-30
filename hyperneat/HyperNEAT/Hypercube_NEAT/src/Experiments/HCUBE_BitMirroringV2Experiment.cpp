//V2 allows randomized, % randomized within row (but not column), and % direct across

#include "Experiments/HCUBE_BitMirroringV2Experiment.h"

namespace HCUBE
{
    using namespace NEAT;
	
	double dequeAverage(deque<double> d)
	{
		double av = 0;
		deque<double>::iterator dit;
		
		for(dit = d.begin(); dit != d.end(); ++dit)
		{
			av+= (*dit);
		}
		return av/d.size();
	}
	
#ifdef BITMIRROR_HIDDEN_LAYER
	int BitMirroringV2Experiment::getHiddenNodeCount()
	{
		int nodecount = 0;
		for (int y1=0; y1<numNodesY; y1++)
		{
			for (int x1=0; x1<numNodesX; x1++)
			{
				if (HiddenNodeLookup[pair<int,int>(y1,x1)] == true) {
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
					cout<<"HiddenNodeLookup["<<y1<<","<<x1<<"] = TRUE"<<endl;
#endif
					nodecount++;
				}
				else {
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
					cout<<"HiddenNodeLookup["<<y1<<","<<x1<<"] = FALSE"<<endl;
#endif
				}
			}
		} 
		return nodecount;
	}
	
	void BitMirroringV2Experiment::printHiddenNodeLookup()
	{

		for (int y1=0; y1<numNodesY; y1++)
		{
			for (int x1=0; x1<numNodesX; x1++)
			{
				if (HiddenNodeLookup[pair<int,int>(y1,x1)] == true) {
					cout<<"HiddenNodeLookup["<<y1<<","<<x1<<"] = TRUE"<<endl;
				}
				else {
					cout<<"HiddenNodeLookup["<<y1<<","<<x1<<"] = FALSE"<<endl;
				}
			}
		} 

	}

#endif

    BitMirroringV2Experiment::BitMirroringV2Experiment(string _experimentName)
            :
            Experiment(_experimentName),
            numNodesX((int)NEAT::Globals::getSingleton()->getParameterValue("NumNodesX-Y")),  //defautl was 11 in 07 gecco papers
            numNodesY((int)NEAT::Globals::getSingleton()->getParameterValue("NumNodesX-Y"))
    {
		
//get values from configuration
		convergence_step = int(NEAT::Globals::getSingleton()->getParameterValue("ConvergenceStep"));

#ifdef BITMIRROR_HIDDEN_LAYER
		numSheets = 3;
#else
		numSheets = 2;
#endif

		
#ifdef MANUAL_NORMALIZATION
		normalization_cutoff = double(NEAT::Globals::getSingleton()->getParameterValue("NormalizatonCutoff"));
#endif //MANUAL_NORMALIZATION
		
//if we specify a fitness to end the run at, set it
		if (NEAT::Globals::getSingleton()->hasParameterValue("TerminalFitness"))
		{
			terminateOnTargetFitness = true;
			terminalAverageN = 10;

			terminalFitness = int(NEAT::Globals::getSingleton()->getParameterValue("TerminalFitness"));

			if (NEAT::Globals::getSingleton()->hasParameterValue("TerminalAverageN"))
				terminalAverageN = int(NEAT::Globals::getSingleton()->getParameterValue("TerminalAverageN"));
			
			LastNFitnesses.assign(terminalAverageN,0);
		}
		else {
			terminateOnTargetFitness = false;
			terminalFitness = -1;
			terminalAverageN = -1;
		}

		
#ifdef BITMIRROR_HIDDEN_LAYER
		hiddenNodesUsed = -1;
		hiddenAlgorithm = HiddenLayerAlgorithm(NEAT::Globals::getSingleton()->getParameterValue("HiddenNodeChoiceAlgorithm"));
		
		switch (hiddenAlgorithm) {
			case RANDOM:
				cout<<"---Bitmirroring Algorithm: RANDOM---"<<endl;
				maxHiddenNodes = int(NEAT::Globals::getSingleton()->getParameterValue("HiddenNodeChoiceValue"));
				cout<<"---Maximum Hidden Nodes: = "<<maxHiddenNodes<<endl;
				hiddenNodesUsed = generateHiddenNodeLookup_Random();
				break;
			
			case MANUAL:
				cout<<"---Bitmirroring Algorithm: MANUAL---"<<endl;
				maxHiddenNodes = int(NEAT::Globals::getSingleton()->getParameterValue("HiddenNodeChoiceValue"));
				cout<<"---Maximum Hidden Nodes: = "<<maxHiddenNodes<<endl;
				hiddenNodesUsed = generateHiddenNodeLookup_Manual();
				break;
			
			case THRESHOLD:
				cout<<"---Bitmirroring Algorithm: THRESHOLD---"<<endl;
				hiddenNodeThreshold = double(NEAT::Globals::getSingleton()->getParameterValue("HiddenNodeChoiceValue"));
				cout<<"---Threshold = "<<hiddenNodeThreshold<<endl;
				assert(hiddenNodeThreshold <= 1.0);
				break;
			
			case HIGHESTN:
				cout<<"---Bitmirroring Algorithm: HIGHESTN---"<<endl;
				maxHiddenNodes = int(NEAT::Globals::getSingleton()->getParameterValue("HiddenNodeChoiceValue"));
				cout<<"---Highest N: = "<<maxHiddenNodes<<endl;
				break;

			default:
				throw string("Improper Hidden Node Algorithm choice!");
				break;
		}
		
		
#endif //BITMIRROR_HIDDEN_LAYER
		if(convergence_step < 0)
			convergence_step = INT_MAX; // never switch
		
		alreadyEndedRun = false;
		generateSubstrate();
		generateInputToOutputMap();

    }

    NEAT::GeneticPopulation* BitMirroringV2Experiment::createInitialPopulation(int populationSize)
    {
        NEAT::GeneticPopulation* population = new NEAT::GeneticPopulation();

        vector<GeneticNodeGene> genes;

        genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("X1","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Y1","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("X2","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Y2","NetworkSensor",0,false));
		
#ifdef BITMIRROR_HIDDEN_LAYER
		genes.push_back(GeneticNodeGene("Output_ab","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		genes.push_back(GeneticNodeGene("Output_bc","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		genes.push_back(GeneticNodeGene("HiddenStrength","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
#else
        genes.push_back(GeneticNodeGene("Output","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
#endif


        for (size_t a=0;a<populationSize;a++)
        {
            shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,true,1.0));

#ifdef BITMIRROR_TESTMUTATE
//            individual->testMutate();
#endif
			
            population->addIndividual(individual);
        }

        return population;
    }

void BitMirroringV2Experiment::generateSubstrate()
{
        cout << "Generating substrate..."<<endl;
        boost::object_pool<NEAT::NetworkNode> networkNodePool;
        boost::object_pool<NEAT::NetworkLink> networkLinkPool;
		
        NEAT::NetworkNode *nodes = NULL;
        NEAT::NetworkLink *links = NULL;

        try
        {
			//cout<<sizeof(NEAT::NetworkLink)*numNodesX*numNodesY*numNodesX*numNodesY*(numSheets -1)<<endl;
            nodes = new NEAT::NetworkNode[numNodesX*numNodesY*numSheets];
            //cout << numNodesX << " " <<  numNodesY;
			links = new NEAT::NetworkLink[numNodesX*numNodesY*numNodesX*numNodesY*(numSheets-1)];       
		}
		catch (std::exception e)
        {
            cout << e.what() << endl;
        }

        nameLookup.clear();

        cout << "Creating nodes...\n"<<endl;

        int nodeCounter=0;
		
        for (int y1=0;y1<numNodesY;y1++)
        {
            for (int x1=0;x1<numNodesX;x1++)
            {
				
				Node zeroNode(y1,x1,0);
				Node oneNode(y1,x1,1);
				
				string zeroName = toString(x1)+string("/")+toString(y1) + string("/") + toString(0);
				string oneName = toString(x1)+string("/")+toString(y1) + string("/") + toString(1);
				
#ifdef BITMIRROR_HIDDEN_LAYER
				Node twoNode(y1,x1,2);
				string twoName = toString(x1)+string("/")+toString(y1) + string("/") + toString(2);
#endif
				
				nameLookup[zeroNode] = zeroName;
				//nodeLookup[zeroName] = zeroNode;
				new(&nodes[nodeCounter]) NetworkNode(nameLookup[zeroNode]);
				nodeCounter++;
				
				nameLookup[oneNode] = oneName;
				//nodeLookup[oneName] = oneNode;
				new(&nodes[nodeCounter]) NetworkNode(nameLookup[oneNode]);
				nodeCounter++;
				
#ifdef BITMIRROR_HIDDEN_LAYER
				nameLookup[twoNode] = twoName;
				//nodeLookup[twoName] = twoNode;
				new(&nodes[nodeCounter]) NetworkNode(nameLookup[twoNode]);
				nodeCounter++;
#endif
			
            }
        }
		cout<<nodeCounter<<endl;

        cout << "Creating links...\n";

		//loop through each link
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
			//cout<<&nodes[y1*numNodesX*numSheets + x1*numSheets + z1]<<endl;
			//cout<<&nodes[y2*numNodesX*numSheets + x2*numSheets]<<endl;
			
		
		try
		{
			new (&links[linkCounter]) NetworkLink(
				&nodes[y1*numNodesX*numSheets + x1*numSheets + z1],
				&nodes[y2*numNodesX*numSheets + x2*numSheets + z2],
				0.0
				);
			//LHZ - z2 will only = numSheets when recurrence is turned on - when it is, we should 
			//loop the link back to the input
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
		
		}}}}}//done looping through the links

		cout<<linkCounter<<endl;

        cout << "Creating FastNetwork\n";

        substrate = NEAT::FastNetwork<double>(nodes,nodeCounter,links,linkCounter);

        cout << "done!\n";

        delete[] nodes;
        delete[] links;
    
}
#ifdef BITMIRROR_HIDDEN_LAYER

#ifdef HNC_RANDOM
	
/*
 ############################################################
 THE RANDOM ALGORITHM FUNCTION		#########################
 ############################################################
 */

int BitMirroringV2Experiment::generateHiddenNodeLookup_Random()
{
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
		cout<<"in BitMirroringV2Experiment::generateRandomHiddenNodeLookup()"<<endl;
#endif
		vector<std::pair<int,int> > HiddenNodeVector;
		
		for(int y1=0; y1<numNodesY; y1++)
		{
			for(int x1=0; x1<numNodesX; x1++)
			{
				HiddenNodeLookup[pair<int,int>(y1,x1)] = false;
				HiddenNodeVector.push_back(pair<int,int>(y1,x1));
			}
		}
		
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
		cout<<"maxHiddenNodes: "<<maxHiddenNodes<<endl;
		cout<<"Size of HiddenNodeVector: "<<HiddenNodeVector.size()<<endl;
#endif //BIT_MIRRORING_V2_EXPERIMENT_DEBUG
		srand(NEAT::Globals::getSingleton()->getRandom().getSeed());                                        //reset the STL RNG so it works the same in Hybrid in Hyper and FT (should not affect non-hybrid)
        NEAT::Globals::getSingleton()->seedRandom(NEAT::Globals::getSingleton()->getRandom().getSeed());    //same for the HyperNEAT RNG
		int numshuffles = NEAT::Globals::getSingleton()->getRandom().getRandomInt(1000);
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
		cout<<"numshuffles "<<numshuffles<<endl;
#endif //BIT_MIRRORING_V2_EXPERIMENT_DEBUG
		for(int i=0; i<numshuffles; i++)
		{
			random_shuffle(HiddenNodeVector.begin(),  HiddenNodeVector.end());
		}
	int nodecount = 0;
		for(int i=0; i<maxHiddenNodes; ++i)
		{
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
			//cout<<HiddenNodeVector.back().first()<<endl;
#endif //BIT_MIRRORING_V2_EXPERIMENT_DEBUG
			HiddenNodeLookup[HiddenNodeVector.back()] = true;
			HiddenNodeVector.pop_back();
			nodecount++;
		}
		
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
		cout<<"Hidden Layer for this Experiment"<<endl;
		for(int y1=0; y1<numNodesY; y1++)
		{
			for(int x1=0; x1<numNodesX; x1++)
			{
				cout<<y1<<" "<<x1<<" "<<HiddenNodeLookup[pair<int,int>(y1,x1)]<<endl;
			}
		}
#endif //BIT_MIRRORING_V2_EXPERIMENT_DEBUG
	return nodecount;
}

#else
	int BitMirroringV2Experiment::generateHiddenNodeLookup_Random()
	{
		cout<<"Random not compiled"<<endl;
		return -1;
	}
#endif //HNC_RANDOM
	
#ifdef HNC_MANUAL
	
	
/*
 ############################################################
 THE MANUAL ALGORITHM FUNCTION		#########################
 ############################################################
*/
	
int BitMirroringV2Experiment::generateHiddenNodeLookup_Manual()
{
		
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
		cout<<"in BitMirroringV2Experiment::generateManualHiddenNodeLookup()"<<endl;
#endif
		map< std::pair<int,int>, int > HiddenNodeLimitLookup;
		
		//this configuration tries to be as symmetrical as possible \
		and starting at the 'center' of the hidden node grid.
		
		HiddenNodeLimitLookup[pair<int,int>(0,0)] = 32;
		HiddenNodeLimitLookup[pair<int,int>(1,0)] = 41;
		HiddenNodeLimitLookup[pair<int,int>(2,0)] = 45;
		HiddenNodeLimitLookup[pair<int,int>(3,0)] = 26;
		HiddenNodeLimitLookup[pair<int,int>(4,0)] = 42;
		HiddenNodeLimitLookup[pair<int,int>(5,0)] = 38;
		HiddenNodeLimitLookup[pair<int,int>(6,0)] = 30;
		
		HiddenNodeLimitLookup[pair<int,int>(0,1)] = 34;
		HiddenNodeLimitLookup[pair<int,int>(1,1)] = 16;
		HiddenNodeLimitLookup[pair<int,int>(2,1)] = 24;
		HiddenNodeLimitLookup[pair<int,int>(3,1)] = 10;
		HiddenNodeLimitLookup[pair<int,int>(4,1)] = 22;
		HiddenNodeLimitLookup[pair<int,int>(5,1)] = 14;
		HiddenNodeLimitLookup[pair<int,int>(6,1)] = 36;
		
		HiddenNodeLimitLookup[pair<int,int>(0,2)] = 48;
		HiddenNodeLimitLookup[pair<int,int>(1,2)] = 18;
		HiddenNodeLimitLookup[pair<int,int>(2,2)] = 8;
		HiddenNodeLimitLookup[pair<int,int>(3,2)] = 2;
		HiddenNodeLimitLookup[pair<int,int>(4,2)] = 6;
		HiddenNodeLimitLookup[pair<int,int>(5,2)] = 20;
		HiddenNodeLimitLookup[pair<int,int>(6,2)] = 46;
		
		HiddenNodeLimitLookup[pair<int,int>(0,3)] = 29;
		HiddenNodeLimitLookup[pair<int,int>(1,3)] = 13;
		HiddenNodeLimitLookup[pair<int,int>(2,3)] = 5;
		HiddenNodeLimitLookup[pair<int,int>(3,3)] = 1;
		HiddenNodeLimitLookup[pair<int,int>(4,3)] = 4;
		HiddenNodeLimitLookup[pair<int,int>(5,3)] = 12;
		HiddenNodeLimitLookup[pair<int,int>(6,3)] = 28;
		
		HiddenNodeLimitLookup[pair<int,int>(0,4)] = 47;
		HiddenNodeLimitLookup[pair<int,int>(1,4)] = 21;
		HiddenNodeLimitLookup[pair<int,int>(2,4)] = 7;
		HiddenNodeLimitLookup[pair<int,int>(3,4)] = 3;
		HiddenNodeLimitLookup[pair<int,int>(4,4)] = 9;
		HiddenNodeLimitLookup[pair<int,int>(5,4)] = 19;
		HiddenNodeLimitLookup[pair<int,int>(6,4)] = 49;
		
		HiddenNodeLimitLookup[pair<int,int>(0,5)] = 37;
		HiddenNodeLimitLookup[pair<int,int>(1,5)] = 15;
		HiddenNodeLimitLookup[pair<int,int>(2,5)] = 23;
		HiddenNodeLimitLookup[pair<int,int>(3,5)] = 11;
		HiddenNodeLimitLookup[pair<int,int>(4,5)] = 25;
		HiddenNodeLimitLookup[pair<int,int>(5,5)] = 17;
		HiddenNodeLimitLookup[pair<int,int>(6,5)] = 35;
		
		HiddenNodeLimitLookup[pair<int,int>(0,6)] = 31;
		HiddenNodeLimitLookup[pair<int,int>(1,6)] = 39;
		HiddenNodeLimitLookup[pair<int,int>(2,6)] = 43;
		HiddenNodeLimitLookup[pair<int,int>(3,6)] = 27;
		HiddenNodeLimitLookup[pair<int,int>(4,6)] = 44;
		HiddenNodeLimitLookup[pair<int,int>(5,6)] = 40;
		HiddenNodeLimitLookup[pair<int,int>(6,6)] = 33;
		
		
	for (int y1=0; y1<numNodesY; y1++)
	{
		for (int x1=0; x1<numNodesX; x1++)
		{
			if (HiddenNodeLimitLookup[pair<int,int>(y1,x1)] <= maxHiddenNodes) {
				
				HiddenNodeLookup[pair<int,int>(y1,x1)] = true;
			}
			else {
				
				HiddenNodeLookup[pair<int,int>(y1,x1)] = false;
			}
		}
	}
	
	int nodecount = getHiddenNodeCount();
	return nodecount;
}

#else
	int BitMirroringV2Experiment::generateHiddenNodeLookup_Manual()
	{
		cout<<"Manual not compiled"<<endl;
		return -1;
	}
	
#endif //HNC_MANUAL
	
#ifdef HNC_THRESHOLD
/*
 ############################################################
 THE THRESHOLD ALGORITHM FUNCTION	#########################
 ############################################################
 */
	
int BitMirroringV2Experiment::generateHiddenNodeLookup_Threshold(NEAT::FastNetwork<double> &network)
{
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
	cout<<"in BitMirroringV2Experiment::generateThresholdHiddenNodeLookup()"<<endl;
#endif
	
	for (int y1=0; y1<numNodesY; y1++) 
	{
		for (int x1=0; x1<numNodesX; x1++)
		{
			
			network.reinitialize();
			network.setValue("X1",x1);
			network.setValue("Y1",y1);
			network.setValue("X2",0);
			network.setValue("Y2",0);
			
			network.setValue("Bias",0.3);                       				
			
			network.update();
			
			double hiddenStrength = fabs(network.getValue("HiddenStrength"));
			
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
			cout<<"Hidden Strength for ("<<y1<<","<<x1<<"): "<<hiddenStrength<<endl;
#endif
			if(hiddenStrength < hiddenNodeThreshold)
			{
				HiddenNodeLookup[pair<int,int>(y1,x1)] = false;
			}
			else 
			{
				HiddenNodeLookup[pair<int,int>(y1,x1)] = true;
			}
		}
	}
	
	int nodecount = getHiddenNodeCount();
	return nodecount;
}	

#else
	int BitMirroringV2Experiment::generateHiddenNodeLookup_Threshold(NEAT::FastNetwork<double> &network)
	{
		cout<<"Threshold not compiled"<<endl;
		return -1;
	}
	
#endif //HNC_THRESHOLD
	
#ifdef HNC_HIGHEST_N
	
/*
 ############################################################
 THE HIGHEST N ALGORITHM FUNCTION	#########################
 ############################################################
*/
	
//helper function for highest N - sorts each node based on its 'strength' value
bool compareNodeStrength(pair<pair<int,int> ,double> one, pair<pair<int,int> ,double> two)
{
	if (one.second<two.second) return true;
	else return false;
}


int BitMirroringV2Experiment::generateHiddenNodeLookup_HighestN(NEAT::FastNetwork<double> &network)
{
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
	cout<<"in BitMirroringV2Experiment::generateHighestNHiddenNodeLookup()"<<endl;
#endif
	list< pair<pair<int,int>,double> > HiddenNodeStrengths;
	
	for (int y1=0; y1<numNodesY; y1++) 
	{
		for (int x1=0; x1<numNodesX; x1++)
		{
			//initialize the Hidden Node Lookup table
			HiddenNodeLookup[pair<int,int>(y1,x1)] = false;
			
			//query the individual's CPPN genome
			network.reinitialize();
			
			network.setValue("X1",x1);
			network.setValue("Y1",y1);
			network.setValue("X2",0);
			network.setValue("Y2",0);
			network.setValue("Bias",0.3);  
			
			network.update();                             
			
			//construct our list element, put it at the end of HiddenNodeStrengths
			pair<int,int> hiddenName = pair<int,int>(y1,x1);
			double hiddenStrength = fabs(network.getValue("HiddenStrength"));
			
			HiddenNodeStrengths.push_back(pair<pair<int,int>,double>(hiddenName,hiddenStrength));
		}
	}
	
	HiddenNodeStrengths.sort(compareNodeStrength); //sort the nodes from 'weakest' to 'strongest'
	HiddenNodeStrengths.reverse(); //reverse the order, to go from 'strongest' to 'weakest'

	
	list< pair<pair<int,int>,double> >::iterator strengthsIt;
	int ncount = 0;
	for (strengthsIt = HiddenNodeStrengths.begin(); strengthsIt != HiddenNodeStrengths.end(); ++strengthsIt)
	{
		ncount++;
		if (ncount > maxHiddenNodes) break;
		HiddenNodeLookup[(*strengthsIt).first] = true;
		
	}
	
	int nodecount = getHiddenNodeCount();
	return nodecount;
}

#else 
	int BitMirroringV2Experiment::generateHiddenNodeLookup_HighestN(NEAT::FastNetwork<double> &network)
	{
		cout<<"highestN not compiled"<<endl;
		return -1;
	}
	
#endif //HNC_HIGHEST_N
	
#endif //BITMIRROR_HIDDEN_LAYER

    void BitMirroringV2Experiment::generateInputToOutputMap()
    {
        
        //cout << "entering BitMirroringV2Experiment::generateInputToOutputMap()" << endl;
        
        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
        cout << "entering BitMirroringV2Experiment::generateInputToOutputMap()" << endl;
        #endif
        vector< vector <int> > temp_coordinate_pair_holder; // initially holds all coordinate pairs, then they are taken out one at a time randomly and put in input_to_output_map_y
        vector< vector <int> > constrained_coordinate_pair_holder; // initially holds all coordinate pairs, then we randomize them and the first numNodesConstrained nodes are constrained
        vector< vector <int> > real_temp_coordinate_pair_holder; // holds those that were found unsuitable for a few seconds, and then they get added back in
        
        vector <int > coordinatePair;
        
        for(int yi=0;yi<numNodesY;yi++)                                    //place each x and y coordinate pair in the temp holder
        {            
            for (int x=0;x<numNodesX;x++)
            {
              coordinatePair.push_back(x);
              coordinatePair.push_back(yi);
              temp_coordinate_pair_holder.push_back(coordinatePair);
              coordinatePair.clear();              
            }
        }

        vector< int > tmpCoordPair;
        tmpCoordPair.push_back(-1);
        tmpCoordPair.push_back(-1);
        vector< vector < int > > tmpRowCoordPair;
        for(int yi=0;yi<numNodesY;yi++)                                    //pre-build up the entire input_to_output_map
        {            
            for (int x=0;x<numNodesX;x++)
            {
              tmpRowCoordPair.push_back(tmpCoordPair);
            }
            input_to_output_map_y.push_back(tmpRowCoordPair);
            tmpRowCoordPair.clear();
        }

        srand(NEAT::Globals::getSingleton()->getRandom().getSeed());                                        //reset the STL RNG so it works the same in Hybrid in Hyper and FT (should not affect non-hybrid)
        NEAT::Globals::getSingleton()->seedRandom(NEAT::Globals::getSingleton()->getRandom().getSeed());    //same for the HyperNEAT RNG
        
        //randomize the order of the coordinates 
        random_shuffle(temp_coordinate_pair_holder.begin(), temp_coordinate_pair_holder.end());
        
        bool needOne = true;
        bool TargetConstrainedToRow = NEAT::Globals::getSingleton()->getParameterValue("TargetConstrainedToRow");
        int percentConstrained = int(NEAT::Globals::getSingleton()->getParameterValue("PercentConstrained"));       
        int regularityConstrains = int(NEAT::Globals::getSingleton()->getParameterValue("RegularityConstrains"));       //1= row, 2 = column

        int numNodesConstrained = int((percentConstrained/100.0)*numNodesX*numNodesY); 
        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
        PRINT(numNodesConstrained);
        #endif

        constrained_coordinate_pair_holder = temp_coordinate_pair_holder;                                     //this will eventually hold those randomly chosen to be constrained
        for(int j=0;j<NEAT::Globals::getSingleton()->getRandom().getRandomInt(1000);j++)                      //makes the randomization based on the seed
        {
           random_shuffle(constrained_coordinate_pair_holder.begin(), constrained_coordinate_pair_holder.end()); //randomize this one to prevent some regularity
        }
        for(int q=0;q<numNodesX*numNodesY-numNodesConstrained;q++) constrained_coordinate_pair_holder.pop_back(); //leave only numNodesConstrained in here

        for(int q=0;q<constrained_coordinate_pair_holder.size();q++)
        {
            int y = constrained_coordinate_pair_holder[q][1];
            int x = constrained_coordinate_pair_holder[q][0];
            needOne = true;
            
            while(needOne)
            {
                if (TargetConstrainedToRow)
                {
                      if(temp_coordinate_pair_holder[temp_coordinate_pair_holder.size()-1][1] != y)   //if the target is not in the same row, move on
                      {
                          real_temp_coordinate_pair_holder.push_back(temp_coordinate_pair_holder.back());  //temporarily take it out (we'll put it back once we've found a candidate)
                          temp_coordinate_pair_holder.pop_back();
                          continue;
                      }
                }

                if(regularityConstrains==1) //constraining row
                {
                    //now find one in the right column
                    if(temp_coordinate_pair_holder[temp_coordinate_pair_holder.size()-1][1] != y)   //if the target is not in the same row, move on
                    {
                        real_temp_coordinate_pair_holder.push_back(temp_coordinate_pair_holder.back());       //temporarily take it out (we'll put it back once we've found a candidate)
                        temp_coordinate_pair_holder.pop_back();
                        continue;
                    }
                }
                else if(regularityConstrains==2) //constraining column
                {
                    //now find one in the right column
                    if(temp_coordinate_pair_holder[temp_coordinate_pair_holder.size()-1][0] != x)   //if the target is not in the same row, move on
                    {
                        real_temp_coordinate_pair_holder.push_back(temp_coordinate_pair_holder.back());       //temporarily take it out (we'll put it back once we've found a candidate)
                        temp_coordinate_pair_holder.pop_back();
                        continue;
                    }
                }

                input_to_output_map_y[constrained_coordinate_pair_holder[q][1]][constrained_coordinate_pair_holder[q][0]]
                    = temp_coordinate_pair_holder[temp_coordinate_pair_holder.size()-1]; //set the one we want to the right place in the map                    
                temp_coordinate_pair_holder.pop_back();                                                              //delete the one we actually took
                needOne = false;
                                
                while(real_temp_coordinate_pair_holder.size()>0)
                {
                    temp_coordinate_pair_holder.push_back(real_temp_coordinate_pair_holder.back());                  //put back the ones we found unsuitable
                    real_temp_coordinate_pair_holder.pop_back();
                }                
            }                    
        }

        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
        cout << "the constrained input to output map is" << endl;
        for(int i=0;i<input_to_output_map_y.size();i++)
        {
          for(int j=0;j<input_to_output_map_y[i].size();j++)
          {
            cout << "(" << input_to_output_map_y[i][j][0] << "," << input_to_output_map_y[i][j][1] << ") ";
          }
          cout << endl;
        }
        #endif
        
        // now we can fill in in all the non-constrained nodes                    
        for (int y=0;y<numNodesY;y++)                                    //now randomly take an output coordinate pair and see if it meets our criteria for a target
        {
            for (int x=0;x<numNodesX;x++) //for each x cell in this row, take one of the remaining coordinates to connect to
            {
                if (input_to_output_map_y[y][x][0] != -1) continue; //if this node has been set, continue
                needOne = true;
                                
                while(needOne)
                {
                    if (TargetConstrainedToRow)
                    {
                          if(temp_coordinate_pair_holder[temp_coordinate_pair_holder.size()-1][1] != y)   //if the target is not in the same row, move on
                          {
                              real_temp_coordinate_pair_holder.push_back(temp_coordinate_pair_holder.back());
                              temp_coordinate_pair_holder.pop_back();
                              continue;
                          }
                    }



                    input_to_output_map_y[y][x] = temp_coordinate_pair_holder[temp_coordinate_pair_holder.size()-1]; //grab the one we want                    
                    temp_coordinate_pair_holder.pop_back();                                                              //delete the one we actually took
                    needOne = false;
                    
                    while(real_temp_coordinate_pair_holder.size()>0)
                    {
                        temp_coordinate_pair_holder.push_back(real_temp_coordinate_pair_holder.back());                  //put back the ones we found unsuitable
                        real_temp_coordinate_pair_holder.pop_back();
                    }                    
                }
            }
        }
        
        
        //prints out the mapping for debugging
        cout << "the input to output map for HyperNEAT is" << endl;
        for(int i=0;i<input_to_output_map_y.size();i++)
        {
          for(int j=0;j<input_to_output_map_y[i].size();j++)
          {
            cout << "(" << input_to_output_map_y[i][j][0] << "," << input_to_output_map_y[i][j][1] << ") ";
          }
          cout << endl;
        }
        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
        cout << "leaving BitMirroringV2Experiment::generateInputToOutputMap()" << endl;       
        #endif

    }

	NEAT::FastNetwork<double> BitMirroringV2Experiment::populateAndReturnSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual) {
		populateSubstrate(individual);
		return substrate;
	}
	
/*
 ############################################################
 THE POPULATE SUBSTRATE FUNCTION	#########################
 ############################################################
*/
    void BitMirroringV2Experiment::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual)  //JMC: should this be refactored? seems to exist in every experiment
    {
        //cout << "Populating substrate...";
        NEAT::FastNetwork <double> network = individual->spawnFastPhenotypeStack<double>();        //JMC: this network is the CPPN used to produce the substrate (neural network)
        //progress_timer t;
        
		
#ifdef BITMIRROR_HIDDEN_LAYER
		double cppnLinkWeightFromInputToHiddenSheet = 0.0;
		double cppnLinkWeightFromHiddenToOutputSheet = 0.0;
		double substrateLinkWeightFromInputToHiddenSheet = 0.0;
		double substrateLinkWeightFromHiddenToOutputSheet = 0.0;
#else
		double cppnLinkWeightFromInputToOutputSheet = 0.0;
		double substrateLinkWeightFromInputToOutputSheet = 0.0;
#endif
		
#ifdef BITMIRROR_HIDDEN_LAYER
		switch (hiddenAlgorithm) {
			case RANDOM:
				assert(hiddenNodesUsed == maxHiddenNodes);
				break;
				
			case MANUAL:
				assert(hiddenNodesUsed == maxHiddenNodes);
				break;
				
			case THRESHOLD:
				hiddenNodesUsed = generateHiddenNodeLookup_Threshold(network);
				break;
				
			case HIGHESTN:
				hiddenNodesUsed = generateHiddenNodeLookup_HighestN(network);
				assert(hiddenNodesUsed == maxHiddenNodes);
				break;
				
			default:
				throw string("Improper Hidden Node Algorithm choice!");
				break;
		}		

		
		if(individualstats) individualstats->addStat(hiddenNodesUsed);
		
#endif //BITMIRROR_HIDDEN_LAYER
		
        int linkCounter=0;
        double x1Val,y1Val,x2Val,y2Val;
        for (int y1=0;y1<numNodesY;y1++)
        {
            for (int x1=0;x1<numNodesX;x1++)
            {
                for (int y2=0;y2<numNodesY;y2++)
                {
                    for (int x2=0;x2<numNodesX;x2++)
                    {
                        x1Val = (x1-numNodesX/2)/double((numNodesX-1)/2);   //JMC: these four lines map numNodes onto a grid centered at zero, and only work for odd by odd grids
                        y1Val = (y1-numNodesY/2)/double((numNodesY-1)/2);   //JMC: e.g 3 x 3 = -1 0 1, 5x5 = -2 -1 0 1 2
                        x2Val = (x2-numNodesX/2)/double((numNodesX-1)/2);
                        y2Val = (y2-numNodesY/2)/double((numNodesY-1)/2);
						
                        
                        network.reinitialize();
                        network.setValue("X1",x1Val);
                        network.setValue("Y1",y1Val);
                        network.setValue("X2",x2Val);
                        network.setValue("Y2",y2Val);

                        network.setValue("Bias",0.3);                       //JMC: we have just set the inputs to the     to be x1,y1,x2,y2,bias
                        
                        
                        //cout << "RUNNING UPDATE ON CPPN----------------------------------------------------------------------------------------" << endl;
                        network.update();                                   //JMC: on this line we run the CPPN network...  
                        //cout << "DONE RUNNING UPDATE ON CPPN----------------------------------------------------------------------------------------" << endl;
						
#ifdef BITMIRROR_HIDDEN_LAYER
						
						if(HiddenNodeLookup[pair<int,int>(y2,x2)])
						{
							cppnLinkWeightFromInputToHiddenSheet = double(network.getValue("Output_ab"));
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
//							cout<<"LinkAB: ("<<y1<<","<<x1<<") -> ("<<y2<<","<<x2<<"):	ON ON ON"<<endl;
#endif
						}
						else
						{
							cppnLinkWeightFromInputToHiddenSheet = 0;
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
//							cout<<"LinkAB: ("<<y1<<","<<x1<<") -> ("<<y2<<","<<x2<<"):	OFF"<<endl;
#endif
						}
						 
#ifndef HIDDEN_TO_OUTPUT_SOLVED						
						
						if(HiddenNodeLookup[pair<int,int>(y1,x1)])// ||*/ (pair<int,int>(y2,x2) == pair<int,int>(y1,x1)))
						{
							cppnLinkWeightFromHiddenToOutputSheet = double(network.getValue("Output_bc"));
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
							cout<<"LinkBC: ("<<y1<<","<<x1<<") = "<<cppnLinkWeightFromHiddenToOutputSheet<<endl;
#endif
						}
						else
						{

							cppnLinkWeightFromHiddenToOutputSheet = 0;
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
							cout<<"LinkBC: ("<<y1<<","<<x1<<") -> ("<<y2<<","<<x2<<") = "<<cppnLinkWeightFromHiddenToOutputSheet<<endl;
#endif
						}
#else //HIDDEN_TO_OUTPUT_SOLVED
						if(pair<int,int>(y2,x2) == pair<int,int>(y1,x1))
						{
							
							//cppnLinkWeightFromInputToHiddenSheet = 1;
							
							cppnLinkWeightFromHiddenToOutputSheet = 1;
							
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
							cout<<"LinkBC: ("<<y1<<","<<x1<<") -> ("<<y2<<","<<x2<<")= "<<cppnLinkWeightFromHiddenToOutputSheet<<endl;
#endif
						}
						else
						{
							//cppnLinkWeightFromInputToHiddenSheet = 0;
							cppnLinkWeightFromHiddenToOutputSheet = 0;
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
//							cout<<y1<<","<<x1<<" OFF"<<endl;
#endif
						}
#endif //HIDDEN_TO_OUTPUT_SOLVED
						
#else  //BITMIRROR_HIDDEN_LAYER
						
						cppnLinkWeightFromInputToOutputSheet = double(network.getValue("Output"));	
						
#endif //BITMIRROR_HIDDEN_LAYER		
						
						
						Node fromInputNode (y1, x1, 0); // z=0 cause its the input layer
						Node *ptr_toNode;
						
#ifdef BITMIRROR_HIDDEN_LAYER
						Node toHiddenNode  (y2, x2, 1); 
						Node fromHiddenNode(y1, x1, 1);
						Node toOutputNode  (y2, x2, 2);
						
						ptr_toNode = &toHiddenNode;
						
#else
						Node toOutputNode  (y2, x2, 1);
						//Node fromOutputNode (y1, x1, 1);
						//Node toInputNode (y2, x2, 0);
						ptr_toNode = &toOutputNode;
#endif
						
						
                        //substrate.getLink(counter)->weight = ( greyVal*3.0 );       
                                                                                    
						if ((substrate.getLink(linkCounter)) == substrate.getLink(nameLookup[fromInputNode],nameLookup[*ptr_toNode])) {}
						else {cout << "not the same " << linkCounter << " "<<nameLookup[fromInputNode]<<" "<<nameLookup[*ptr_toNode]<<endl;exit(9);}
						
						//get the cppn output and set it to the relevant link the substrate
#ifdef BITMIRROR_HIDDEN_LAYER
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


                    }
                }
            }
        }

        //cout << "done!\n";

        //cout << "Number of expressed links: " << linkCount << endl;
    }

#ifndef MANUAL_NORMALIZATION
	double BitMirroringV2Experiment::zeroOrNormalize(double mWeightToAlter){
		if (fabs(mWeightToAlter) > 0.2)                              //JMC: if the absolute value is greater than a threshold (here .2)... 
		{
			if (mWeightToAlter > 0.0)
				return ( ((mWeightToAlter-0.2)/0.8)*3.0 );   //JMC: ..then normalize it (0 to 3) and set it as the weight in the substrate (NN)
			else
				return ( ((mWeightToAlter+0.2)/0.8)*3.0 );   //JMC: same thing, but if it is negative, it stays so (and is thus inhibitory)  
		}
		else
		{
			return (0.0);
		}
	}
#else
	double BitMirroringV2Experiment::zeroOrNormalize(double mWeightToAlter){
		if (fabs(mWeightToAlter) > normalization_cutoff)                              //JMC: if the absolute value is greater than a threshold (here .2)... 
		{
			if (mWeightToAlter > 0.0)
				return ( ((mWeightToAlter-normalization_cutoff)/(1-normalization_cutoff))*3.0 );   //JMC: ..then normalize it (0 to 3) and set it as the weight in the substrate (NN)
			else
				return ( ((mWeightToAlter+normalization_cutoff)/(1-normalization_cutoff))*3.0 );   //JMC: same thing, but if it is negative, it stays so (and is thus inhibitory)  
		}
		else
		{
			return (0.0);
		}
	}
#endif
		
    double BitMirroringV2Experiment::processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual)
    {
    
        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
//        cout << "JMC: evaluating fitness in BitMirroringV2Experiment::processEvaluation\n";
        #endif

        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
  //              cout << "Initializing substrate\n";
        #endif

        substrate.reinitialize();
        substrate.dummyActivation();

        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
    //            cout << "Setting values\n";
        #endif

        vector< vector<double> > randomized_input_values;        
        randomized_input_values = get_randomized_input_values();   //go through each coordinate on the input plane set a corresponding value in the randomized_input_values to 0 or 1
        
        int xPos = 0;
        int yPos = 0;
        
        for (int yi=0;yi<numNodesY;yi++)                                    //go through each coordinate on the input plane set to the corresponding random int in randomized_input_values
        {
            for (int xj=0;xj<numNodesX;xj++)
            {
                xPos = xj-numNodesX/2;      //@jmc: nameLookup takes e.g. (-4,-4) and returns a string "-4/-4" which is the 'node name'
                yPos = yi-numNodesY/2;      //@jmc: the -4/-4 is simply the coordinates of the node (here -5 to 5 = 11 total, for x and y)
				Node node(yi,xj,0);

                #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG            
                cout << "JMC: setting the value of nameLookup (a.ka. node name): "
                     << nameLookup[node] 
                     << "to: " << randomized_input_values[yi][xj] << endl;
                #endif
				
				

                try
                {
                        substrate.setValue(                                     
                            nameLookup[node],               //gets the node name for xPos, yPos
                            randomized_input_values[yi][xj]                     //what the node gets set to
                        );
                }
                catch (...)
                {
                    cout << "INVALID LOCATION: " << yPos << ',' << xPos << endl;
                    exit(3);
                }

            }
                    
        }

#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
                //start (this prints out the input plane)
/*                cout << "the input plane is" << endl;
                for (int yi=0;yi<numNodesY;yi++)                                   
                {
                    for (int xj=0;xj<numNodesX;xj++)
                    {
                        cout << substrate.getValue(nameLookup[pair<int,int>((yi-numNodesY/2),(xj-numNodesX/2))]) << " ";
                    
                    }
                    
                    cout << endl;
                }

                //end

        cout << "Updating Substrate\n";
  */      
#endif


        int num_correct = 0;
        double target_value;
        double output_value;

        //(note: there is no real input and output plane. it is the same plane. updating the network changes the inputs to outputs. this must mean that there are links from, say, -4/-4 to itself, which is equivalent to -4/-4 in the input plane to -4/-4 in the output plane
#if BIT_MIRRORING_V2_EXPERIMENT_DEBUG == 2
        cout << "RUNNING UPDATE ONE ON SUBSTRATE----------------------------------------------------------------------------------------"<< endl;
		cout << "State of the nodes pre-update\n";
		for (int z=0;z<numSheets;z++)
		{
			cout << "Printing Layer " << z << "************************************************" << endl;
			for (int y1=0;y1<numNodesY;y1++)
			{
				for (int x1=0;x1<numNodesX;x1++)
				{
					//cout << "(x,y,z): (" << x1 << "," << y1 << "," << z << "): " ;
					cout << setw(12) << setiosflags(ios::right) << fixed << 
					double(substrate.getValue(
													   nameLookup[HCUBE::Node(x1,y1,z)]
													   ));
				}
				cout << endl;
			}
		}
		
		
		cout << "Updating Substrate\n";
		//CREATE_PAUSE(string("Line Number: ")+toString(__LINE__));

		
		//have to update the network once for each layer of *links* (or NumLayers -1)

		for (int a=0;a< numSheets - 1 ;a++)
		{
			substrate.update(1);
			
			cout << "State of the nodes post-update: " << a << endl;
			for (int z=0;z<numSheets;z++)
			{
				cout << "Printing Layer " << z << endl;
				for (int y1=0;y1<numNodesY;y1++)
				{
					for (int x1=0;x1<numNodesX;x1++)
					{
						//cout << "(x,y,z): (" << x1 << "," << y1 << "," << z << "): " ;
						cout << double(substrate.getValue(nameLookup[HCUBE::Node(x1,y1,z)]));
						cout << " ";
					}
					cout << endl;
				}
			}
		}
		cout << "DONE RUNNING UPDATE ONE ON SUBSTRATE----------------------------------------------------------------------------------------"<< endl;
#else
		substrate.update(numSheets-1);
#endif 
        
        //jmc: note: only works if input and output planes are of the same size
        if(numNodesX != numNodesY)
        {
          cout << "error: numNodesX must equal numNodesY the way jmc has set this code up." << endl;
          exit(2);
        }
        
        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
        cout << "target output: (actual output) " << endl;
        #endif
        for (int y2=0;y2<numNodesY;y2++)                                    //go through each coordinate on the output plane and bump num_correct if the input & output are >0, or if input and output are <=0 (any positive weight between each input and output should work)
        {
            for (int x2=0;x2<numNodesX;x2++)
            {
				//Node node((y2-numNodesY/2),(x2-numNodesX/2),0);
#ifdef BITMIRROR_HIDDEN_LAYER
				Node node(y2,x2,2);
#else
				Node node(y2,x2,1);
#endif

                target_value  = targetOutput(y2,x2, randomized_input_values);                                       //@jmc target value now can either be straight across (GeometricRegularity of 1.0, or a random (but constant through the run) association (GeoReg 0.0)

                output_value = substrate.getValue(nameLookup[node]);  //@jmc /2 because half are neg and half are pos (cartesian grid centered on 0)
                #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
                cout << setw(2) << target_value << " ";
                cout << "(" << output_value << ") " ;
                #endif
                if (target_value > 0 && output_value > 0)
                {
                    num_correct++;
                }
                else if (target_value <= 0 && output_value <= 0)
                {
                    num_correct++;
                }
            }
            #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
            cout << endl;
            #endif
        }
        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
        PRINT(num_correct);
        #endif
        
        return num_correct;
    }

    void BitMirroringV2Experiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
    {
        shared_ptr<NEAT::GeneticIndividual> individual = group.front();
#ifdef BITMIRROR_HIDDEN_LAYER
		individualstats = shared_ptr<BitMirroringV2Stats>(new BitMirroringV2Stats());
		individual->setUserData(individualstats);
#endif //BITMIRROR_HIDDEN_LAYER

        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
          cout << "in BitMirroringV2Experiment.cpp::processIndividual, processing individual:  " << individual << endl;
        #endif
        double fitness =0;
        double maxFitness = 0;

        populateSubstrate(individual);
        
        for(int k=0;k<10;k++)    // do this 10 times to test for generality (since every time the inputs are randomized)
        {
            fitness += processEvaluation(individual);
            maxFitness += numNodesY*numNodesX;
        }

        if(maxFitness > std::numeric_limits<double>::max())
        {
            cout << "error: the max fitness is greater than the size of the double. " << endl;
            cout << "max double size is: : " << std::numeric_limits<double>::max() << endl;
            cout << "max fitness is: " << maxFitness << endl;
            exit(88);
        } 

        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG        
            cout << "Individual Evaluation complete!\n";
            cout << "maxFitness: " << maxFitness << endl;
            cout << "fitness: " << fitness << endl;
        #endif
  
        individual->reward(pow(1.1,double(fitness)));
        //individual->reward(pow(1.15,double(fitness))); 1.15 does not blow out the double with 11x11 = 121 answers (1.15^121 < INT_MAX) 
        //individual->reward(fitness); //using a linear fitness function because it hangs indefinitely with the function on the previous line

        if (fitness >= maxFitness)
        {
            cout << "PROBLEM DOMAIN SOLVED!!!\n";

            //print network and substrate
            printSubstrate(individual);        
            printNetworkCPPN(individual);    
        }
        if (fitness < 0)
        {
            cout << "Fitness Less Than Zero!!!, it is: " << fitness << "\n";    //note: the org originally got 10 points just for showing up, so unless this is >-9 it should be fine
            exit(10);
        }
		
		if (individual == generation->getIndividual(generation->getIndividualCount()-1))
		{
			double champfit = log(generation->getGenerationChampion()->getFitness())/log(1.1);
			cout << "#######GENERATION CHAMPION	"<<champfit<<endl;
			
			if (terminateOnTargetFitness) {
				
				LastNFitnesses.push_back(champfit);
				LastNFitnesses.pop_front();
				
				double champFitAvg = dequeAverage(LastNFitnesses);
				cout<<"#######Fitness Average	"<<champFitAvg<<endl;
				if (champFitAvg >= terminalFitness && alreadyEndedRun == false)
				{
					alreadyEndedRun = true;
					cout<<"#########################"<<endl;
					cout<<"#########################"<<endl;
					cout<<"terminal fitness reached. ending run"<<endl;
					SET_PARAMETER("MaxGenerations",double(generation->getGenerationNumber()+1));
					//cout<<GET_PARAMETER("MaxGenerations")<<endl;
				}
			}
		}
    }

    void BitMirroringV2Experiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
    {
        /*{
            mutex::scoped_lock scoped_lock(*Globals::ioMutex);
            cout << "Starting Evaluation on object:" << this << endl;
            cout << "Running on individual " << individual << endl;
        }*/

        cout << "Sorry, this was never coded up for postHoc. You'll have to do that now." << endl;
        exit(6);
        
        //You get 10 points just for entering the game, wahooo!
        individual->setFitness(10);

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

    void BitMirroringV2Experiment::printSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
      cout << "Printing substrate" << endl;
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
    
    void BitMirroringV2Experiment::printNetworkCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
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


    Experiment* BitMirroringV2Experiment::clone()
    {
        BitMirroringV2Experiment* experiment = new BitMirroringV2Experiment(*this);
        
        return experiment;
    }
		

    
    double BitMirroringV2Experiment::targetOutput(const int c_y2, const int c_x2, const vector<vector< double> > c_randomized_input_values) 
    {
        int target_output_value;
      
        int corresponding_x1_source = input_to_output_map_y[c_y2][c_x2][0];
        int corresponding_y1_source = input_to_output_map_y[c_y2][c_x2][1];
        target_output_value = (int) c_randomized_input_values[corresponding_y1_source][corresponding_x1_source];
      
      return(target_output_value);
    }

    vector< vector<double> > BitMirroringV2Experiment::get_randomized_input_values()
    {

        vector< vector<double> > randomized_input_values;
        vector<double> randomized_row;
        double temp_val;
        
        for (int y=0;y<numNodesY;y++)                                    //go through each coordinate on the input plane set a corresponding value in the randomized_input_values to 0 or 1
        {
            for (int x=0;x<numNodesX;x++)
            {
                temp_val = NEAT::Globals::getSingleton()->getRandom().getRandomInt(2);
                
                if (temp_val==0) temp_val = -1.0;    
                randomized_row.push_back(temp_val);
            }
            randomized_input_values.push_back(randomized_row);
            randomized_row.clear();
        }

      return(randomized_input_values);
    }


    void BitMirroringV2Experiment::print_vector_of_vectors(const vector<vector<int> > c_vector_of_vectors){
      cout << endl;
      for(int i=0; i< c_vector_of_vectors.size();i++) {//print rows in reverse order (since x=0 is the lowest)                 
        for(int j=0;j<c_vector_of_vectors[i].size();j++){
          cout << c_vector_of_vectors[i][j] << " ";
        }
        cout << endl;
      }
    }

    void BitMirroringV2Experiment::print_vector_of_vectors(const vector<vector<double> > c_vector_of_vectors){
      cout << endl;
      for(int i=0; i< c_vector_of_vectors.size();i++) {//print rows in reverse order (since x=0 is the lowest)                 
        for(int j=0;j<c_vector_of_vectors[i].size();j++){
          cout << c_vector_of_vectors[i][j] << " ";
        }
        cout << endl;
      }
    }

	bool BitMirroringV2Experiment::converged(int generation) {
		if(generation == convergence_step)
			return true;
		return false;
	}	
}


