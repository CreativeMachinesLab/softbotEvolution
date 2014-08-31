#ifndef HCUBE_CHECKERSEXPERIMENTORIGINALFOGEL_H_INCLUDED
#define HCUBE_CHECKERSEXPERIMENTORIGINALFOGEL_H_INCLUDED

#include "Experiments/HCUBE_Experiment.h"
#include "Experiments/HCUBE_CheckersCommon.h"

#include "Experiments/HCUBE_CheckersExperiment.h"

namespace HCUBE
{
    class CheckersExperimentOriginalFogel : public CheckersExperiment
    {
    public:
    protected:

    public:
        CheckersExperimentOriginalFogel(string _experimentName);

        virtual ~CheckersExperimentOriginalFogel()
        {}

        virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize);

        virtual void generateSubstrate(int substrateNum=0);

        virtual void populateSubstrate(
            shared_ptr<const NEAT::GeneticIndividual> individual,
            int substrateNum=0
        );

        virtual CheckersNEATDatatype evaluateLeafHyperNEAT(uchar b[8][8]);

        virtual CheckersNEATDatatype getSpatialInput(uchar b[8][8],int x,int y,int sizex,int sizey);

        virtual Experiment* clone();
    };

}

#endif // HCUBE_CHECKERSEXPERIMENTORIGINALFOGEL_H_INCLUDED
