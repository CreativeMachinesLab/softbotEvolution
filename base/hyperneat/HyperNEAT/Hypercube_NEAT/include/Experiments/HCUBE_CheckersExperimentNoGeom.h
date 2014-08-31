#ifndef HCUBE_CHECKERSEXPERIMENTNOGEOM_H_INCLUDED
#define HCUBE_CHECKERSEXPERIMENTNOGEOM_H_INCLUDED

#include "Experiments/HCUBE_Experiment.h"
#include "Experiments/HCUBE_CheckersCommon.h"

#include "Experiments/HCUBE_CheckersExperiment.h"

namespace HCUBE
{
    class CheckersExperimentNoGeom : public CheckersExperiment
    {
    public:
    protected:

    public:
        CheckersExperimentNoGeom(string _experimentName);

        virtual ~CheckersExperimentNoGeom()
        {}

        virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize);

        virtual void generateSubstrate(int substrateNum=0);

        virtual void populateSubstrate(
            shared_ptr<const NEAT::GeneticIndividual> individual,
            int substrateNum=0
        );

#if 0
        virtual double evaluateLeafHyperNEAT(uchar b[8][8]);
#endif

        virtual Experiment* clone();
    };

}

#endif // HCUBE_CHECKERSEXPERIMENTNOGEOM_H_INCLUDED
