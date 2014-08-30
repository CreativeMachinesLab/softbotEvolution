#include <iomanip> 
#include "Experiments/HCUBE_ShapesExperiment.h"
#include <stdlib.h>
#include "MarchCube.h"
#include <time.h>
//#include "DigitalMatter.h"
//#include "DM_FEA.h" 
#include <stdio.h>
#include <iostream>
#include <fstream>
//ACHEN: Included for temporary hack


#ifdef VISUALIZESHAPES
//#include <SFML/Graphics.hpp>
#endif
#include <sstream>

float viewThresh = 100000.0;	// arbitrary high number for the amount of time an interactive eval can take

//JMC TODO Parameterize these and put them in the constructor
bool addDistanceFromCenter = false;
bool addDistanceFromCenterXY = false;
bool addDistanceFromCenterYZ = false;
bool addDistanceFromCenterXZ = false;
bool addDistanceFromShell = true; //ACHEN: Shell evolve
bool addInsideOrOutside = false; //ACHEN: Shell evolve
bool useColor = false;
bool addArcTan2 = false;
int windowWidth = 1600;
int windowHeight = 1200;
bool firstGen = true; //necessary for numbering .vf files after restart, because otherwise the first generation no. is high and then starts at 1
bool firstGenSeedingSoDontPrintParentsFile = false;

#define SHAPES_EXPERIMENT_DEBUG (0)


void
initGL(void)
{
#ifdef VISUALIZESHAPES
/*
//  all of these don't seem to change things for me, but Hiller had them in his code. Some may help performance?
//	glFrontFace(GL_CCW);
//	glCullFace(GL_BACK);

//	glDepthFunc(GL_LEQUAL);
//	glEnable(GL_NORMALIZE);
//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_POLYGON_SMOOTH);
//	
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	
	glEnable(GL_CULL_FACE);//add to boost performance, if it doesn't mess things up
	if(useColor) glEnable(GL_COLOR_MATERIAL);
	
	// Enable a single OpenGL light. 
//	GLfloat light_diffuse[] = {0.5, 0.5, 0.5, 1.0};  //gray diffuse light, for use with color 
	GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  // Red diffuse light. for use without color 

	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  // Infinite light location. 

	//glLightfv(GL_LIGHT0, GL_EMISSION, light_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_diffuse);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	
	
	//will be needed once I want transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Use depth buffering for hidden surface elimination. 
	glEnable(GL_DEPTH_TEST);
	
	//Set up the view 
	glMatrixMode(GL_PROJECTION);
	
	gluPerspective( // field of view in degree  40.0,
				   // aspect ratio  1.0,
				   // Z near
				  0.001, // Z far  
				10000.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0, 0, 50.0,  // eye is at (0,0,10) 
			  0.0, 0.0, 0.0,      // center is at (0,0,0) 
			  0.0, 1.0, 0.0);      // up is in positive Y direction 
	//original
//	gluLookAt(0, 0, 100.0,  // eye is at (0,0,10) 
//			  0.0, 0.0, 0.0,      // center is at (0,0,0) 
//			  0.0, 1.0, 0.);      // up is in positive Y direction 
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);	//sets background color to white
	*/
#endif //ends #ifdef VISUALIZESHAPES
	
}




namespace HCUBE
{
    using namespace NEAT;

    ShapesExperiment::ShapesExperiment(string _experimentName)
    :   Experiment(_experimentName)
    {
        
        cout << "Constructing experiment named ShapesExperiment.\n";
		
		windowsPerRow = 5;
		windowsPerCol = 3;

		if(int(NEAT::Globals::getSingleton()->getParameterValue("PopulationSize")) > 15)
	   {
		   cout << "The code currently only supports visualizing up to 15 shapes.\n"; 
		   exit(5);			   
	   }
		maxPercentSoFar = 0.0;
		allTimeHighCounter = 0;
			
		convergence_step = int(NEAT::Globals::getSingleton()->getParameterValue("ConvergenceStep"));
		if(convergence_step < 0)
			convergence_step = INT_MAX; // never switch

		// initialize size/granularity of workspace 
		WorkSpace = Vec3D(0.01, 0.02, 0.01); //reasonable workspace (meters)
//		VoxelSize = 0.005; //Size of voxel in meters (IE the lattice dimension); (original)
		VoxelSize = 0.001; //Size of voxel in meters (IE the lattice dimension); (what all the paper screen shots used)
//		VoxelSize = 0.0005; //Size of voxel in meters (IE the lattice dimension); (original)
		voxelExistsThreshold = 0.0;//@# MIGHT CHANGE ACHEN
		num_x_voxels = int(WorkSpace.x/VoxelSize);
		num_y_voxels = int(WorkSpace.y/VoxelSize);
		num_z_voxels = int(WorkSpace.z/VoxelSize);

		#ifndef INTERACTIVELYEVOLVINGSHAPES		
		TargetContinuousArray = CArray3Df(num_x_voxels, num_y_voxels, num_z_voxels);	//because calling the consrucor on TargetContinuousArray only initialized it locally (!?!)
		generateTarget3DObject(); // fill array with target shape
		#endif
        
    }

