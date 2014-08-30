#ifndef HCUBE_LEGSWING_PNEAT_H_INCLUDED
#define HCUBE_LEGSWING_PNEAT_H_INCLUDED

#include "Experiments/HCUBE_Experiment.h"
#ifdef COMPILE_ODE
#include "HCUBE_ODE.h"
#endif

//#define LEGSWING_EXPERIMENT_PNEAT_DEBUG


namespace HCUBE
{
    class LegSwingPneatExperiment : public Experiment
    {
    public:
    protected:
			
        NEAT::FastNetwork<double> substrate;

        int numNodesX,numNodesY;

        map<Node,string> nameLookup;
        map<int,string> linkIndexToNameLookup;
			
        double Hybrid_FTMutateOnlyProbability;
        double Hybrid_FTMutateLinkProbability;

    public:
        LegSwingPneatExperiment(string _experimentName);

        virtual ~LegSwingPneatExperiment()
        {}

        virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize);

        virtual NEAT::GeneticPopulation* createInitialPopulation(shared_ptr<NEAT::GeneticPopulation> CPPN_pop, shared_ptr<Experiment>);
			
        virtual void generateSubstrate();

        virtual void populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual);
			
        double getHybrid_FTMutateOnlyProbability() const { return Hybrid_FTMutateOnlyProbability; }
        double getHybrid_FTMutateLinkProbability() const { return Hybrid_FTMutateLinkProbability; }


        double processEvaluation(
            shared_ptr<NEAT::GeneticIndividual> individual,
			bool postHocRecord
        );

        virtual void processGroup(shared_ptr<NEAT::GeneticGeneration> generation);

        virtual void processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual);
      
        double zeroOrNormalize(double mWeightToAlter);

        virtual inline bool performUserEvaluations()
        {
            return false;
        }

        virtual inline bool isDisplayGenerationResult()
        {
            return false;
        }

        virtual Experiment* clone();

        void printSubstrate();
        void printActualSubstrate(  );
        void printCPPN(shared_ptr<const NEAT::GeneticIndividual> individual);
    };

    class LegSwingPneatStats : public NEAT::Stringable
    {
    public:
        vector<Vector2<double> > baitLocation,targetLocation,actualLocation;


        LegSwingPneatStats()
        {
            baitLocation.reserve(1000);
            targetLocation.reserve(1000);
            actualLocation.reserve(1000);
        }

        virtual ~LegSwingPneatStats()
        {}

        void addStat(Vector2<double> &_bait,Vector2<double> &_target,Vector2<double> &_actual)
        {
            baitLocation.push_back(_bait);
            targetLocation.push_back(_target);
            actualLocation.push_back(_actual);
        }

        virtual string toString() const
        {
            std::ostringstream oss;

            for (int a=0;a<(int)targetLocation.size();a++)
            {
                oss << "Bait: " << baitLocation[a].x << ' ' << baitLocation[a].y << ' ';
                oss << "Target: " << targetLocation[a].x << ' ' << targetLocation[a].y << ' ';
                oss << "Actual: "  << actualLocation[a].x << ' ' << actualLocation[a].y;
                oss << " / ";
            }
            return oss.str();
        }

        virtual string toMultiLineString() const
        {
            std::ostringstream oss;

            int numWrong=0;
            double totalDistanceOff=0, distanceOff=0;

            for (int a=0;a<(int)targetLocation.size();a++)
            {
                oss << "Bait: " << baitLocation[a].x << ' ' << baitLocation[a].y << ' ';
                oss << "Target: " << targetLocation[a].x << ' ' << targetLocation[a].y << ' ';
                oss << "Actual: "  << actualLocation[a].x << ' ' << actualLocation[a].y;
                oss << endl;

//                distanceOff =
//                    sqrt(
//                        (actualLocation[a].x-targetLocation[a].x)*(actualLocation[a].x-targetLocation[a].x) +
//                        (actualLocation[a].y-targetLocation[a].y)*(actualLocation[a].y-targetLocation[a].y)
//                    );

                totalDistanceOff += distanceOff;

                if (distanceOff>.6)
                    numWrong++;
            }

            oss << "Total_distance_off: " << totalDistanceOff << endl;
            oss << "Average_distance_off: " << totalDistanceOff/actualLocation.size() << endl;
            oss << "Number_wrong: " << numWrong << endl;
            oss << "Percent_wrong: " << (numWrong*10000)/actualLocation.size()/100.0 << endl;

            return oss.str();
        }

        virtual string summaryHeaderToString() const
        {
            std::ostringstream oss;

            oss << "Total_distance_off: ";
            oss << "Average_distance_off: ";
            oss << "Number_wrong: ";
            oss << "Percent_wrong: ";

            return oss.str();
        }

        virtual string summaryToString() const
        {
            std::ostringstream oss;


//            for (int a=0;a<(int)targetLocation.size();a++)
//            {
//                distanceOff =
//                    sqrt(
//                        (actualLocation[a].x-targetLocation[a].x)*(actualLocation[a].x-targetLocation[a].x) +
//                        (actualLocation[a].y-targetLocation[a].y)*(actualLocation[a].y-targetLocation[a].y)
//                    );
//
//                totalDistanceOff += distanceOff;
//
//                if (distanceOff>.6)
//                    numWrong++;
//            }

//            oss << totalDistanceOff << " ";
//            oss << totalDistanceOff/actualLocation.size() << " "; //these were used in find cluster, but were crashing now that we are in legswing
//            oss << numWrong << " ";
//            oss << (numWrong*10000)/actualLocation.size()/100.0;  //actuallocation and targetlocation vectors should be removed

            return oss.str();
        }

        virtual Stringable *clone() const
        {
            return new LegSwingPneatStats(*this);
        }
    };
}

#endif // HCUBE_LEGSWING_H_INCLUDED
