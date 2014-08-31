#include "Experiments/HCUBE_FindClusterExperiment.h"

#define FIND_CLUSTER_EXPERIMENT_DEBUG (0)

#define FIND_CLUSTER_EXPERIMENT_USE_DELTAS (0)

namespace HCUBE
{
    using namespace NEAT;

    FindClusterExperiment::FindClusterExperiment(string _experimentName)
            :
            Experiment(_experimentName),
            numNodesX(int(NEAT::Globals::getSingleton()->getParameterValue("NumberOfNodesX"))),
            numNodesY(numNodesX),
            user1(1,1),
            user2(4,4),
            userDisplayOutputLinkWeights(-1,-1),
            userDisplayOutputs(false),
            clicked1Last(true),
            CELL_SIZE(15),
            sizeMultiplier(1),
	    NUMSHEETS(int(NEAT::Globals::getSingleton()->getParameterValue("NumberOfTotalSheets")))
    {
        generateSubstrate();
    }

    NEAT::GeneticPopulation* FindClusterExperiment::createInitialPopulation(int populationSize)
    {
        NEAT::GeneticPopulation* population = new NEAT::GeneticPopulation();

        vector<GeneticNodeGene> genes;

        genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("X1","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Y1","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("X2","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Y2","NetworkSensor",0,false));
#if FIND_CLUSTER_EXPERIMENT_USE_DELTAS
        genes.push_back(GeneticNodeGene("DeltaX","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("DeltaY","NetworkSensor",0,false));
#endif
		for(int i=1;i<NUMSHEETS;i++)
		{
         genes.push_back(GeneticNodeGene("Output" + i,"NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		}

        for (int a=0;a<populationSize;a++)
        {
            shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,true,1.0));

            for (int b=0;b<0;b++)
            {
                individual->testMutate();
            }

            population->addIndividual(individual);
        }

        return population;
    }

    void FindClusterExperiment::generateSubstrate()
    {
        cout << "Generating substrate...";
        boost::object_pool<NEAT::NetworkNode> networkNodePool;
        boost::object_pool<NEAT::NetworkLink> networkLinkPool;

        NEAT::NetworkNode *nodes = NULL;
        NEAT::NetworkLink *links = NULL;

        try
        {
            nodes = (NEAT::NetworkNode*)malloc(sizeof(NEAT::NetworkNode)*numNodesX*numNodesY*NUMSHEETS);
            links = (NEAT::NetworkLink*)malloc(sizeof(NEAT::NetworkLink)*numNodesX*numNodesY*numNodesX*numNodesY*(NUMSHEETS - 1));
        }
        catch(const std::exception &e)
        {
            cout << e.what() << endl;
            CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
        }

        nameLookup.clear();

        cout << "Creating nodes...\n";

        int nodeCounter=0;

        for (int y1=0;y1<numNodesY;y1++)
        {
            for (int x1=0;x1<numNodesX;x1++)
            {
				for(int z=0;z<NUMSHEETS;z++)
				{

                //cout << (y1-numNodesY/2) << '/' << (x1-numNodesX/2) << endl;
				Node node(y1,x1,z);
                nameLookup[node] = toString(y1)+string("/")+toString(x1)+string("/")+toString(z);
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

		for(int z=1;z<NUMSHEETS;z++)
		{
			for (int y1=0;y1<numNodesY;y1++)
			{
				for (int x1=0;x1<numNodesX;x1++)
				{
					for (int y2=0;y2<numNodesY;y2++)
					{
						for (int x2=0;x2<numNodesX;x2++)
						{
							try
							{
								new (&links[linkCounter]) NetworkLink(&nodes[y1*numNodesX*NUMSHEETS + x1*NUMSHEETS + z-1],&nodes[y2*numNodesX*NUMSHEETS + x2*NUMSHEETS + z],0.0);

								linkCounter++;
							}
							catch (const char *c)
							{
								cout << c << endl;
								CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
							}
							catch (string s)
							{
								cout << s << endl;
								CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
							}
							catch(const std::exception &e)
							{
								cout << e.what() << endl;
								CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
							}
							catch (...)
							{
								cout << "AN EXCEPTION HAS OCCURED!\n";
								CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
							}
						}
					}
				}
			}
        }

        cout << "Creating FastNetwork\n";

        substrate = NEAT::FastNetwork<double>(
                        nodes,
                        numNodesX*numNodesY*NUMSHEETS,
                        links,
                        numNodesX*numNodesY*numNodesX*numNodesY*(NUMSHEETS-1)
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
 
    void FindClusterExperiment::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
       // cout << "Populating substrate...";
        NEAT::FastNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
        //progress_timer t;

        int linkCount=0;

        double x1Val,y1Val,x2Val,y2Val;
		for(int z = 1; z < NUMSHEETS; z++)
		{
			for (int y1=0;y1<numNodesY;y1++)
			{
				for (int x1=0;x1<numNodesX;x1++)
				{
					for (int y2=0;y2<numNodesY;y2++)
					{
						for (int x2=0;x2<numNodesX;x2++)
						{
							Node node1(y1,x1,z-1);
							Node node2(y2,x2,z);

							x1Val = (x1-numNodesX/2)/double((numNodesX-1)/2);
							y1Val = (y1-numNodesY/2)/double((numNodesY-1)/2);
							x2Val = (x2-numNodesX/2)/double((numNodesX-1)/2);
							y2Val = (y2-numNodesY/2)/double((numNodesY-1)/2);

							network.reinitialize();
							network.setValue("X1",x1Val);
							network.setValue("Y1",y1Val);
							network.setValue("X2",x2Val);
							network.setValue("Y2",y2Val);

							if (network.hasNode("DeltaX"))
							{
								network.setValue("DeltaX",x2Val-x1Val);
								network.setValue("DeltaY",y2Val-y1Val);
							}

							network.setValue("Bias",0.3);
							network.update();


							double greyVal = network.getValue("Output" + z);

							//substrate.getLink(counter)->weight = ( greyVal*3.0 );

							/*
							if (greyVal>0.0)
								substrate.getLink(counter)->weight = ( (max(0,greyVal-0.2)/0.8)*3.0 );
							else
								substrate.getLink(counter)->weight = ( (min(0,greyVal+0.2)/0.8)*3.0 );
							*/

							if (fabs(greyVal)>0.2)
							{
								if (greyVal>0.0)
									substrate.getLink(nameLookup[node1],nameLookup[node2])->weight = ( ((greyVal-0.2)/0.8)*3.0 );
								else
									substrate.getLink(nameLookup[node1],nameLookup[node2])->weight = ( ((greyVal+0.2)/0.8)*3.0 );
								linkCount++;
							}
							else
							{
								substrate.getLink(nameLookup[node1],nameLookup[node2])->weight = (0.0);
							}
						}
					}
				}
			}
		}

        //cout << "done!\n";

        //cout << "Number of expressed links: " << linkCount << endl;
    }

    double FindClusterExperiment::processEvaluation(
        shared_ptr<NEAT::GeneticIndividual> individual,
        int x1,
        int y1,
        int x1Big,
        int y1Big
    )
    {
#if FIND_CLUSTER_EXPERIMENT_DEBUG
        cout << "EVALUATION: Testing " << x1 << ',' << y1 << " and big " << x1Big << ',' << y1Big << endl;
        CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
#endif


#if FIND_CLUSTER_EXPERIMENT_DEBUG
        cout << "Initializing substrate\n";
        CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
#endif

        substrate.reinitialize();
        substrate.dummyActivation();

#if FIND_CLUSTER_EXPERIMENT_DEBUG
        cout << "Setting values\n";
        CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
#endif


        int smallRadius;
        int bigRadius;

        double filledValue;

        if (sizeMultiplier==1)
        {
			if(numNodesX == 11)
			{
				smallRadius=0;
				bigRadius=1;
				filledValue=1.0;
			
			}
			else if(numNodesX == 33)
			{
				smallRadius=1;
				bigRadius=4;
				filledValue=1.0;
			
			}
			else if(numNodesX == 55)
			{
				smallRadius=2;
				bigRadius=7;
				filledValue=1.0;
			
			}
			else if(numNodesX == 110)
			{
				smallRadius=5;
				bigRadius=48;
				filledValue=1.0;
			
			}
			else
				throw CREATE_LOCATEDEXCEPTION_INFO("Unsupported size!");

			
        }
        else if (sizeMultiplier==3)
        {
            smallRadius=1;
            bigRadius=4;
            filledValue= (11.0/33.0)*(11.0/33.0);
        }
        else if (sizeMultiplier==5)
        {
            smallRadius=2;
            bigRadius=7;
            filledValue= (11.0/55.0)*(11.0/55.0);
        }
        else if (sizeMultiplier==9)
        {
            smallRadius=4;
            bigRadius=13;
            filledValue= (11.0/66.0)*(11.0/66.0);
        }
        else
        {
            throw CREATE_LOCATEDEXCEPTION_INFO("Unsupported size multiplier!");
        }

        for (int mody=-1*smallRadius;mody<=1*smallRadius;mody++)
            for (int modx=-1*smallRadius;modx<=1*smallRadius;modx++)
            {
                try
                {
                    int tmpy = y1+mody;
                    int tmpx = x1+modx;
                    Node node(tmpy,tmpx,0);

                    substrate.setValue(
                        nameLookup[node],
                        filledValue
                    );
					
                }
                catch (...)
                {
                    cout << "INVALID LOCATION: " << ((y1Big+mody)-numNodesY/2) << ',' << ((x1Big+modx)-numNodesX/2) << endl;
                }

            }

        for (int mody=-1*bigRadius;mody<=1*bigRadius;mody++)
            for (int modx=-1*bigRadius;modx<=1*bigRadius;modx++)
            {
                try
                {
                    substrate.setValue(
                        nameLookup[Node((y1Big+mody),(x1Big+modx),0)],
                        filledValue
                    );
                }
                catch (...)
                {
                    cout << "INVALID LOCATION: " << ((y1Big+mody)-numNodesY/2) << ',' << ((x1Big+modx)-numNodesX/2) << endl;
                }

            }

#if FIND_CLUSTER_EXPERIMENT_DEBUG
        cout << "Updating Substrate\n";
        CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
#endif

        substrate.update(NUMSHEETS-1);

        double largestValue = -INT_MAX;
        double smallestValue = INT_MAX;
        int largesty,largestx;

        for (int y2=0;y2<numNodesY;y2++)
        {
            for (int x2=0;x2<numNodesX;x2++)
            {
                double value = substrate.getValue(nameLookup[Node(y2,x2,NUMSHEETS-1)]);

                if (value > largestValue)
                {
                    largestValue = value;
                    largestx = x2;
                    largesty = y2;
                }
                else if (value==largestValue)
                {
                    //cout << "ERROR: value and largestValue are equal!!!\n";
                    //cout << value << ' ' << largestValue << endl;
                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
                    /*
                    if(
                    max(0,30- ( (x2-x1Big)*(x2-x1Big) + (y2-y1Big)*(y2-y1Big) ) )
                    >
                    max(0,30- ( (largestx-x1Big)*(largestx-x1Big) + (largesty-y1Big)*(largesty-y1Big) ) )
                    )
                    {
                        largestValue = value;
                        largestx = x2;
                        largesty = y2;

                    }
                    */
                }

                if (value<smallestValue)
                {
                    smallestValue=value;
                }

            }
        }


        if (individual->getUserData())
        {
            double x1Normal = (x1-numNodesX/2)/double((numNodesX-1)/2);
            double y1Normal = (y1-numNodesY/2)/double((numNodesY-1)/2);

            double x1BigNormal = (x1Big-numNodesX/2)/double((numNodesX-1)/2);
            double y1BigNormal = (y1Big-numNodesY/2)/double((numNodesY-1)/2);

            double largestxNormal = (largestx-numNodesX/2)/double((numNodesX-1)/2);
            double largestyNormal = (largesty-numNodesY/2)/double((numNodesY-1)/2);

            Vector2<double> baitVec(x1Normal,y1Normal);
            Vector2<double> targetVec(x1BigNormal,y1BigNormal);
            Vector2<double> actualVec(largestxNormal,largestyNormal);
            static_pointer_cast<FindClusterStats>(individual->getUserData())->addStat(
                baitVec,
                targetVec,
                actualVec
            );
        }

        //cout << "Trial Complete: " << x1 << ',' << y1 << " and " << x1Big << ',' << y1Big << ".  Score: "
        //<< max(0,30- ( (largestx-x1Big)*(largestx-x1Big) + (largesty-y1Big)*(largesty-y1Big) ) ) << endl;

        return max(0, ((30/11) * numNodesX )- ( (largestx-x1Big)*(largestx-x1Big) + (largesty-y1Big)*(largesty-y1Big) ) );
    }

    void FindClusterExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
    {
        shared_ptr<NEAT::GeneticIndividual> individual = group.front();
        /*{
            cout << "Starting Evaluation on object:" << this << endl;
            cout << "Running on individual " << individual << endl;
        }*/

        //You get 10 points just for entering the game, wahooo!
        individual->setFitness(10);
        individual->setUserData(shared_ptr<FindClusterStats>(new FindClusterStats()));

        populateSubstrate(individual);

        double fitness=0;

        double maxFitness = 0;

        for (int y1=1;y1<numNodesY;y1+=2)
        {
            for (int x1=1;x1<numNodesX;x1+=2)
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
                    fitness += processEvaluation(individual,x1,y1,x1,y1Big);
                    maxFitness += 30;
#if FIND_CLUSTER_EXPERIMENT_DEBUG
                    cout << "Testing " << x1 << ',' << y1 << " and big " << x1 << ',' << y1Big << "\n";
#endif
                }
                else
                {
#if FIND_CLUSTER_EXPERIMENT_DEBUG
                    cout << "Tried to test " << x1 << ',' << y1 << " and big " << x1 << ',' << y1Big << " but big was out of range\n";
#endif
                }

                if (y1>0&&y1+1<numNodesY)
                {
                    fitness += processEvaluation(individual,x1,y1,x1Big,y1);
                    maxFitness += 30;
#if FIND_CLUSTER_EXPERIMENT_DEBUG
                    cout << "Testing " << x1 << ',' << y1 << " and big " << x1Big << ',' << y1 << "\n";
#endif
                }
                else
                {
#if FIND_CLUSTER_EXPERIMENT_DEBUG
                    cout << "Tried to test " << x1 << ',' << y1 << " and big " << x1Big << ',' << y1 << " but big was out of range\n";
#endif
                }

                fitness += processEvaluation(individual,x1,y1,x1Big,y1Big);
#if FIND_CLUSTER_EXPERIMENT_DEBUG
                cout << "Testing " << x1 << ',' << y1 << " and big " << x1Big << ',' << y1Big << "\n";
#endif
                maxFitness += 30;
#if FIND_CLUSTER_EXPERIMENT_DEBUG
                CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
#endif

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

    void FindClusterExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
    {
        /*{
            cout << "Starting Evaluation on object:" << this << endl;
            cout << "Running on individual " << individual << endl;
        }*/

        //You get 10 points just for entering the game, wahooo!
        individual->setFitness(10);
        individual->setUserData(shared_ptr<FindClusterStats>(new FindClusterStats()));


        populateSubstrate(individual);

        double fitness=0;

        double maxFitness = 0;

        int testCases=0;

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
                            throw CREATE_LOCATEDEXCEPTION_INFO("Unsupported size multiplier!");
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

                        testCases++;

                        fitness += processEvaluation(individual,x1,y1,x1Big,y1Big);

                        maxFitness += 30;

                    }
                }
            }
        }

        cout << "TOTAL TEST CASES: " << testCases << endl;

        //cout << "Individual Evaluation complete!\n";

        //cout << maxFitness << endl;

        individual->reward(fitness);

        if (fitness >= maxFitness*.95)
        {
            cout << "PROBLEM DOMAIN SOLVED!!!\n";
        }
    }


    void FindClusterExperiment::increaseResolution()
    {
        if (sizeMultiplier==1)
        {
            numNodesX *= 3; //((numNodesX-1)*2)+1;
            numNodesY *= 3; //((numNodesY-1)*2)+1;
            sizeMultiplier*=3;
            CELL_SIZE/=3;
            user1.x*=3;
            user1.y*=3;
            user2.x*=3;
            user2.y*=3;
            generateSubstrate();
            if (lastIndividualSeen)
                populateSubstrate(lastIndividualSeen);
        }
        else if (sizeMultiplier==3)
        {
            numNodesX = (numNodesX*5)/3; //((numNodesX-1)*2)+1;
            numNodesY = (numNodesY*5)/3; //((numNodesY-1)*2)+1;
            sizeMultiplier= (sizeMultiplier*5)/3;
            CELL_SIZE =(CELL_SIZE*3)/5;
            user1.x = (user1.x*5)/3;
            user1.y = (user1.y*5)/3;
            user2.x = (user2.x*5)/3;
            user2.y = (user2.y*5)/3;
            generateSubstrate();
            if (lastIndividualSeen)
                populateSubstrate(lastIndividualSeen);
        }
    }

    void FindClusterExperiment::decreaseResolution()
    {
        if (sizeMultiplier==3)
        {
            numNodesX /= 3;//((numNodesX-1)/2)+1;
            numNodesY /= 3;//((numNodesY-1)/2)+1;
            sizeMultiplier/=3;
            CELL_SIZE*=3;
            user1.x/=3;
            user1.y/=3;
            user2.x/=3;
            user2.y/=3;
            generateSubstrate();
            if (lastIndividualSeen)
                populateSubstrate(lastIndividualSeen);
        }
        else if (sizeMultiplier==5)
        {
            numNodesX = (numNodesX*3)/5; //((numNodesX-1)*2)+1;
            numNodesY = (numNodesY*3)/5; //((numNodesY-1)*2)+1;
            sizeMultiplier= (sizeMultiplier*3)/5;
            CELL_SIZE =(CELL_SIZE*5)/3;
            user1.x = (user1.x*3)/5;
            user1.y = (user1.y*3)/5;
            user2.x = (user2.x*3)/5;
            user2.y = (user2.y*3)/5;
            generateSubstrate();
            if (lastIndividualSeen)
                populateSubstrate(lastIndividualSeen);
        }
    }

    Experiment* FindClusterExperiment::clone()
    {
        FindClusterExperiment* experiment = new FindClusterExperiment(*this);

        return experiment;
    }

}
