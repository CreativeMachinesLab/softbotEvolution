#ifndef HCUBE_TARGETWEIGHTSNOGEOMEXPERIMENT_H_INCLUDED
#define HCUBE_TARGETWEIGHTSNOGEOMEXPERIMENT_H_INCLUDED

#include "Experiments/HCUBE_Experiment.h"


namespace HCUBE
{
    class TargetWeightsNoGeomExperiment : public Experiment
    {
    public:
    protected:
        NEAT::FastNetwork <double> substrate;

        int numNodesX, numNodesY, numNodesPerSheet;

        vector<double> target_weights;                          // the target values for each link the substrate

        vector< vector< vector<double> > > vector_of_patterns;  // holds the pre-generated possible patterns that can be shown to the networks
        vector< vector <int> > patterns_for_panels;        // which patterns index numbers will be shown to each panel

        map<pair<double,double>,string> nameLookup;

    public:
        TargetWeightsNoGeomExperiment(string _experimentName)
                :
                Experiment(_experimentName),
                numNodesX((int)NEAT::Globals::getSingleton()->getParameterValue("numNodesX")),  //defautl was 11 in 07 gecco papers
                numNodesY((int)NEAT::Globals::getSingleton()->getParameterValue("numNodesY"))
        {
            numNodesPerSheet = numNodesX*numNodesY;
                          
            cout << "Experiment name: TargetWeightsNoGeom\n";
            
            generateSubstrate();
            generateTargetWeights();
        }

        virtual ~TargetWeightsNoGeomExperiment() {}

        virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize);
			virtual NEAT::GeneticPopulation* createInitialPopulation(shared_ptr<NEAT::GeneticPopulation> CPPN_pop, shared_ptr<Experiment> HyperNEAT_experiment);


        virtual void generateSubstrate();
        virtual void generateTargetWeights();

        
        int mapXYvalToGridCoord(const int & r_xyVal, const int & r_numNodesXorY);

        
        virtual void populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual);

        double processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual);

        virtual void processGroup(shared_ptr<NEAT::GeneticGeneration> generation);

        virtual void processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual);
        
        //jmc added functions
        virtual void printSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual);

        virtual void printNetworkCPPN(shared_ptr<const NEAT::GeneticIndividual> individual);

        virtual Experiment* clone();
        
        //jmc added functions
        
        void print_vector_of_vectors(const vector<vector<int> > vector_to_print);

        void print_vector_of_vectors(const vector<vector<double> > vector_to_print);

    };

}


#endif // HCUBE_FINDCLUSTERNOGEOMEXPERIMENT_H_INCLUDED
