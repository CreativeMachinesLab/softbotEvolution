#ifndef HCUBE_MPIEVALUATIONSET_H_INCLUDED
#define HCUBE_MPIEVALUATIONSET_H_INCLUDED

#include "HCUBE_Defines.h"

#include "HCUBE_EvaluationSet.h"

#include "Experiments/HCUBE_Experiment.h"

namespace HCUBE
{
    class MPIEvaluationSet : public EvaluationSet
    {
    public:
    protected:
        int targetProcessor;

    public:
        MPIEvaluationSet(
            shared_ptr<Experiment> _experiment,
            shared_ptr<NEAT::GeneticGeneration> _generation,
            vector<shared_ptr<NEAT::GeneticIndividual> >::iterator _individualIterator,
            int _individualCount,
            int _targetProcessor
        )
                :
                EvaluationSet(
                    _experiment,
                    _generation,
                    _individualIterator,
                    _individualCount
                ),
                targetProcessor(_targetProcessor)
        {}

        virtual ~MPIEvaluationSet()
        {}

        virtual void run();

    protected:
    };
}

#endif // HCUBE_MPIEVALUATIONSET_H_INCLUDED
