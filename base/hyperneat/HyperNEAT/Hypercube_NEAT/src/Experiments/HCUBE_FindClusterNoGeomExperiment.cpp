#include "Experiments/HCUBE_FindClusterNoGeomExperiment.h"
#define FIND_CLUSTER_EXPERIMENT_DEBUG (0)
namespace HCUBE
{
    using namespace NEAT;
    NEAT::GeneticPopulation* FindClusterNoGeomExperiment::createInitialPopulation(int populationSize)
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
                        //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
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
        for (int a=0;a<populationSize;a++)
        {
            //cout << "Creating Individual\n";
            shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,links));
            for (int b=0;b<0;b++)
            {
                individual->testMutate();
            }
            population->addIndividual(individual);
        }
        return population;
    }
    void FindClusterNoGeomExperiment::generateSubstrate()
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
                //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
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
                        //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
                        //cout << "Making link from " << nodes[y1*numNodesX+x1]->getName() << " to " << nodes[y2*numNodesX+x2]->getName() << endl;
                        //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
                        links.push_back(networkLinkPool.construct(nodes[y1*numNodesX+x1],nodes[y2*numNodesX+x2],0.0));
                    }
                }
            }
        }
        substrate = NEAT::FastNetwork<double>(nodes,links);
        substrate.getLink(
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
    void FindClusterNoGeomExperiment::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
        NEAT::FastNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
        double x1Val,y1Val,x2Val,y2Val;
        for (int y1=0;y1<numNodesY;y1++)
        {
            for (int x1=0;x1<numNodesX;x1++)
            {
                for (int y2=0;y2<numNodesY;y2++)
                {
                    for (int x2=0;x2<numNodesX;x2++)
                    {
                        x1Val = (x1-numNodesX/2)/double((numNodesX-1)/2);
                        y1Val = (y1-numNodesY/2)/double((numNodesY-1)/2);
                        x2Val = (x2-numNodesX/2)/double((numNodesX-1)/2);
                        y2Val = (y2-numNodesY/2)/double((numNodesY-1)/2);
                        /*
                        cout << "Getting link from " <<
                        nameLookup[pair<int,int>(y1-numNodesY/2,x1-numNodesX/2)] <<
                        " to " <<
                        nameLookup[pair<int,int>(y2-numNodesY/2,x2-numNodesX/2)] << endl;
                        */
                        substrate.getLink(
                            nameLookup[pair<int,int>(y1-numNodesY/2,x1-numNodesX/2)],
                            nameLookup[pair<int,int>(y2-numNodesY/2,x2-numNodesX/2)]
                        )->weight =
                            network.getLink(
                                nameLookup[pair<int,int>(y1-numNodesY/2,x1-numNodesX/2)],
                                nameLookup[pair<int,int>(y2-numNodesY/2,x2-numNodesX/2)]
                            )->weight;
                    }
                }
            }
        }
    }
    double FindClusterNoGeomExperiment::processEvaluation(
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
        substrate.reinitialize();
        substrate.dummyActivation();

        if (sizeMultiplier==1)
        {
            for (int mody=-1*(sizeMultiplier-1);mody<=1*(sizeMultiplier-1);mody++)
                for (int modx=-1*(sizeMultiplier-1);modx<=1*(sizeMultiplier-1);modx++)
                {
                    substrate.setValue(nameLookup[pair<int,int>((y1+mody)-numNodesY/2,(x1+modx)-numNodesX/2)],1.0);
                }
            for (int mody=-1*sizeMultiplier;mody<=1*sizeMultiplier;mody++)
                for (int modx=-1*sizeMultiplier;modx<=1*sizeMultiplier;modx++)
                {
                    substrate.setValue(nameLookup[pair<int,int>((y1Big+mody)-numNodesY/2,(x1Big+modx)-numNodesX/2)],1.0);
                }
        }
        else
        {
            for (int mody=0;mody<=0;mody++)
                for (int modx=0;modx<=0;modx++)
                {
                    substrate.setValue(nameLookup[pair<int,int>((y1+mody)-numNodesY/2,(x1+modx)-numNodesX/2)],1.0);
                }
            for (int mody=-2;mody<=2;mody++)
                for (int modx=-2;modx<=2;modx++)
                {
                    substrate.setValue(nameLookup[pair<int,int>((y1Big+mody)-numNodesY/2,(x1Big+modx)-numNodesX/2)],1.0);
                }
        }
        substrate.update(1);
        double largestValue = -INT_MAX;
        int largesty,largestx;
        for (int y2=0;y2<numNodesY;y2++)
        {
            for (int x2=0;x2<numNodesX;x2++)
            {
                double value = substrate.getValue(nameLookup[pair<int,int>((y2-numNodesY/2),(x2-numNodesX/2))]);
                if (value > largestValue)
                {
                    largestValue = value;
                    largestx = x2;
                    largesty = y2;
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
            (static_pointer_cast<FindClusterStats>(individual->getUserData()))->addStat(
                baitVec,
                targetVec,
                actualVec
            );
        }
        return max(0,30- ( (largestx-x1Big)*(largestx-x1Big) + (largesty-y1Big)*(largesty-y1Big) ) );
    }
    void FindClusterNoGeomExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
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
    void FindClusterNoGeomExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
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
	
    Experiment* FindClusterNoGeomExperiment::clone()
    {
        cout << "Experiment cloned\n";
        FindClusterNoGeomExperiment* experiment = new FindClusterNoGeomExperiment(*this);
        cout << this << " cloned to " << experiment << endl;
        return experiment;
    }
}
