
#ifndef HCUBE_BITMIRRORINGV2NOGEOMEXPERIMENT_H_INCLUDED
#define HCUBE_BITMIRRORINGV2NOGEOMEXPERIMENT_H_INCLUDED

#include "Experiments/HCUBE_Experiment.h"

#define BIT_MIRRORING_V2_NOGEOM_EXPERIMENT_DEBUG (0)

namespace HCUBE
{
    class BitMirroringV2NoGeomExperiment : public Experiment
    {
    public:
    protected:
        NEAT::FastNetwork <double> substrate;
        int numNodesX,numNodesY;

        vector< vector< vector <int> > > input_to_output_map_y; // connects an input plane cell to an output plane cell for mirroring problems, the y is because when accessing the y coordinate is left most then the right (e.g. input_to_output_map[y][x])

        map<pair<int,int>,string> nameLookup;

        double Hybrid_FTMutateOnlyProbability;
        double Hybrid_FTMutateLinkProbability;

    public:
        BitMirroringV2NoGeomExperiment(string _experimentName)
                :
                Experiment(_experimentName),
                numNodesX((int)NEAT::Globals::getSingleton()->getParameterValue("NumNodesX-Y")),  //defautl was 11 in 07 gecco papers
                numNodesY((int)NEAT::Globals::getSingleton()->getParameterValue("NumNodesX-Y")),
                Hybrid_FTMutateOnlyProbability(-1.0),
                Hybrid_FTMutateLinkProbability(-1.0)

        {
            if(_experimentName == "HYBRID") {
                Hybrid_FTMutateOnlyProbability = NEAT::Globals::getSingleton()->getParameterValue("Hybrid-FT-MutateOnlyProbability");
                Hybrid_FTMutateLinkProbability =	NEAT::Globals::getSingleton()->getParameterValue("Hybrid-FT-MutateLinkProbability");
            }
            generateSubstrate();
            generateInputToOutputMap();
        }

        virtual ~BitMirroringV2NoGeomExperiment() {}

        virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize);
			
			virtual NEAT::GeneticPopulation* createInitialPopulation(shared_ptr<NEAT::GeneticPopulation> CPPN_pop, shared_ptr<Experiment> HyperNEAT_experiment);

        virtual void generateSubstrate();
        
        virtual void generateInputToOutputMap();

        virtual void populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual);

        double getHybrid_FTMutateOnlyProbability() const { return Hybrid_FTMutateOnlyProbability; }
        double getHybrid_FTMutateLinkProbability() const { return Hybrid_FTMutateLinkProbability; }

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

        vector< vector<double> > get_randomized_input_values ();

        double targetOutput(const int c_x2, const int c_y2, const vector<vector< double> > c_randomized_input_values); 

    };

}


#endif // HCUBE_FINDCLUSTERNOGEOMEXPERIMENT_H_INCLUDED
