#ifndef HCUBE_EXPERIMENTRUN_H_INCLUDED
#define HCUBE_EXPERIMENTRUN_H_INCLUDED



#include "HCUBE_Defines.h"

namespace HCUBE
{
    class ExperimentRun
    {
    public:
    protected:
        bool running;
        bool started;
        bool cleanup;
        int experimentType;
				int currentSubExperiment;
				int totalSubExperiments;
						
        shared_ptr<NEAT::GeneticPopulation> population;

        vector<shared_ptr<Experiment> > experiments;

        mutex* populationMutex;

        MainFrame *frame;

        string outputFileName;
				
    public:
        ExperimentRun();

        virtual ~ExperimentRun();

        inline void setCleanup(bool value)
        {
            cleanup = value;
        }

        inline shared_ptr<NEAT::GeneticPopulation> getPopulation()
        {
            return population;
        }

        inline shared_ptr<Experiment> getExperiment()
        {
            return experiments[currentSubExperiment];
        }

        inline void setFrame(MainFrame *_frame)
        {
            frame=_frame;
        }

        inline const bool &isRunning()
        {
            return running;
        }

        inline void setRunning(bool newRunning)
        {
            if (!population)
            {
                throw CREATE_LOCATEDEXCEPTION_INFO("Tried to start an experiment with no population!");
            }

            running = newRunning;
        }

        inline const bool &isStarted()
        {
            return started;
        }

        inline shared_ptr<NEAT::GeneticGeneration> getGeneration(int generation)
        {
            return population->getGeneration(generation);
        }

        inline shared_ptr<NEAT::GeneticIndividual> getIndividual(int generation,int individual)
        {
            return population->getIndividual(individual,generation);
        }

        inline shared_ptr<NEAT::GeneticIndividual> getIndividual(int individual)
        {
            return population->getIndividual(individual);
        }

        void setupExperimentInProgress(
            string populationFileName,
            string _outputFileName
        );
		
		void setupExperimentInProgressUseDat(
															string _datFile, 
															string populationFileName,
															string _outputFileName
															);

        void start();

        void setupExperiment(
            int experimentType,
            string _outputFileName
        );

        void createPopulation(string populationString="");

        virtual void evaluatePopulation();

        void finishEvaluations();

        void produceNextGeneration(); 
        void experimentRunPrintToGenChampFile();


        inline mutex* getPopulationMutex()
        {
            return populationMutex;
        }

    protected:
			bool switchSubExperiment(int);

    };
}

#endif // HCUBE_EXPERIMENTRUN_H_INCLUDED
