#include "Experiments/HCUBE_BitMirroringV2NoGeomExperiment.h"

namespace HCUBE
{
    using namespace NEAT;

    NEAT::GeneticPopulation* BitMirroringV2NoGeomExperiment::createInitialPopulation(int populationSize)
    {
        NEAT::GeneticPopulation* population = new NEAT::GeneticPopulation();

        vector<GeneticNodeGene> genes;
        vector<GeneticLinkGene> links;

        genes.reserve(10000);
        links.reserve(10000);

        cout << "Creating Nodes\n";

        for (int y1=0;y1<numNodesY;y1++)
        {
            for (int x1=0;x1<numNodesX;x1++)
            {
                genes.push_back(
                    GeneticNodeGene(
                        toString(y1-numNodesY/2)+string("/")+toString(x1-numNodesX/2),
                        "NetworkOutputNode",
                        0,
                        false
                    )
                );
            }
        }

        cout << "Creating links\n";

        for (int y1=0;y1<numNodesY;y1++)
        {
            for (int x1=0;x1<numNodesX;x1++)
            {
                for (int y2=0;y2<numNodesY;y2++)
                {
                    for (int x2=0;x2<numNodesX;x2++)
                    {
                        //cout << "Creating link from "
                        //<< genes[y1*numNodesY+x1].getName() << " to "
                        //<< genes[y2*numNodesY+x2].getName() << endl;
                        links.push_back(
                            GeneticLinkGene(
                                genes[y1*numNodesY+x1].getID(),
                                genes[y2*numNodesY+x2].getID()
                            )
                        );
                    }
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
	
	NEAT::GeneticPopulation* BitMirroringV2NoGeomExperiment::createInitialPopulation(shared_ptr<NEAT::GeneticPopulation> CPPN_pop, shared_ptr<Experiment> HyperNEAT_experiment) {
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

							string sourceString = toString(fromY-numNodesY/2)+string("/")+toString(fromX-numNodesX/2);
							string targetString = toString(toY-numNodesY/2)+string("/")+toString(toX-numNodesX/2);
							
//							string sourceString(toString(fromX)+string("/")+toString(fromY)+ string("/") + toString(0));
							GeneticNodeGene* sourceNode = PNEAT_individaul->getNode(sourceString);
//							string targetString(toString(toX)+string("/")+toString(toY)+ string("/") + toString(1));
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
	

    void BitMirroringV2NoGeomExperiment::generateSubstrate()
    {
        cout << "Generating substrate...";
        boost::object_pool<NEAT::NetworkNode> networkNodePool;
        boost::object_pool<NEAT::NetworkLink> networkLinkPool;
        vector<NEAT::NetworkNode *> nodes;
        vector<NEAT::NetworkLink *> links;
        vector<double> nodeBiases;

        nameLookup.clear();

        for (int y1=0;y1<numNodesY;y1++)
        {
            for (int x1=0;x1<numNodesX;x1++)
            {
                //cout << (y1-numNodesY/2) << '/' << (x1-numNodesX/2) << endl;
                nameLookup[pair<int,int>(y1-numNodesY/2,x1-numNodesX/2)] = toString(y1-numNodesY/2)+string("/")+toString(x1-numNodesX/2);
                nodes.push_back(networkNodePool.construct(nameLookup[pair<int,int>(y1-numNodesY/2,x1-numNodesX/2)]));
                nodeBiases.push_back(0.0);

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

        for (int y1=0;y1<numNodesY;y1++)
        {
            for (int x1=0;x1<numNodesX;x1++)
            {
                for (int y2=0;y2<numNodesY;y2++)
                {
                    for (int x2=0;x2<numNodesX;x2++)
                    {
                        //cout << (y1-numNodesY/2) << '/' << (x1-numNodesX/2) << endl;
                        //cout << (y2-numNodesY/2) << '/' << (x2-numNodesX/2) << endl;
                        //            int result = system("PAUSE");
						//(void) result;

                        //cout << "Making link from " << nodes[y1*numNodesX+x1]->getName() << " to " << nodes[y2*numNodesX+x2]->getName() << endl;
                        //int result = system("PAUSE");
						//(void) result;

						
                        links.push_back(networkLinkPool.construct(nodes[y1*numNodesX+x1],nodes[y2*numNodesX+x2],0.0));
                    }
                }
            }
        }

        substrate = NEAT::FastNetwork<double>(nodes,links);

        substrate.getLink(                            //jmc I don't know what these values mean and why they are hard coded. they don't show up in the Geom experiments
            nameLookup[pair<int,int>(-5,-5)],
            nameLookup[pair<int,int>(-5,-4)]
        )->weight = 0.0;

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
    }

    void BitMirroringV2NoGeomExperiment::generateInputToOutputMap()
    {
        #if BIT_MIRRORING_V2_EXPERIMENT_DEBUG
                cout << "entering BitMirroringV2Experiment::generateInputToOutputMap()" << endl;
        #endif
        vector< vector <int> > temp_coordinate_pair_holder; // initially holds all coordinate target nodes, then they are taken out one at a time randomly and put in input_to_output_map_y
        vector< vector <int> > constrained_coordinate_pair_holder; // initially holds all inputs, then we randomize them and the first numNodesConstrained nodes are constrained in their relationship with their output node
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
        double percentConstrained = NEAT::Globals::getSingleton()->getParameterValue("PercentConstrained");       
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
        cout << "the input to output map for FT-NEAT is" << endl;
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

    void BitMirroringV2NoGeomExperiment::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
        NEAT::FastNetwork <double> network = individual->spawnFastPhenotypeStack<double>();
        double cppnLinkWeight = 0.0;
        double substrateLinkWeight = 0.0;

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
                        
                        //trying to make thing easier for P-Neat to make zeros
                        cppnLinkWeight = 
                            network.getLink(                                                 //network = cppn 
                                nameLookup[pair<int,int>(y1-numNodesY/2,x1-numNodesX/2)],
                                nameLookup[pair<int,int>(y2-numNodesY/2,x2-numNodesX/2)]
                            )->weight;
                            
                            
                        if (fabs(cppnLinkWeight)>0.2)                              //JMC: if the absolute value is greater than a threshold (here .2)... 
                        {
                            if (cppnLinkWeight>0.0)
                                substrateLinkWeight = ( ((cppnLinkWeight-0.2)/0.8)*3.0 );   //JMC: ..then normalize it (0 to 3) and set it as the weight in the substrate (NN)
                            else
                                substrateLinkWeight = ( ((cppnLinkWeight+0.2)/0.8)*3.0 );   //JMC: same thing, but if it is negative, it stays so (and is thus inhibitory)  
                        }
                        else
                        {
                            substrateLinkWeight = (0.0);
                        }

                        //set this new value to be the weight 
                        substrate.getLink(                                                   //substrate = final network we evaluate fitness on   
                            nameLookup[pair<int,int>(y1-numNodesY/2,x1-numNodesX/2)],
                            nameLookup[pair<int,int>(y2-numNodesY/2,x2-numNodesX/2)]
                        )->weight = substrateLinkWeight;

                        //end trying to make thing easier for P-Neat to make zeros


                      //                        //jmc original p-neat code (in which to make a zero you have to be exactly zero)
                      //                        substrate.getLink(                                                   //substrate = final network we evaluate fitness on   
                      //                            nameLookup[pair<int,int>(y1-numNodesY/2,x1-numNodesX/2)],
                      //                            nameLookup[pair<int,int>(y2-numNodesY/2,x2-numNodesX/2)]
                      //                        )->weight =
                      //                            network.getLink(                                                 //network = cppn 
                      //                                nameLookup[pair<int,int>(y1-numNodesY/2,x1-numNodesX/2)],
                      //                                nameLookup[pair<int,int>(y2-numNodesY/2,x2-numNodesX/2)]
                      //                            )->weight;
                      //                        //jmc end original p-neat code

                    }
                }
            }
        }
    }

    double BitMirroringV2NoGeomExperiment::processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual)
    {
    
        #if BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG
        cout << "JMC: evaluating fitness in BitMirroringV2NoGeomExperiment::processEvaluation\n";
        #endif
        

        #if BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG
        cout << "Initializing substrate\n";
        #endif

        substrate.reinitialize();
        substrate.dummyActivation();

        #if BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG
        cout << "Setting values\n";
        #endif

        vector< vector<double> > randomized_input_values;        
        randomized_input_values = get_randomized_input_values();   //go through each coordinate on the input plane set a corresponding value in the randomized_input_values to 0 or 1
        
        
        for (int yi=0;yi<numNodesY;yi++)                                    //go through each coordinate on the input plane set to the corresponding random int in randomized_input_values
        {
            for (int xj=0;xj<numNodesX;xj++)
            {
            
                //cout << "JMC: setting the value of nameLookup (a.ka. node name): " << nameLookup[pair<int,int>(yi-numNodesY/2,xj-numNodesX/2)] << endl;

                try
                {
                        substrate.setValue(                                     
                            nameLookup[pair<int,int>(yi-numNodesY/2,            //@jmc: nameLookup takes e.g. (-4,-4) and returns a string "-4/-4" which is the 'node name'
                                                     xj-numNodesX/2)],          //@jmc: the -4/-4 is simply the coordinates of the node (here -5 to 5 = 11 total, for x and y)
                            randomized_input_values[yi][xj]                     //what the node gets set to
                        );
                }
                catch (...)
                {
                    cout << "INVALID LOCATION: " << (yi-numNodesY/2) << ',' << (xj-numNodesX/2) << endl;
                    exit(3);
                }

            }
                    
        }

        #if BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG
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
  */      #endif


        int num_correct = 0;
        double target_value;
        double output_value;

        //(note: there is no real input and output plane. it is the same plane. updating the network changes the inputs to outputs. this must mean that there are links from, say, -4/-4 to itself, which is equivalent to -4/-4 in the input plane to -4/-4 in the output plane 
        //cout << "RUNNING UPDATE ON SUBSTRATE----------------------------------------------------------------------------------------"<< endl;        
        substrate.update();
        //cout << "DONE RUNNING UPDATE ON SUBSTRATE----------------------------------------------------------------------------------------"<< endl;
        
        //jmc: note: only works if input and output planes are of the same size
        if(numNodesX != numNodesY)
        {
          cout << "error: numNodesX must equal numNodesY the way jmc has set this code up." << endl;
          exit(2);
        }

        for (int y2=0;y2<numNodesY;y2++)                                    //go through each coordinate on the output plane and bump num_correct if the input & output are >0, or if input and output are <=0 (any positive weight between each input and output should work)
        {
            for (int x2=0;x2<numNodesX;x2++)
            {

                target_value  = targetOutput(y2,x2, randomized_input_values);                                       //@jmc target value now can either be straight across (GeometricRegularity of 1.0, or a random (but constant through the run) association (GeoReg 0.0)
                                  
                output_value = substrate.getValue(nameLookup[pair<int,int>((y2-numNodesY/2),(x2-numNodesX/2))]);  //@jmc /2 because half are neg and half are pos (cartesian grid centered on 0)

                if (target_value > 0 && output_value > 0)
                {
                    num_correct++;
                }
                else if (target_value <= 0 && output_value <= 0)
                {
                    num_correct++;
                }
            }
        }

        
        return num_correct;
    }

    void BitMirroringV2NoGeomExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
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



        #if BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG
          cout << "in BitMirroringV2NoGeomExperiment.cpp::processIndividual, processing individual:  " << individual << endl;
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

        #if BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG        
            cout << "Individual Evaluation complete!\n";
            cout << "maxFitness: " << maxFitness << endl;
            cout << "fitness: " << fitness << endl;
        #endif
  
        individual->reward(pow(1.1,double(fitness)));
        //individual->reward(pow(1.15,double(fitness))); 1.15 does not blow out the double with 11x11 = 121 answers (1.15^121 < INT_MAX) 
        //individual->reward(fitness); //using a linear fitness function because it hangs indefinitely with the function on the previous line

        if (fitness >= maxFitness)
        {
            #if BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG        
            cout << "PROBLEM DOMAIN SOLVED!!!\n";
            #endif

            //print network and substrate
            printSubstrate(individual);        
            printNetworkCPPN(individual);
        }
        if (fitness < 0)
        {
            cout << "Fitness Less Than Zero!!!, it is: " << fitness << "\n";  
            exit(10);
        }
        
    }

    void BitMirroringV2NoGeomExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
    {
        /*{
            mutex::scoped_lock scoped_lock(*Globals::ioMutex);
            cout << "Starting Evaluation on object:" << this << endl;
            cout << "Running on individual " << individual << endl;
        }*/

        cout << "Sorry, this was never coded up for PostHoc. You'll have to do that now. That is, if you need to. I don't think this code ever gets called. Even doing a post-hoc analysis seems to call processIndividual" << endl;
        exit(6);

        //You get 10 points just for entering the game, wahooo!
        individual->setFitness(10);
        
        populateSubstrate(individual);

        double fitness=0;

        double maxFitness = 0;

        for (int y1=1;y1<numNodesY;y1++)
        {
            for (int x1=1;x1<numNodesX;x1++)
            {
                int y1Big = (y1+numNodesY/2)%numNodesY;
                int x1Big = (x1+numNodesX/2)%numNodesX;

                if (y1Big==0)
                    y1Big++;
                else if (y1Big+1==numNodesY)
                    y1Big--;

                if (x1Big==0)
                    x1Big++;
                else if (x1Big+1==numNodesX)
                    x1Big--;

                if (x1>0&&x1+1<numNodesX)
                {
                    fitness += processEvaluation(individual);
                    maxFitness += 30;
#if BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG
                    cout << "Testing " << x1 << ',' << y1 << " and big " << x1 << ',' << y1Big << "\n";
#endif
                }
                else
                {
#if BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG
                    cout << "Tried to test " << x1 << ',' << y1 << " and big " << x1 << ',' << y1Big << " but big was out of range\n";
#endif
                }

                if (y1>0&&y1+1<numNodesY)
                {
                    fitness += processEvaluation(individual);
                    maxFitness += 30;
#if BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG
                    cout << "Testing " << x1 << ',' << y1 << " and big " << x1Big << ',' << y1 << "\n";
#endif
                }
                else
                {
                #if BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG
                                    cout << "Tried to test " << x1 << ',' << y1 << " and big " << x1Big << ',' << y1 << " but big was out of range\n";
                #endif
                }

                fitness += processEvaluation(individual);
                maxFitness += 30;

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

    void BitMirroringV2NoGeomExperiment::printSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual)
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
    
    void BitMirroringV2NoGeomExperiment::printNetworkCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
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

    Experiment* BitMirroringV2NoGeomExperiment::clone()
    {
        cout << "Experiment cloned\n";
        BitMirroringV2NoGeomExperiment* experiment = new BitMirroringV2NoGeomExperiment(*this);

        cout << this << " cloned to " << experiment << endl;

        return experiment;
    }

    double BitMirroringV2NoGeomExperiment::targetOutput(const int c_y2, const int c_x2, const vector<vector< double> > c_randomized_input_values) 
    {
        int target_output_value;
      
        int corresponding_x1_source = input_to_output_map_y[c_y2][c_x2][0];
        int corresponding_y1_source = input_to_output_map_y[c_y2][c_x2][1];
        target_output_value = (int) c_randomized_input_values[corresponding_y1_source][corresponding_x1_source];
      
      return(target_output_value);
    }

    vector< vector<double> > BitMirroringV2NoGeomExperiment::get_randomized_input_values()
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


    void BitMirroringV2NoGeomExperiment::print_vector_of_vectors(const vector<vector<int> > c_vector_of_vectors){
      cout << endl;
      for(int i=0; i< c_vector_of_vectors.size();i++) {//print rows in reverse order (since x=0 is the lowest)                 
        for(int j=0;j<c_vector_of_vectors[i].size();j++){
          cout << c_vector_of_vectors[i][j] << " ";
        }
        cout << endl;
      }
    }

    void BitMirroringV2NoGeomExperiment::print_vector_of_vectors(const vector<vector<double> > c_vector_of_vectors){
      cout << endl;
      for(int i=0; i< c_vector_of_vectors.size();i++) {//print rows in reverse order (since x=0 is the lowest)                 
        for(int j=0;j<c_vector_of_vectors[i].size();j++){
          cout << c_vector_of_vectors[i][j] << " ";
        }
        cout << endl;
      }
    }

}
