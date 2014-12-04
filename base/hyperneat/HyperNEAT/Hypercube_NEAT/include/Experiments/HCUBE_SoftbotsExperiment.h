#ifndef HCUBE_SOFTBOTS_H_INCLUDED
#define HCUBE_SOFTBOTS_H_INCLUDED

#include "HCUBE_Experiment.h"

#include "HCUBE_Vector2.h"

#include <fstream>
#include "Mesh.h"
#include "Array3D.h"



namespace HCUBE
{

	struct Coords
	{
	    float x;
	    float y;
	    float z;

	    Coords(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	    {
	    }

	    bool operator<(const Coords other) const
		{
		    if (z < other.z) 
		    	{return true;}
		    else 
		    {
		    	if (z > other.z)
		    		{return false;}
		    	else
		    	{
		    		if (y < other.y)
		    			{return true;}
		    		else
		    		{
		    			if (y > other.y)
		    				{return false;}
		    			else
		    			{
		    				if (x < other.x)
		    					{return true;}
		    				else
		    					{return false;}
		    			}
		    		}
		    	}
		    }
		}

		bool operator>(const Coords other) const
		{
		    if (z > other.z) 
		    	{return true;}
		    else 
		    {
		    	if (z < other.z)
		    		{return false;}
		    	else
		    	{
		    		if (y > other.y)
		    			{return true;}
		    		else
		    		{
		    			if (y < other.y)
		    				{return false;}
		    			else
		    			{
		    				if (x > other.x)
		    					{return true;}
		    				else
		    					{return false;}
		    			}
		    		}
		    	}
		    }
		}

		bool operator==(const Coords other) const
		{
			if (z == other.z and y == other.y and x == other.x) {return true;}
			else {return false;}
		}

	};

    class SoftbotsExperiment : public Experiment
    {
    public:
    protected:
		
        int numVoxelsX,numVoxelsY,numVoxelsZ;
        double fitness;
        double origFitness;
        double fitness2;
        double bestFitnessThisGen;
        int genNum;


        map<Node,string> nameLookup;

        int sizeMultiplier;
		int convergence_step;
		float voxelExistsThreshold;
		int allTimeHighCounter; 
		CArray3Df TargetContinuousArray;
		CMesh TargetMesh;
		
		Vec3D WorkSpace; //reasonable workspace (meters)
		double voxelSize; //Size of voxel in meters (IE the lattice dimension);
		int num_x_voxels;
		int num_y_voxels;
		int num_z_voxels;

		map<std::string, pair<double, double> > fitnessLookup;

		bool addDistanceFromCenter;
		bool addDistanceFromCenterXY;
		bool addDistanceFromCenterYZ;
		bool addDistanceFromCenterXZ;
		
		
		// void generateTarget3DObject();
		int compareEvolvedArrayToTargetArray(CArray3Df &ContinuousArray);  
		double drawShape(bool m_EvolvingInteractively, CMesh& meshToDraw);
		vector <int> drawShapes(vector <CMesh> m_meshesToDraw, int _generationNumber);

    public:
    	inline void setBestFitnessThisGen(double value) {bestFitnessThisGen = value;}
    	inline double getBestFitnessThisGen() {return bestFitnessThisGen;}

        SoftbotsExperiment(string _experimentName);

        virtual ~SoftbotsExperiment() {}

        virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize);

        double mapXYvalToNormalizedGridCoord(const int & r_xyVal, const int & r_numVoxelsXorY);
			
        double processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual);

        virtual void processGroup(shared_ptr<NEAT::GeneticGeneration> generation);

        virtual void processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual);

        virtual void printNetworkCPPN(shared_ptr<const NEAT::GeneticIndividual> individual);

        virtual Experiment* clone();
				
		bool converged(int generation);

		bool outOfBoundingBox(double x, double y, double z);

		double calculateFitnessAdjustment( vector< vector< vector< int > > > matrixForVoxelyze );

		void moveFitnessFile( shared_ptr<const NEAT::GeneticIndividual> individual );

		void writeVoxelyzeFile( vector< vector< vector< int > > > matrixForVoxelyze );

		int getMaterialAtThisVoxel(NEAT::FastNetwork <double> network, double x, double y, double z);
    };

}

#endif // HCUBE_SHAPES_H_INCLUDED
