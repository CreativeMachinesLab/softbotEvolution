
#ifndef HCUBE_BITMIRRORINGV2_H_INCLUDED
#define HCUBE_BITMIRRORINGV2_H_INCLUDED

#include "HCUBE_Experiment.h"

#include "HCUBE_Vector2.h"

#include <fstream>

#define BIT_MIRRORING_V2_EXPERIMENT_DEBUG (0)
//#define BITMIRROR_HIDDEN_LAYER

//#define HNC_HIGHEST_N
//#define HNC_THRESHOLD
//#define HNC_MANUAL
//#define HNC_RANDOM

//#define HIDDEN_TO_OUTPUT_SOLVED

//#define MANUAL_NORMALIZATION

namespace HCUBE
{

	
	class BitMirroringV2Stats;
	
    class BitMirroringV2Experiment : public Experiment
    {
    public:
    protected:
        NEAT::FastNetwork <double> substrate;
        int numNodesX,numNodesY;
		
        vector< vector< vector <int> > > input_to_output_map_y; // connects an input plane cell to an output plane cell for mirroring problems, the y is because when accessing the y coordinate is left most then the right (e.g. input_to_output_map[y][x])
		
		deque<double> LastNFitnesses;
		
        map<Node,string> nameLookup;
		map<string,Node> nodeLookup;
		
        int sizeMultiplier;
		int convergence_step;
		
		bool terminateOnTargetFitness;
		bool alreadyEndedRun;
		int terminalFitness;
		int terminalAverageN;
		int numSheets;
		
		
		inline int cartesiate(int var, int ref) { return var-(ref/2); }
		
#ifdef MANUAL_NORMALIZATION
		double normalization_cutoff;
#endif
		
#ifdef BITMIRROR_HIDDEN_LAYER
//below are defs for testing hidden node choice algorithms
		
		//enums
		enum HiddenLayerAlgorithm {
			RANDOM,
			MANUAL,
			THRESHOLD,
			HIGHESTN
		};
		
		//variables
		HiddenLayerAlgorithm hiddenAlgorithm; //what algorithm are we using?
				
		int maxHiddenNodes; //variable for each algorithm to choose the number of hidden nodes to use. \
		In Manual and HigestN, it governs the maximum number of hidden nodes allowed to be used
		
		double hiddenNodeThreshold; //variable for the Threshold algorithm
			
		
		//data structures
		map< std::pair<int,int>, bool > HiddenNodeLookup; //all algorithms generate this for populateSubstrate \
		to check on which hidden nodes to use
		
		//list< std::pair<string,double> > HiddenNodeStrengthLookup; //Highest N uses this so it can store the strength of each hidden node, \
		and then sort it to get the highest N nodes used
		
		//map< std::pair<int,int>, int > HiddenNodeLimitLookup;  //Manual uses this for its node choice. For a given maxHiddenNodes, it queries each node's assigned number\
		to decide if it can be included. If the 'limit lookup' <= maxHiddenNodes, then it is included, otherwise it is not.

		
		//methods for choosing hidden nodes. Returns the number of hidden nodes actually used, which is sometimes useful
		int generateHiddenNodeLookup_Random(); 
		
		int generateHiddenNodeLookup_Manual();
				
		int generateHiddenNodeLookup_Threshold(NEAT::FastNetwork<double> &network);
		
		int generateHiddenNodeLookup_HighestN(NEAT::FastNetwork<double> &network);
		
		//debugging method for the hidden Node lookup table
		void printHiddenNodeLookup();
		
		//gets the actual number of hidden nodes turned on by the lookup table
		int getHiddenNodeCount();
		
		
		//records the number of hidden nodes actually used
		int hiddenNodesUsed;
		
		//def for the stats class, for recording statistics like hiddenNodesUsed
		shared_ptr<BitMirroringV2Stats> individualstats;
		
		
		
#endif //BITMIRROR_HIDDEN_LAYER
		
		
    public:
        BitMirroringV2Experiment(string _experimentName);
		
        virtual ~BitMirroringV2Experiment() {}
		
        virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize);
		
		double zeroOrNormalize(double mWeightToAlter);		
		
        virtual void generateSubstrate();
		
        virtual void generateInputToOutputMap();
		
		NEAT::FastNetwork<double> populateAndReturnSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual);
		
        virtual void populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual);
		
        double processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual);
		
        virtual void processGroup(shared_ptr<NEAT::GeneticGeneration> generation);
		
        virtual void processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual);
		
        virtual void printSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual);
		
        virtual void printNetworkCPPN(shared_ptr<const NEAT::GeneticIndividual> individual);
		
        virtual Experiment* clone();
		
        void print_vector_of_vectors(const vector<vector<int> > vector_to_print);
		
        void print_vector_of_vectors(const vector<vector<double> > vector_to_print);
        
        vector< vector<double> > get_randomized_input_values ();
        
        double targetOutput(const int c_x2, const int c_y2, const vector<vector< double> > c_randomized_input_values); 
        
		bool converged(int generation);
		
    };
#ifdef BITMIRROR_HIDDEN_LAYER	
	class BitMirroringV2Stats : public NEAT::Stringable
	{
	public:
		//vector<Vector2<double> > baitLocation,targetLocation,actualLocation;
		
		int HidNodes;
		
		
		BitMirroringV2Stats()
		{
			
		}
		
		virtual ~BitMirroringV2Stats()
		{}
		
		void addStat(int _hid)
		{
			HidNodes = _hid;
		}
		
		virtual string toString() const
		{
			std::stringstream oss;
			oss << HidNodes;
			
			return oss.str();
		}
		
		virtual string summaryHeaderToString() const
		{
			std::ostringstream oss;
			
			oss << "NumHiddenNodes";
			
			return oss.str();
		}
		
		virtual string summaryToString() const
		{
			std::ostringstream oss;
			oss << HidNodes;
			return oss.str();
		}
		
		virtual Stringable *clone() const
		{
			return new BitMirroringV2Stats(*this);
		}
	};
#endif
}





#endif // HCUBE_BITMIRRORINGV2_H_INCLUDED
