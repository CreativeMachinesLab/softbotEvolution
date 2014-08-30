#ifndef HCUBE_TARGETWEIGHTS_H_INCLUDED
#define HCUBE_TARGETWEIGHTS_H_INCLUDED

#include "HCUBE_Experiment.h"

#include "HCUBE_Vector2.h"

#include <fstream>

namespace HCUBE
{
    class TargetWeightsExperiment : public Experiment
    {
    public:
    protected:
        NEAT::FastNetwork <double> substrate;
        int numNodesX,numNodesY,numSheets, numNodesPerSheet;

        vector<double> target_weights;                          // the target values for each link the substrate

        map<Node,string> nameLookup;

        int sizeMultiplier;
			int convergence_step;

    public:
        TargetWeightsExperiment(string _experimentName);

        virtual ~TargetWeightsExperiment() {}

        virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize);

        virtual void generateSubstrate();
        virtual void generateTargetWeights();

        int mapXYvalToGridCoord(const int & r_xyVal, const int & r_numNodesXorY);

        double mapXYvalToNormalizedGridCoord(const int & r_xyVal, const int & r_numNodesXorY);
			
			NEAT::FastNetwork<double> populateAndReturnSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual);

        virtual void populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual);

        double processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual);

        virtual void processGroup(shared_ptr<NEAT::GeneticGeneration> generation);
//        virtual void processIndividual(shared_ptr<NEAT::GeneticIndividual> individual);

        virtual void processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual);

        virtual void printSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual);

        virtual void printNetworkCPPN(shared_ptr<const NEAT::GeneticIndividual> individual);

        virtual Experiment* clone();

        void print_vector_of_vectors(const vector<vector<int> > vector_to_print);

        void print_vector_of_vectors(const vector<vector<double> > vector_to_print);
				
			bool converged(int generation);
			
			vector<double> getTargetWeights() { return target_weights; }
    };

}

#endif // HCUBE_TARGETWEIGHTS_H_INCLUDED
