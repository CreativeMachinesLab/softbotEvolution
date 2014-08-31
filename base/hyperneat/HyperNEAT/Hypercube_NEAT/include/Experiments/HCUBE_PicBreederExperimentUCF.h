#ifndef HCUBE_NOGUI
#ifndef HCUBE_PicBreeder_UCF_H_INCLUDED
#define HCUBE_PicBreeder_UCF_H_INCLUDED


#include "Experiments/HCUBE_Experiment.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "HCUBE_ODE.h"
#include <fstream>

#define PicBreeder_EXPERIMENT_ENABLE_BIASES (0)

//#define PicBreeder_EXPERIMENT_DEBUG (1)

namespace HCUBE
{
	/*
	class PicBreederIndividual
		{
		protected:
			wxBitmap* image;
			shared_ptr<NEAT::GeneticIndividual> genome;
			
		public:
			PicBreeederIndividual()
			{
				image = new wxBitmap();
			}
			
			~PicBreederIndividual()
			{
				delete image;
			}
			
			void RegisterIndividual(shared_ptr<NEAT::GeneticIndividual> individual)
			{
				genome = individual;
			}
		};
			
		*/	
			
	
    class PicBreederExperimentUCF : public Experiment
		{
		public:
		protected:
			vector<vector<long> > substrate;
			unsigned numPixelsX,numPixelsY;
			//int numGames;
			
			map<Node,string> nameLookup;
			
			//vector<PicBreederIndividual*> current_generation;
			
			long int colval;
			wxColour* color;
			wxPen* pen;
		
		protected:
			//long int n2c(double nodeval); //converts a node value (double) to wxColour (long int)
			
		public:
			PicBreederExperimentUCF(string _experimentName);
			
			virtual ~PicBreederExperimentUCF()
			{}
			
			virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize);
			
			void generateSubstrate();
			
			void populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual);
			
			/*inline void setNumGames(int _numGames)
			 {
			 numGames = _numGames;
			 }
			 
			 inline int getNumGames()
			 {
			 return numGames;
			 }*/
			
			double processEvaluation(
									 wxDC *drawContext
									 );
			
			virtual void processGroup(shared_ptr<NEAT::GeneticGeneration> generation);
			
			virtual void processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual);
			
#ifndef HCUBE_NOGUI
			virtual void createIndividualImage(wxDC &drawContext,shared_ptr<NEAT::GeneticIndividual> individual);
			
			virtual bool handleMousePress(wxMouseEvent& event,wxSize &bitmapSize);
#endif
			
			virtual inline bool performUserEvaluations()
			{
				return false;
			}
			
			virtual inline bool isDisplayGenerationResult()
			{
				return false;
			}
			
			virtual Experiment* clone();
			
			void printCPPN(shared_ptr<const NEAT::GeneticIndividual> individual);
			
			virtual void resetGenerationData(shared_ptr<NEAT::GeneticGeneration> generation);
			
			virtual void addGenerationData(shared_ptr<NEAT::GeneticGeneration> generation,shared_ptr<NEAT::GeneticIndividual> individual);
		};
	
	
	class PicBreederStatsUCF : public NEAT::Stringable
    {
    public:
        int wins,losses,ties;
		
        PicBreederStatsUCF()
		:
		wins(0),
		losses(0),
		ties(0)
        {}
		
        PicBreederStatsUCF(
					   int _wins,
					   int _losses,
					   int _ties
					   )
		:
		wins(_wins),
		losses(_losses),
		ties(_ties)
        {}
		
        virtual ~PicBreederStatsUCF()
        {}
		
        virtual string toString() const
        {
            std::ostringstream oss;
            oss << "W:" << wins << " T:" << ties << " L:" << losses;
            return oss.str();
        }
		
        void operator +=(const PicBreederStatsUCF &other)
        {
            wins += other.wins;
            losses += other.losses;
            ties += other.ties;
        }
		
        virtual Stringable *clone() const
        {
            return new PicBreederStatsUCF(wins,losses,ties);
        }
    };
	
}

#endif // HCUBE_PicBreeder_H_INCLUDED
#endif
