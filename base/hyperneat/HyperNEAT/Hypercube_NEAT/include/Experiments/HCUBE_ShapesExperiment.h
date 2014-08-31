#ifndef HCUBE_SHAPES_H_INCLUDED
#define HCUBE_SHAPES_H_INCLUDED

#include "HCUBE_Experiment.h"

#include "HCUBE_Vector2.h"

#include <fstream>
#include "Mesh.h"
#include "Array3D.h"



namespace HCUBE
{
    class ShapesExperiment : public Experiment
    {
    public:
    protected:
		
        int numVoxelsX,numVoxelsY,numVoxelsZ;

        map<Node,string> nameLookup;

        int sizeMultiplier;
		int convergence_step;
		float maxPercentSoFar;
		float voxelExistsThreshold;
		int allTimeHighCounter; 
		CArray3Df TargetContinuousArray;
		CMesh TargetMesh;
		int windowsPerRow;
		int windowsPerCol;

		
		Vec3D WorkSpace; //reasonable workspace (meters)
		double VoxelSize; //Size of voxel in meters (IE the lattice dimension);
		int num_x_voxels;
		int num_y_voxels;
		int num_z_voxels;
		
		
		void generateTarget3DObject();
		int compareEvolvedArrayToTargetArray(CArray3Df &ContinuousArray);  
		double drawShape(bool m_EvolvingInteractively, CMesh& meshToDraw);
		vector <int> drawShapes(vector <CMesh> m_meshesToDraw, int _generationNumber);

    public:
        ShapesExperiment(string _experimentName);

        virtual ~ShapesExperiment() {}

        virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize);

        double mapXYvalToNormalizedGridCoord(const int & r_xyVal, const int & r_numVoxelsXorY);
			
        double processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual);
        void processInteractiveEvaluation(int _genNumber);

        virtual void processGroup(shared_ptr<NEAT::GeneticGeneration> generation);

        virtual void processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual);

        virtual void printNetworkCPPN(shared_ptr<const NEAT::GeneticIndividual> individual);

        virtual Experiment* clone();
				
		bool converged(int generation);

		void writeAndTeeUpParentsOrderFile(); 

			
		

    };

}

#endif // HCUBE_SHAPES_H_INCLUDED