    NEAT::GeneticPopulation* ShapesExperiment::createInitialPopulation(int populationSize)
    {
        NEAT::GeneticPopulation* population = new NEAT::GeneticPopulation();

        vector<GeneticNodeGene> genes;
	cout << "JMC HERE" << endl;

        genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("x","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("y","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("z","NetworkSensor",0,false));
		if(addDistanceFromCenter)	genes.push_back(GeneticNodeGene("d","NetworkSensor",0,false));
		if(addDistanceFromCenterXY) genes.push_back(GeneticNodeGene("dxy","NetworkSensor",0,false));
		if(addDistanceFromCenterYZ) genes.push_back(GeneticNodeGene("dyz","NetworkSensor",0,false));
		if(addDistanceFromCenterXZ) genes.push_back(GeneticNodeGene("dxz","NetworkSensor",0,false));
		if(addArcTan2) genes.push_back(GeneticNodeGene("arcTan2","NetworkSensor",0,false));
		if(addDistanceFromShell) genes.push_back(GeneticNodeGene("ds","NetworkSensor",0,false)); //ACHEN ADDED
		if(addInsideOrOutside) genes.push_back(GeneticNodeGene("inout","NetworkSensor",0,false)); //ACHEN ADDED
		genes.push_back(GeneticNodeGene("Output","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		if(useColor){
			genes.push_back(GeneticNodeGene("Output-colorR","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
			genes.push_back(GeneticNodeGene("Output-colorG","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
			genes.push_back(GeneticNodeGene("Output-colorB","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		}
        for (size_t a=0;a<populationSize;a++)
        {
            shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,true,1.0));

            //for (size_t b=0;b<0;b++)  // this was uncommented in jason's original code, but I think since he has b<0 it never fired. setting it to >0 would probably testMutate it
            //{            
            //    individual->testMutate();
            //}

            population->addIndividual(individual);
        }

        return population;
    }


    double ShapesExperiment::mapXYvalToNormalizedGridCoord(const int & r_xyVal, const int & r_numVoxelsXorY) 
    {
        // turn the xth or yth node into its coordinates on a grid from -1 to 1, e.g. x values (1,2,3,4,5) become (-1, -.5 , 0, .5, 1)
        // this works with even numbers, and for x or y grids only 1 wide/tall, which was not the case for the original
        // e.g. see findCluster for the orignal versions where it was not a funciton and did not work with odd or 1 tall/wide #s
		
        double coord;
                
        if(r_numVoxelsXorY==1) coord = 0;
        else                  coord = -1 + ( r_xyVal * 2.0/(r_numVoxelsXorY-1) );

        return(coord);    
    }
	

    double ShapesExperiment::processEvaluation(shared_ptr<NEAT::GeneticIndividual> individual)
    {
        #if SHAPES_EXPERIMENT_DEBUG
        cout << "JMC: evaluating fitness in ShapesExperiment::processEvaluation\n";
        #endif

		bool EvolvingTarget = true;
		bool EvolvingInteractive;
		
		if(EvolvingTarget == true) {EvolvingInteractive = false; viewThresh = 0.5;}
		else {					   EvolvingInteractive = true; }

		
		float InteractiveFitnessScore = 0.0; //since we raise to the power of two, it can be zero
		
		
		//initializes continuous space array with zeros. +1 is because we need to sample
		// these points at all corners of each voxel, leading to n+1 points in any dimension
		CArray3Df ContinuousArray(num_x_voxels, num_y_voxels, num_z_voxels); //evolved array

		//TODO; If you end up not using color, then pound define these out when not using them
		CArray3Df rColorArray(num_x_voxels, num_y_voxels, num_z_voxels); //evolved array of colors for each voxel
		CArray3Df gColorArray(num_x_voxels, num_y_voxels, num_z_voxels); //evolved array of colors for each voxel
		CArray3Df bColorArray(num_x_voxels, num_y_voxels, num_z_voxels); //evolved array of colors for each voxel
		
		NEAT::FastNetwork <double> network = individual->spawnFastPhenotypeStack<double>();        //JMC: this is the CPPN network

		int px, py, pz; //temporary variable to store locations as we iterate
		float xNormalized;
		float yNormalized;
		float zNormalized;
		float distanceFromCenter;
		float distanceFromCenterXY;
		float distanceFromCenterYZ;
		float distanceFromCenterXZ;
		float distanceFromShell;
		float insideOutside;
		float arcTan2Val;
		int ai;
		/**
		//START HACK PREPARATIONS
		string line;
		ifstream file("/home/achen/csv/pure.csv"); //FILE NAME
		float store [2000];
		
		int i= 0;
		float e= 0;

		while (getline(file,line)){
		  store[i]= atof(line.c_str());
		  i++;
		}

		file.close();
		**/		
		for (int j=0; j<ContinuousArray.GetFullSize(); j++)  //iterate through each location of the continuous matrix
		{ 
			ContinuousArray.GetXYZ(&px, &py, &pz, j); //gets XYZ location of this element in the array
			xNormalized = mapXYvalToNormalizedGridCoord(px, num_x_voxels);
			yNormalized = mapXYvalToNormalizedGridCoord(py, num_y_voxels);
			zNormalized = mapXYvalToNormalizedGridCoord(pz, num_z_voxels);
			
			//Array hack
			//e= store[i];
			//i++;

    			//calculate input vars
			if(addDistanceFromCenter)   distanceFromCenter   = sqrt(pow(double(xNormalized),2.0)+pow(double(yNormalized),2.0)+pow(double(zNormalized),2.0));			
			if(addDistanceFromCenterXY) distanceFromCenterXY = sqrt(pow(double(xNormalized),2.0)+pow(double(yNormalized),2.0));
			if(addDistanceFromCenterYZ) distanceFromCenterYZ = sqrt(pow(double(yNormalized),2.0)+pow(double(zNormalized),2.0));
			if(addDistanceFromCenterXZ) distanceFromCenterXZ = sqrt(pow(double(xNormalized),2.0)+pow(double(zNormalized),2.0));				
			if(addArcTan2) arcTan2Val = atan2(yNormalized, zNormalized);
			
			network.reinitialize();								//reset CPPN
			network.setValue("x",xNormalized);					//set the input numbers
			network.setValue("y",yNormalized);
			network.setValue("z",zNormalized);
			if(addDistanceFromCenter) network.setValue("d",distanceFromCenter);
			if(addDistanceFromCenterXY) network.setValue("dxy",distanceFromCenterXY);
			if(addDistanceFromCenterYZ) network.setValue("dyz",distanceFromCenterYZ);
			if(addDistanceFromCenterXZ) network.setValue("dxz",distanceFromCenterXZ);
			//if(addDistanceFromShell) network.setValue("ds",e); //ACHEN: Shell distance
			//if(addInsideOrOutside) network.setValue("inout",e);
			if(addArcTan2) network.setValue("arcTan2",arcTan2Val);
			network.setValue("Bias",0.3);                       
							
			network.update();                                   //JMC: on this line we run the CPPN network...  
			
			ContinuousArray[j] = network.getValue("Output");        //JMC: and here we get the CPPN output (which is the weight of the connection between the two)
			if(useColor){
				rColorArray[j]     = network.getValue("Output-colorR");
				gColorArray[j]     = network.getValue("Output-colorG");
				bColorArray[j]     = network.getValue("Output-colorB");
			}
			

		}

		int numberSameVoxels = compareEvolvedArrayToTargetArray(ContinuousArray);
		double percentSameVoxels = double(numberSameVoxels)/double(ContinuousArray.GetFullSize());
		
		
		PRINT(percentSameVoxels);
		
		
		//std::cout << "Performing marching cubes...\n";
		CMesh OutputMesh;
		CMarchCube::SingleMaterial(&OutputMesh, &ContinuousArray, voxelExistsThreshold);	//jmc: last argument is the threshold above which we consider the voxel to be extant

		//save stl file for new all-time highs
		if (percentSameVoxels - maxPercentSoFar > 0.001)
		{	
			printf("++++++++++++++++++++++++++++++++ New all time high! PercentSameVoxelx = %f\n", percentSameVoxels);
			maxPercentSoFar = percentSameVoxels;
			allTimeHighCounter++;

			string suffix = ".stl";
			string allTimeNumAsString;
			string percentSameVoxelsAsFloat; 

			//convert allTimeHigh to string
			std::stringstream tempVar;
			std::stringstream tempVarPercent;
			tempVar << allTimeHighCounter;
			allTimeNumAsString = tempVar.str();
			tempVarPercent << percentSameVoxels;
			percentSameVoxelsAsFloat = tempVarPercent.str();
			
			string filename = "ChamNum" + allTimeNumAsString + "-Fit_" + percentSameVoxelsAsFloat + suffix;
			cout << "filename is " << filename << endl;
			
				std::cout << "Saving file - " << filename << endl;
		}


		//Draw function goes here
		#ifdef VISUALIZESHAPES
		//InteractiveFitnessScore = drawShape(EvolvingInteractive, OutputMesh); //InteractiveFitnessScore ignored in target based evolution
		#endif
		
		double fitness = 0;
		if(EvolvingInteractive){ fitness = double(InteractiveFitnessScore); PRINT(InteractiveFitnessScore);}  
		else if (EvolvingTarget) fitness = double(percentSameVoxels); 
		else {cout << "error: what is your fitness function?" << endl; exit(-1);}
		
		double exponentiatedFitness = pow(2000,double(fitness));
		assert(exponentiatedFitness >0);						//no fitness scores of zero allowed

		return exponentiatedFitness; 

    }

    void ShapesExperiment::processInteractiveEvaluation(int _genNumber)
    {
		vector <int> fitnessScores;
		writeAndTeeUpParentsOrderFile();

		//if the command line is telling us which orgs to select, handle that and return (skip the rest of the function)
		if(int(NEAT::Globals::getSingleton()->getParameterValue("NeedToInjectFitnessValuesFromCommandLine")))
		{
			
			NEAT::Globals::getSingleton()->setParameterValue("NeedToInjectFitnessValuesFromCommandLine", 0.0); //set flag so we don't enter this condition the next time around
			
			if(int(NEAT::Globals::getSingleton()->getParameterValue("SeedingSoDecrementByOne"))) firstGenSeedingSoDontPrintParentsFile = true;

			
			//initialize default (non-selected) fitness values
			for(int z=0;z< group.size();z++)
			{
				fitnessScores.push_back(1);
			}
			
			if(int(NEAT::Globals::getSingleton()->getParameterValue("StartVfFileIndexesAtOne")))
			  {
			    firstGen = false; //don't make the gen num zero if branching
			  }

			
			//get vector that lists the orgs we need to mark as selected (variable sized)
			vector <int> listOfOrgsInjectedAsSelected = NEAT::Globals::getSingleton()->getInjectOrgsSelected();
			if (!listOfOrgsInjectedAsSelected.size()>=1)
			{
				cout << "You must provide a list of orgs to mark as selected with the -F flag" << endl;
			}
			
			//mark those orgs as selected
			for(int z=0;z<listOfOrgsInjectedAsSelected.size();z++)
			{
				PRINT(listOfOrgsInjectedAsSelected[z]);
				fitnessScores[(listOfOrgsInjectedAsSelected[z])] = 1000;
			}

			cout << "result of command line arguments being used to make initial selection" << endl;
			for(int z=0;z< group.size();z++)
			{
				PRINT(fitnessScores[z]);
				shared_ptr<NEAT::GeneticIndividual> individual = group[z];
				if (fitnessScores[z]>	5) individual->reward(fitnessScores[z]);	//if picked by user (either as champ or tie-for-second, use those scores
				else individual->reward(1);	 
			}
			return;
		}
		
		
//		writeAndTeeUpParentsOrderFile(); //delme if you moved this up above the one-time for inserted fitness and like it...this was it's original position

		viewThresh = 100000;	//how long the user has to input their champ
		vector <CMesh> meshesToDraw;
		
		NEAT::FastNetwork <double> network;

		//initializes continuous space array with zeros. +1 is because we need to sample
		// these points at all corners of each voxel, leading to n+1 points in any dimension
		CArray3Df ContinuousArray(num_x_voxels, num_y_voxels, num_z_voxels); //evolved array
		CArray3Df rColorArray(num_x_voxels, num_y_voxels, num_z_voxels); //evolved array of red color for each voxel
		CArray3Df gColorArray(num_x_voxels, num_y_voxels, num_z_voxels); //evolved array of green color for each voxel
		CArray3Df bColorArray(num_x_voxels, num_y_voxels, num_z_voxels); //evolved array of blue color for each voxel
		

		int px, py, pz; //temporary variable to store locations as we iterate
		float xNormalized;
		float yNormalized;
		float zNormalized;
		float distanceFromCenter = 0.0;
		float distanceFromCenterXY = 0.0;
		float distanceFromCenterYZ = 0.0;
		float distanceFromCenterXZ = 0.0;
		float arcTan2Val = 0.0;
		float distanceFromShell = 0.0;
		float insideOutside = 0.0;
		
		int ai;
		float e;

		//START HACK v.2
		string line;
		ifstream file("/home/achen/csv/pure.csv"); //FILE NAME
		float store2 [2000];
		
		int i= 0;
		
		while (getline(file,line)){
		  store2[i]= atof(line.c_str());
		  i++;
		}

		file.close();
		i= 0;
		
		int voxelCount=0;
		shared_ptr<NEAT::GeneticIndividual> individual;
		
		for(int z=0;z< group.size();z++)
		{
			cout << "generating shape: " << z << endl;
			individual = group[z];									
			network = individual->spawnFastPhenotypeStack<double>();        //JMC: this is the CPPN network
			
			voxelCount=0;
				
			cout << "JMC INTERACTIVE HACKING" << endl;
			for (int j=0; j<ContinuousArray.GetFullSize(); j++)  //iterate through each location of the continuous matrix
			{ 
				ContinuousArray.GetXYZ(&px, &py, &pz, j); //gets XYZ location of this element in the array
				xNormalized = mapXYvalToNormalizedGridCoord(px, num_x_voxels);
				yNormalized = mapXYvalToNormalizedGridCoord(py, num_y_voxels);
				zNormalized = mapXYvalToNormalizedGridCoord(pz, num_z_voxels);

				cout << px << endl;
            
            //ACHEN: Will change variable name...
				e= store2[i];
				i++;

				//ACHEN: LOGGING POSITIONS
				//cout << "(" << xNormalized << "," << yNormalized << "," << zNormalized << ")" << endl; 

				//calculate input vars
				if(addDistanceFromCenter)   distanceFromCenter   = sqrt(pow(double(xNormalized),2.0)+pow(double(yNormalized),2.0)+pow(double(zNormalized),2.0));
				if(addDistanceFromCenterXY) distanceFromCenterXY = sqrt(pow(double(xNormalized),2.0)+pow(double(yNormalized),2.0));
				if(addDistanceFromCenterYZ) distanceFromCenterYZ = sqrt(pow(double(yNormalized),2.0)+pow(double(zNormalized),2.0));
				if(addDistanceFromCenterXZ) distanceFromCenterXZ = sqrt(pow(double(xNormalized),2.0)+pow(double(zNormalized),2.0));				
				if(addArcTan2) arcTan2Val = atan2(yNormalized, zNormalized);
												
				network.reinitialize();								//reset CPPN
				//next three lines are zeroed out for shell debugging
				//cout << "JMC HACKING VALUES TO ZERO" << endl;
				
				//network.setValue("x",0);					//set the input numbers
				//network.setValue("y",0);
				//network.setValue("z",0);

				//network.setValue("x",xNormalized);					//set the input numbers
			  //network.setValue("y",yNormalized);
				//network.setValue("z",zNormalized);
				if(addDistanceFromShell) network.setValue("ds",e); //ACHEN: Shell distance
				if(addDistanceFromCenter) network.setValue("d",distanceFromCenter);
				if(addDistanceFromCenterXY) network.setValue("dxy",distanceFromCenterXY);
				if(addDistanceFromCenterYZ) network.setValue("dyz",distanceFromCenterYZ);
				if(addDistanceFromCenterXZ) network.setValue("dxz",distanceFromCenterXZ);
				if(addArcTan2) network.setValue("arcTan2",arcTan2Val);
				//network.setValue("Bias",0.3);                      
				
				network.update();                                   //JMC: on this line we run the CPPN network...  
				
				ContinuousArray[j] = network.getValue("Output");        //JMC: and here we get the CPPN output (which is the weight of the connection between the two)
				
				//hack directly into array @#
				//ContinuousArray[j] = e;
				//cout << "e: " << e << "   i " << i ;



								
				if(useColor){
					rColorArray[j]     = (1 + network.getValue("Output-colorR"))/2.0;
					gColorArray[j]     = (1 + network.getValue("Output-colorG"))/2.0;
					bColorArray[j]     = (1 + network.getValue("Output-colorB"))/2.0;
				}
				
//				PRINT(rColorArray[j]);

				if (ContinuousArray[j]>voxelExistsThreshold) voxelCount++;
				
			}

			i=0;
			
			std::cout << "Performing marching cubes...\n";
			CMesh OutputMesh;
			CMarchCube::SingleMaterialMultiColor(&OutputMesh, &ContinuousArray, &rColorArray, &gColorArray, &bColorArray, voxelExistsThreshold, VoxelSize*1000);	//jmc: last argument is the threshold above which we consider the voxel to be extant
			
			meshesToDraw.push_back(OutputMesh);
						
		}

		
		cout << "drawing shapes" << endl;
		fitnessScores = drawShapes(meshesToDraw, _genNumber);

		cout << "fitness values used when not getting list of selected orgs from command line" << endl;
		for(int z=0;z< group.size();z++)
		{
			PRINT(fitnessScores[z]);
			shared_ptr<NEAT::GeneticIndividual> individual = group[z];
			if (fitnessScores[z]>	5) individual->reward(fitnessScores[z]);	//if picked by user (either as champ or tie-for-second, use those scores
			else 
			{
				individual->reward(1);	
			}
		}
		
					
    }
	
    void ShapesExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
    {
		//if doing interactive evolution...call processInteractiveEvaluation() and skip rest of this function
		#ifdef INTERACTIVELYEVOLVINGSHAPES
		processInteractiveEvaluation(generation->getGenerationNumber());
		return;
		#endif
		
		
		for(int z=0;z< group.size();z++)
		{
			shared_ptr<NEAT::GeneticIndividual> individual = group[z];

			#if SHAPES_EXPERIMENT_DEBUG
			  cout << "in ShapesExperiment.cpp::processIndividual, processing individual:  " << individual << endl;
			#endif
			double fitness = 0;
			
			
			fitness = processEvaluation(individual);
					
			if(fitness > std::numeric_limits<double>::max())
			{
				cout << "error: the max fitness is greater than the size of the double. " << endl;
				cout << "max double size is: : " << std::numeric_limits<double>::max() << endl;
				exit(88);
			} 
			if (fitness < 0)
			{
				cout << "Fitness Less Than Zero!!!, it is: " << fitness << "\n";  
				exit(10);
			}
			
			#if SHAPES_EXPERIMENT_DEBUG        
				cout << "Individual Evaluation complete!\n";
				printf("fitness: %f\n", fitness);	
			#endif

			cout << "Individual Evaluation complete!\n";
			cout << "fitness: " << fitness << endl;
	  
			individual->reward(fitness);

			if (false)	//to print cppn
			{
				printNetworkCPPN(individual);    
			}
			
		}
    }

    void ShapesExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
    {
        /*{
            mutex::scoped_lock scoped_lock(*Globals::ioMutex);
            cout << "Starting Evaluation on object:" << this << endl;
            cout << "Running on individual " << individual << endl;
        }*/

        cout << "Sorry, this was never coded up for ShapeRecognitioV1. You'll have to do that now." << endl;
        exit(6);
        
        //in jason's code, the got 10 points just for entering the game, wahooo!
        individual->setFitness(0);

        double fitness=0;

        double maxFitness = 0;

        //bool solved=true; @jmc: was uncommented in the original and produced an error forn not being used

        //cout << "Individual Evaluation complete!\n";

        //cout << maxFitness << endl;

        individual->reward(fitness);

        if (fitness >= maxFitness*.95)
        {
            cout << "PROBLEM DOMAIN SOLVED!!!\n";
        }
    }

    
    void ShapesExperiment::printNetworkCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
      cout << "Printing cppn network" << endl;
      ofstream network_file;        
      network_file.open ("networkCPPN-ThatSolvedTheProblem.txt", ios::trunc );
      
      NEAT::FastNetwork <double> network = individual->spawnFastPhenotypeStack <double>();        //JMC: this creates the network CPPN associated with this individual used to produce the substrate (neural network)

      network_file << "num links:" << network.getLinkCount() << endl;
      network_file << "num nodes:" << network.getNodeCount() << endl;
 
      int numLinks = network.getLinkCount();
      int numNodes = network.getNodeCount();
      ActivationFunction activationFunction;

      //print out which node corresponds to which integer (e.g. so you can translate a fromNode of 1 to "x1"  
      map<string,int> localNodeNameToIndex = *network.getNodeNameToIndex();
      for( map<string,int>::iterator iter = localNodeNameToIndex.begin(); iter != localNodeNameToIndex.end(); iter++ ) {
        network_file << (*iter).first << " is node number: " << (*iter).second << endl;
      }
      
      for (size_t a=0;a<numLinks;a++)
      {
          
          NetworkIndexedLink <double> link = *network.getLink(a);
           
          network_file << link.fromNode << "->" << link.toNode << " : " << link.weight << endl;
      }
      for (size_t a=0;a<numNodes;a++)
      {          
          activationFunction = *network.getActivationFunction(a);           
          network_file << " activation function " << a << ": ";
          if(activationFunction == ACTIVATION_FUNCTION_SIGMOID) network_file << "ACTIVATION_FUNCTION_SIGMOID";
          if(activationFunction == ACTIVATION_FUNCTION_SIN) network_file << "ACTIVATION_FUNCTION_SIN";
          if(activationFunction == ACTIVATION_FUNCTION_COS) network_file << "ACTIVATION_FUNCTION_COS";
          if(activationFunction == ACTIVATION_FUNCTION_GAUSSIAN) network_file << "ACTIVATION_FUNCTION_GAUSSIAN";
          if(activationFunction == ACTIVATION_FUNCTION_SQUARE) network_file << "ACTIVATION_FUNCTION_SQUARE";
          if(activationFunction == ACTIVATION_FUNCTION_ABS_ROOT) network_file << "ACTIVATION_FUNCTION_ABS_ROOT";
          if(activationFunction == ACTIVATION_FUNCTION_LINEAR) network_file << "ACTIVATION_FUNCTION_LINEAR";
          if(activationFunction == ACTIVATION_FUNCTION_ONES_COMPLIMENT) network_file << "ACTIVATION_FUNCTION_ONES_COMPLIMENT";
          if(activationFunction == ACTIVATION_FUNCTION_END) network_file << "ACTIVATION_FUNCTION_END";
          network_file << endl;
      }

      network_file.close();

      return;
    }

    Experiment* ShapesExperiment::clone()
    {
        ShapesExperiment* experiment = new ShapesExperiment(*this);

        return experiment;
    }
    
	
	bool ShapesExperiment::converged(int generation) {
		if(generation == convergence_step)
			return true;
		return false;
	}		

	void ShapesExperiment::writeAndTeeUpParentsOrderFile() {		//this lets django know what the order in the vector was of each parent for each org in the current gen
		string outFilePrefix = NEAT::Globals::getSingleton()->getOutputFilePrefix();
		std::ostringstream parentFilename;
		std::ostringstream parentFilenameTemp;
		std::ostringstream parentFilenameCmd;

		parentFilename << outFilePrefix << "-parents";
		parentFilenameTemp << parentFilename.str() << ".tmp";

		FILE* file;
		file = fopen(parentFilenameTemp.str().c_str(), "w");		
		if (!file) 
		{
			cout << "could not open parent order file" << endl;
			exit(33);
		}

        bool parentFound = false;
		for(int s=0;s< group.size();s++)
		{
			shared_ptr<NEAT::GeneticIndividual> individual = group[s];
			individual->setOrder(s); //tells each org what order in the vector they were in when the were evaluated (tees up the info for the next time)
			int parent1Order = individual->getParent1Order();
			int parent2Order = individual->getParent2Order();
			
			PRINT(parent1Order);
			PRINT(parent2Order);
			
			//write info 
			if(parent1Order > -1)  fprintf(file, "%i ", parent1Order);
			if(parent2Order > -1)  fprintf(file, "%i ", parent2Order);
		    fprintf(file, "\n");				
		
		    if(parent1Order > -1 || parent2Order > -1) parentFound = true;
		    PRINT(parentFound);
						
		}
		fclose(file);
		//mv the temp file to the non-temp name
		if(parentFound) //only print the file if we found a parent (i.e. it is not the first gen)
        {
			if(firstGenSeedingSoDontPrintParentsFile)
			{
				firstGenSeedingSoDontPrintParentsFile = false; //don't enter this condition ever again
			}
			else
			{
				cout << "CREATING PARENTS FILE" << endl;
				parentFilenameCmd << "mv " << parentFilenameTemp.str() << " " << parentFilename.str();
				int result = ::system(parentFilenameCmd.str().c_str());
				(void) result;
			}
        }
		
	}		
	
	
	void ShapesExperiment::generateTarget3DObject()
	{

		
		if(true)	//use Hiller's object
		{
			
			//cout << "generating sample 3D image from Jon Hiller's code" << endl;
			int NumSpheres = 4; //number of spheres to create
			double DistSpheres = 0.005; //distance along the X axis between the spheres (meters)
			double RadiusSpheres = 0.004; //radius of the spheres (meters)
			std::cout << "Initializing continuous object with " << NumSpheres << " spheres radius " << RadiusSpheres*1000 << "mm, " << DistSpheres*1000 << "mm apart.\n\n";
			
			
			//std::cout << "Rendering Spheres to voxels:\n";
			for (int i=0; i<NumSpheres; i++) //iterate through each sphere
			{
				Vec3D CurCenterMeters(WorkSpace.x/2.0, (double)i/(NumSpheres-1)*WorkSpace.y, WorkSpace.z/2); //center point of this sphere (in meters)
				Vec3D CurCenter(CurCenterMeters.x/VoxelSize, CurCenterMeters.y/VoxelSize, CurCenterMeters.z/VoxelSize); //center point of sphere in voxel indices
				
				//fill in the continuous array for doing marching cubes later...
				int px, py, pz; //temporary variable to store locations as we iterate
				Vec3D P; //Store location as we iterate
				double DistToCenter; //temporary variable to store distance from each point to sphere center as we iterate
				for (int j=0; j<TargetContinuousArray.GetFullSize(); j++){ //iterate through each location of the continuous matrix
					TargetContinuousArray.GetXYZ(&px, &py, &pz, j); //gets XYZ location of this element in the array
					P.x = px+0.5; //Offset of 0.5 mean we are sampling at the CENTER of each voxel, not lower corner
					P.y = py+0.5;
					P.z = pz+0.5;
					DistToCenter = (CurCenter - P).Length(); //distance formula
					if (DistToCenter < RadiusSpheres/VoxelSize) TargetContinuousArray[j] += (float)(1 - DistToCenter/(RadiusSpheres/VoxelSize));
					//				PRINT(ContinuousArray[j]);
				}
			}
		} else  //manual shapes
		{
			int px, py, pz; //temporary variable to store locations as we iterate
			float xNormalized;
			float yNormalized;
			float zNormalized;
			
			for (int j=0; j<TargetContinuousArray.GetFullSize(); j++){ //iterate through each location of the continuous matrix
				TargetContinuousArray.GetXYZ(&px, &py, &pz, j); //gets XYZ location of this element in the array
				xNormalized = mapXYvalToNormalizedGridCoord(px, num_x_voxels);
				yNormalized = mapXYvalToNormalizedGridCoord(py, num_y_voxels);
				zNormalized = mapXYvalToNormalizedGridCoord(pz, num_z_voxels);
				
				if(true)TargetContinuousArray[j] += 1; //fill in manual rule
			}
		} 
		
//		cout << "TARGET CONTINUOUS ARRAY" << endl;
//		for (int j=0; j<TargetContinuousArray.GetFullSize(); j++)
//		{
//			printf("%f: \n", TargetContinuousArray[j]);
//		}
//		exit(3);

		CMarchCube::SingleMaterial(&TargetMesh, &TargetContinuousArray, voxelExistsThreshold);	//jmc: last argument is the threshold above which we consider the voxel to be extant
		TargetMesh.Save("targetObject.stl");
		std::cout << "Saving target object stl file targetObject.stl" << endl;

		
	}
	
	int ShapesExperiment::compareEvolvedArrayToTargetArray(CArray3Df &ContinuousArray)
	{	
		//note: this function should never change ContinuousArray or TargetContinuousArray. they are not const because I need to use the member functions
		int numSame = 0;
		assert(ContinuousArray.GetFullSize() == TargetContinuousArray.GetFullSize());
		for (int j=0; j<ContinuousArray.GetFullSize(); j++)  //iterate through each location of the continuous matrix
		{
			if (ContinuousArray[j] >= voxelExistsThreshold and TargetContinuousArray[j] >= voxelExistsThreshold)
			{
				numSame++;
			}
			else if (ContinuousArray[j] < voxelExistsThreshold and TargetContinuousArray[j] < voxelExistsThreshold)
			{
				numSame++;
			}

		}
		
		return numSame;
		
	}
						
	
	double ShapesExperiment::drawShape(bool m_EvolvingInteractively, CMesh& meshToDraw)
	{
	#ifdef VISUALIZESHAPES
/*
		//BEGIN VISUALIZATION/GUI CODE
		CMesh* meshToDrawPtr = &meshToDraw; //store pointer to mesh we want to draw to provide access in display function

		
		float m_InteractiveFitnessScore = 0;
		GLfloat viewangle = 0.2, tippangle = 0;
		
		
		// Create the main window
		sf::Window App(sf::VideoMode(windowWidth, windowHeight, 32), "SFML OpenGL");
		
		// Create a clock for measuring time elapsed
		sf::Clock Clock;
		
		
		initGL(); //do the onetime gl initializations
		
		
		// Start visualization opengl sfml window loop
		while (App.IsOpened())
		{
			// Process events
			sf::Event Event;
			while (App.GetEvent(Event))
			{
				//TODO put all the KeyPressed events in one case switch
				// Close window : exit
				if (Event.Type == sf::Event::Closed)
					App.Close();

				// Resize event : adjust viewport
				if (Event.Type == sf::Event::Resized)
				{
					glViewport(0, 0, Event.Size.Width, Event.Size.Height);
				}
				
				// Key Pressed
				if (Event.Type == sf::Event::KeyPressed)
				{
					//cout << "key pressed\n" << endl;
					
					switch (Event.Key.Code) {
						case  sf::Key::Escape:
							cout << "escape key pressed\n";
							App.Close();							
							break;
							
						case  sf::Key::Q:
							cout << "q key pressed\n";
							exit(0); //exit entire program
							break;
							
							//arrow keys rotate	
						case  sf::Key::Right:
							//cout << "Rotating right\n";
							viewangle += 0.1;
							break;
							
						case  sf::Key::Left:
							//cout << "Rotating left\n";
							viewangle -= 0.1;
							break;
							
						case  sf::Key::Up:
							cout << "Rotating up\n";
							tippangle -= 0.1;
							break;
							
						case  sf::Key::Down:
							cout << "Rotating down\n";
							tippangle += 0.1;
							break;
							
						case  sf::Key::F:
							cout << "Fast mode\n";
							viewThresh = 0.5;
							break;
							
						case  sf::Key::M:
							cout << "Medium mode\n";
							viewThresh = 3.0;
							break;
							
						case  sf::Key::S:
							cout << "Slow mode\n";
							viewThresh = 10.0;
							break;
							
						case  sf::Key::I:
							cout << "Interactive mode\n";
							viewThresh = 100000.0;
							break;
							
							
						default:
							cout << "default\n";
							
							//check for entry of a number 1-9
							cout << "got the following ";
							int key = Event.Key.Code;
							PRINT(key);
							if(key <= 57 and key >= 49)
							{
								int keyVal = key-48;
								printf("got the follwing number: %i\n", keyVal);
								if(m_EvolvingInteractively)	//if evolving interactively, grab this for the fitness score: otherwise ignore it
								{
									m_InteractiveFitnessScore = float(keyVal);
									App.Close();							
								}
							}
							
							break;
					}
					
				}				
				
				
				
			}
			
			// Set the active window before using OpenGL commands
			// It's useless here because active window is always the same,
			// but don't forget it if you use multiple windows or controls
			App.SetActive();
			
			
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glRotatef (tippangle, 1,0,0);  // Up and down arrow keys 'tip' view.
			glRotatef (viewangle, 0,1,0);  // Right/left arrow keys 'turn' view.	
			
			//draw first object
			glViewport (0, windowHeight/2, windowWidth/2, windowHeight/2);
			meshToDrawPtr->Draw(false, true);

			//draw second object
			glViewport (windowWidth/2, windowHeight/2, windowWidth/2, windowHeight/2);			
			TargetMesh.Draw(false, true);

			//draw third object
			glViewport (0, 0, windowWidth/2, windowHeight/2);			
			TargetMesh.Draw(false, true);

			//draw fourth object
			glViewport (windowWidth/2, 0, windowWidth/2, windowHeight/2);			
			TargetMesh.Draw(false, true);
			
			
			// Finally, display rendered frame on screen
			App.Display();
			
			// reset if elapsed time since last draw > viewThresh
			if (Clock.GetElapsedTime() > viewThresh) App.Close();
			
			
		}
		if(EXIT_SUCCESS){ cout << "SFML error\n"; exit(1);}
		
		
		return m_InteractiveFitnessScore; 
*/
	#else
		cout << "you can't call this function without #defining VISUALIZESHAPES" << endl;
		exit(-1);
		double onlyForCompiling = 0.0;
		return onlyForCompiling;
	#endif
	}
	
	vector <int> ShapesExperiment::drawShapes(vector <CMesh> m_meshesToDraw, int _generationNumber)
	{
		vector <int> m_fitnessScores; 
		for(int s=0;s<group.size();s++)
		{
			m_fitnessScores.push_back(1);		//default fitness score if not chosen
		}

		
#ifdef VISUALIZESHAPES

		/*
		GLfloat viewangle = 0.5, tippangle = 0;
		int clickX = -1; //locations where the user clicked (negative means invalid--out of window)
		int clickY = -1; 
		bool printSelectedObjects = false;

//		uncomment the next two lines to always pick the first one without asking the user (to test code speed)
//		m_fitnessScores[1] = 1000;	//chosenOne
//		return m_fitnessScores;				


		
		// Create the main window
		sf::Window App(sf::VideoMode(windowWidth, windowHeight, 32), "SFML OpenGL");
		
		// Create a clock for measuring time elapsed
		sf::Clock Clock;
		
		
		initGL(); //do the onetime gl initializations
		
		
		// Start visualization opengl sfml window loop
		while (App.IsOpened())
		{
			// Process events
			sf::Event Event;
			while (App.GetEvent(Event))
			{
				//TODO put all the KeyPressed events in one case switch
				// Close window : exit
				if (Event.Type == sf::Event::Closed)
					App.Close();
				
				// Key Pressed
				if (Event.Type == sf::Event::KeyPressed)
				{
					//cout << "key pressed\n" << endl;
					
					switch (Event.Key.Code) {
//						case  sf::Key::Escape:
//							cout << "escape key pressed\n";
//							App.Close();							
//							break;
							
						case  sf::Key::Q:
							//cout << "q key pressed\n";
							exit(0); //exit entire program
							break;
							
							//arrow keys rotate	
						case  sf::Key::Right:
							//cout << "Rotating right\n";
							viewangle += 0.1;
							break;
							
						case  sf::Key::Left:
							//cout << "Rotating left\n";
							viewangle -= 0.1;
							break;
							
						case  sf::Key::Up:
							cout << "Rotating up\n";
							tippangle -= 0.1;
							break;
							
						case  sf::Key::Down:
							cout << "Rotating down\n";
							tippangle += 0.1;
							break;
							
						case  sf::Key::F:
							cout << "Fast mode\n";
							viewThresh = 0.5;
							break;
							
						case  sf::Key::M:
							cout << "Medium mode\n";
							viewThresh = 3.0;
							break;
							
						case  sf::Key::S:
							cout << "Slow mode\n";
							viewThresh = 10.0;
							break;
							
						case  sf::Key::I:
							cout << "Interactive mode\n";
							viewThresh = 100000.0;
							break;

						case  sf::Key::P:
							cout << "print mode\n";

							printSelectedObjects = true;
							break;
							
							
						default:
							cout << "default\n";
							
							//check for entry of a number 1-9
							cout << "got the following in drawShapes ";
							int key = Event.Key.Code;
							PRINT(key);
							if(key <= 57 and key >= 49)
							{
								int keyVal = key-48;
								printf("got and ignoring the follwing number: %i\n", keyVal);
								
							}
							
							break;
					}
					
				}	
				
				if (Event.Type == sf::Event::MouseButtonPressed)
				{
					
					clickX = App.GetInput().GetMouseX();
					clickY = windowHeight - App.GetInput().GetMouseY();
				}				
				
				// Resize event : adjust viewport
				if (Event.Type == sf::Event::Resized)
				{
					glViewport(0, 0, Event.Size.Width, Event.Size.Height);
					windowWidth = Event.Size.Width;
					windowHeight = Event.Size.Height;
				}
				
			}

			

			// Set the active window before using OpenGL commands
			// It's useless here because active window is always the same,
			// but don't forget it if you use multiple windows or controls
			App.SetActive();
			
			
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glRotatef (tippangle, 1,0,0);  // Up and down arrow keys 'tip' view.
			glRotatef (viewangle, 0,1,0);  // Right/left arrow keys 'turn' view.	

			CMesh meshToDraw;		
			CMesh* meshToDrawPtr; //store pointer to mesh we want to draw to provide access in display function

			int windowStartY;
			int windowStartX;
			int shapeCounter = 0;
			float viewPortWidth =  windowWidth/windowsPerRow;
			float viewPortHeight = windowHeight/windowsPerCol;
			int whenY0IsAtBottom;
			
			for (int y=0; y<windowsPerCol; y++)
			{
				windowStartY = y * viewPortHeight; 
				
				for (int x=0; x<windowsPerRow; x++)
				{
					if(shapeCounter > int(NEAT::Globals::getSingleton()->getParameterValue("PopulationSize"))-1) continue; // can't draw shapes that don't exist (or it will crash)
					meshToDraw = m_meshesToDraw[shapeCounter];		
					meshToDrawPtr = &meshToDraw; //store pointer to mesh we want to draw to provide access in display function			
					
					windowStartX = x * viewPortWidth;
					
					whenY0IsAtBottom = windowHeight - Event.MouseMove.Y;					

					// first check if we have received a click in this region, if so, return with the chosen one
					if(		clickX > windowStartX and clickX < (windowStartX + viewPortWidth) 
					   and  clickY > windowStartY and clickY < (windowStartY + viewPortHeight))
					{
						if (Event.MouseButton.Button == 0)
						{
							m_fitnessScores[shapeCounter] = 1000;	//chosenOne
							return m_fitnessScores;					//return since we have a selection
						}
						else
						{	
							m_fitnessScores[shapeCounter] = 1000;		//runner(s) up consolation prize (same as winner)							
						}
					}

					
					//color object mouse is over green
					if(		Event.MouseMove.X > windowStartX and Event.MouseMove.X < (windowStartX + viewPortWidth) 
					   and  whenY0IsAtBottom  > windowStartY and whenY0IsAtBottom  < (windowStartY + viewPortHeight)
					   and !(Event.MouseMove.X == 32 and Event.MouseMove.Y==32))//last clause is because it defaults to these on a new window before a mouse move
					{
						glDisable(GL_LIGHT0);	//turn off red light
						
						// Turn on a green light. 
						GLfloat light_diffuse[] = {0.0, 1.0, 0.0, 1.0};  // green diffuse light. 
						GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  // Infinite light location. 						
						//glLightfv(GL_LIGHT0, GL_EMISSION, light_diffuse);
						glLightfv(GL_LIGHT1, GL_AMBIENT, light_diffuse);
						glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
						glLightfv(GL_LIGHT1, GL_POSITION, light_position);
						glEnable(GL_LIGHT1);
						glEnable(GL_LIGHTING);
						
					}
					else
					{
						glDisable(GL_LIGHT1);
						glEnable(GL_LIGHT0);
					}

					//color objects already selected yellow
					if(m_fitnessScores[shapeCounter] > 1)
					{
						glDisable(GL_LIGHT0);	//turn off red light
						
						// Turn on a yellow light. 
						GLfloat light_diffuse[] = {1.0, 1.0, 0.0, 1.0};  // green diffuse light. 
						GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  // Infinite light location. 						
						//glLightfv(GL_LIGHT0, GL_EMISSION, light_diffuse);
						glLightfv(GL_LIGHT2, GL_AMBIENT, light_diffuse);
						glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
						glLightfv(GL_LIGHT2, GL_POSITION, light_position);
						glEnable(GL_LIGHT2);
						glEnable(GL_LIGHTING);
						
						if(printSelectedObjects)
						{
							cout << "printing selected shapes" << endl;

							//all this to get the time in string form 
							time_t rawtime;
							time ( &rawtime );												
							string timeString = ctime (&rawtime);
							
							std::string s;
							std::stringstream out;
							out << shapeCounter;
							s = out.str();
							
							string filename = "Shape" + timeString + s + ".stl";
							string filenameTrim = "ShapeTrim" + timeString + s + ".stl";
							meshToDraw.Save(filename.c_str());
							meshToDraw.SaveTrimmed(filenameTrim.c_str());
							
							cout << "done printing" << endl;

						}
						
					}
					else
					{
						glDisable(GL_LIGHT2);
					}
					
					
					glViewport (windowStartX, windowStartY, viewPortWidth, viewPortHeight);
					meshToDrawPtr->Draw(false, true);
					shapeCounter++;


				}
			}
			printSelectedObjects = false;
									
			// Finally, display rendered frame on screen
			App.Display();
			
			// reset if elapsed time since last draw > viewThresh
			if (Clock.GetElapsedTime() > viewThresh) App.Close();
			
			
		}
		if(EXIT_SUCCESS){ cout << "SFML error\n"; exit(1);}
		
	*/			
#else
		string outFilePrefix = NEAT::Globals::getSingleton()->getOutputFilePrefix();

		
		#ifdef INTERACTIVELYEVOLVINGSHAPES
		std::ostringstream JSfilename;
		std::ostringstream JSfilenameTemp;
		std::ostringstream JSfilenameCmd;

		cout << "gen number before any adjustments" << endl;
		PRINT(_generationNumber);
		

		if(firstGen)
		{
			_generationNumber = 0; //on restart, this gen number in the first generation only is the highest gen from the previous run
			
			firstGen = false;
			cout << "it's first gen, so making it 0" << endl;
		}
		else if(int(NEAT::Globals::getSingleton()->getParameterValue("SeedingSoDecrementByOne")))
		{
		    _generationNumber = _generationNumber -1;
		    cout << "after adjustment" << endl;
		    PRINT(_generationNumber);
		}


		
		PRINT(_generationNumber);

	
		for(int s=0;s<group.size();s++)
		{
			JSfilename << outFilePrefix << "_" << setw(5) << std::setfill( '0' ) << _generationNumber << "_" << setw(3) << std::setfill( '0' ) << s << ".vf";
			JSfilenameTemp << JSfilename.str() << ".tmp";
			
			m_meshesToDraw[s].SaveThreeJS(JSfilenameTemp.str().c_str());

			JSfilenameCmd << "mv " << JSfilenameTemp.str() << " " << JSfilename.str();
			int result = ::system(JSfilenameCmd.str().c_str());
			(void) result;
			
			JSfilename.str("");
			JSfilenameTemp.str("");
			JSfilenameCmd.str("");
		}
		#endif
		

		string fitnessFilename = outFilePrefix + "-fitness";
		cout << "waiting to gobble gobble up a fitness file with name " << fitnessFilename << endl;

		bool fitFileThere = false;
		ifstream fitnessFile;
        cout << "Waiting for fitness file " << fitnessFilename << endl;
		while(!fitFileThere)//wait till fitness file is there
		{
		    cout << ". ";
            cout.flush();
		    usleep(500000);
		    fitnessFile.open(fitnessFilename.c_str());
		    if(fitnessFile) fitFileThere = true;
		    
		}
		
		for(int s=0;s<group.size();s++)
		{
		    fitnessFile >> m_fitnessScores[s]; 
		    cout << m_fitnessScores[s] << endl;
		}

		fitnessFile.close();
		string rm = "rm ";
		string command = rm + fitnessFilename.c_str();
		int result = ::system(command.c_str());
		(void) result;

		cout << "just grabbed fitness values from a file" << endl;


#endif
		
	//real return statement handled above	
	return m_fitnessScores; //this line should never be reached
	}
	
	
}
