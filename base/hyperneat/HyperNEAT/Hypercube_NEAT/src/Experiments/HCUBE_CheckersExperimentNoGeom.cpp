#include "Experiments/HCUBE_CheckersExperimentNoGeom.h"

#include "Experiments/HCUBE_cliche.h"
#include "Experiments/HCUBE_simplech.h"

#define CHECKERS_EXPERIMENT_DEBUG (0)

namespace HCUBE
{
    using namespace NEAT;

    CheckersExperimentNoGeom::CheckersExperimentNoGeom(string _experimentName)
            :
            CheckersExperiment(_experimentName)
    {
        generateSubstrate();
    }

    GeneticPopulation* CheckersExperimentNoGeom::createInitialPopulation(int populationSize)
    {
        GeneticPopulation *population = new GeneticPopulation();
        vector<GeneticNodeGene> genes;

        genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));

        for (int x=0;x<8;x++)
        {
            for (int y=0;y<8;y++)
            {
                if ((x+y)%2==0)
                {
                    Node node(x,y,0);
                    //cout << (y1-numNodesY/2) << '/' << (x1-numNodesX/2) << endl;
                    string name = (toString(x)+string("/")+toString(y) + string("/") + toString(0));
                    nameLookup.insert(node,name);
                    genes.push_back(GeneticNodeGene(name,"NetworkSensor",0,false));
                }
            }
        }

        Node node(0,0,2);
        //cout << (y1-numNodesY/2) << '/' << (x1-numNodesX/2) << endl;
        string name = (toString(0)+string("/")+toString(0) + string("/") + toString(2));
        nameLookup.insert(node,name);
        genes.push_back(GeneticNodeGene(name,"NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));

        for (int a=0;a<populationSize;a++)
        {
            shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,true,1.0));

            for (int b=0;b<0;b++)
            {
                individual->testMutate();
            }

            population->addIndividual(individual);
        }

        cout << "Finished creating population\n";
        return population;
    }

    void CheckersExperimentNoGeom::generateSubstrate(int substrateNum)
    {}

    void CheckersExperimentNoGeom::populateSubstrate(
        shared_ptr<const NEAT::GeneticIndividual> individual,
        int substrateNum
    )
    {
        if (substrateIndividuals[substrateNum]==individual)
        {
            //Don't bother remaking the same substrate
            return;
        }

        substrateIndividuals[substrateNum]=individual;

        //Clear the evaluation cache
        for (int a=0;a<65536;a++)
        {
            boardEvaluationCaches[substrateNum][a].clear();
        }

        //Clear the state list caches
        for (int a=0;a<65536;a++)
        {
            boardStateLists[substrateNum][BLACK][a].clear();
        }
        for (int a=0;a<65536;a++)
        {
            boardStateLists[substrateNum][WHITE][a].clear();
        }

        substrates[substrateNum] = individual->spawnFastPhenotypeStack<CheckersNEATDatatype>();
    }

    Experiment* CheckersExperimentNoGeom::clone()
    {
        CheckersExperimentNoGeom* experiment = new CheckersExperimentNoGeom(*this);

        return experiment;
    }
}
