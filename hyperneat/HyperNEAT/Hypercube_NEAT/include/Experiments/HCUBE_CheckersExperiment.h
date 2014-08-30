#ifndef HCUBE_CHECKERSEXPERIMENT_H_INCLUDED
#define HCUBE_CHECKERSEXPERIMENT_H_INCLUDED

#include "Experiments/HCUBE_Experiment.h"
#include "Experiments/HCUBE_CheckersCommon.h"

#define MAX_CACHED_BOARDS (8192)

#define CHECKERS_EXPERIMENT_ENABLE_BIASES (0)

#define CHECKERS_MAX_ROUNDS (100)

namespace HCUBE
{
    extern int gridToIndex(int x,int y);

    class CheckersExperiment : public Experiment, public CheckersCommon
    {
    public:
    protected:
#if CHECKERS_EXPERIMENT_ENABLE_BIASES
        NEAT::FastBiasNetwork<CheckersNEATDatatype> substrates[2];
#else
        NEAT::FastNetwork<CheckersNEATDatatype> substrates[2];
#endif
        int currentSubstrateIndex;
        const static int handCodedAISubstrateIndex=2;

        shared_ptr<const NEAT::GeneticIndividual> substrateIndividuals[2];

        int numNodesX[3];
        int numNodesY[3];
        //int numGames;
        //CheckersTreeSearch searchTree;

        NodeMap nameLookup;

        CheckersMove moveToMake;

        double chanceToMakeSecondBestMove;

        CheckersNEATDatatype childAlphaForSecondBestMove;
        CheckersNEATDatatype childBetaForSecondBestMove;
        CheckersMove secondBestMoveToMake;

        vector<CheckersMove> totalMoveList;

        uchar userEvaluationBoard[8][8];
        int userEvaluationRound;

        BoardCacheList boardEvaluationCaches[2][65336];

        BoardStateList boardStateLists[3][3][65536];

        Vector2<uchar> from;

        int handCodedType;

        int DEBUG_USE_HANDCODED_EVALUATION;
        int DEBUG_USE_HYPERNEAT_EVALUATION;
        CheckersCachedBoard tmpboard;

        uchar gameLog[1024][8][8];

    public:
        CheckersExperiment(string _experimentName);

        virtual ~CheckersExperiment()
        {}

        virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize);

        inline string getNameFromNode(Node n)
        {
            const string *stringptr = nameLookup.getData(n);
            if (stringptr)
            {
                return string(*stringptr);
            }
            else
            {
                string s = (string("Could not find node: ") + toString(n.x) + string(" ") +
                            toString(n.x) + string(" ") + toString(n.z) + string("\n"));
                throw CREATE_LOCATEDEXCEPTION_INFO(s);
            }
        }

        virtual void generateSubstrate(int substrateNum=0);

        virtual void populateSubstrate(
            shared_ptr<const NEAT::GeneticIndividual> individual,
            int substrateNum=0
        );

        CheckersNEATDatatype processEvaluation(
            wxDC *drawContext
        );

        virtual CheckersNEATDatatype evaluateLeafWhite(uchar b[8][8]);

        virtual CheckersNEATDatatype evaluateLeafHyperNEAT(uchar b[8][8]);

        virtual CheckersNEATDatatype evaluatemax(
            uchar b[8][8],
            CheckersNEATDatatype parentBeta,
            int depth,
            int maxDepth
        );

        virtual CheckersNEATDatatype evaluatemin(
            uchar b[8][8],
            CheckersNEATDatatype parentAlpha,
            int depth,
            int maxDepth
        );

        virtual CheckersNEATDatatype firstevaluatemax(
            uchar b[8][8],
            int maxDepth,
            double timeLimit = double(INT_MAX)
        );

        virtual CheckersNEATDatatype firstevaluatemin(
            uchar b[8][8],
            int maxDepth,
            double timeLimit = double(INT_MAX)
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

        virtual void resetGenerationData(shared_ptr<NEAT::GeneticGeneration> generation);

        virtual void addGenerationData(shared_ptr<NEAT::GeneticGeneration> generation,shared_ptr<NEAT::GeneticIndividual> individual);

        void setChanceToMakeSecondBestMove(double newChance)
        {
            chanceToMakeSecondBestMove = newChance;
        }
    };

}

#endif // HCUBE_TICTACTOEGAMEEXPERIMENT_H_INCLUDED

