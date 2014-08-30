#ifndef HCUBE_EVALUATIONSET_H_INCLUDED
#define HCUBE_EVALUATIONSET_H_INCLUDED

#include "HCUBE_Defines.h"

#include "Experiments/HCUBE_Experiment.h"

namespace HCUBE
{
    class EvaluationSet
    {
    public:
    protected:
        bool running;
        shared_ptr<Experiment> experiment;
        shared_ptr<NEAT::GeneticGeneration> generation;
        vector<shared_ptr<NEAT::GeneticIndividual> >::iterator individualIterator;
        int individualCount;
        bool finished;

    public:
        EvaluationSet(
            shared_ptr<Experiment> _experiment,
            shared_ptr<NEAT::GeneticGeneration> _generation,
            vector<shared_ptr<NEAT::GeneticIndividual> >::iterator _individualIterator,
            int _individualCount
        )
                :
                running(false),
                experiment(_experiment),
                generation(_generation),
                individualIterator(_individualIterator),
                individualCount(_individualCount),
                finished(false)
        {}

        virtual ~EvaluationSet()
        {}

        virtual void run();

        bool isFinished()
        {
            return finished;
        }

        bool getRunning()
        {
            return running;
        }

        void setRunning(bool _running)
        {
            running = _running;
        }
    protected:
    };
}

#endif // HCUBE_EVALUATIONSET_H_INCLUDED
