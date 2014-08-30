#include <iostream>
#include "VX_Object.h"
#include "VX_Environment.h"
#include "VX_Sim.h"
#include "VX_SimGA.h"


int main(int argc, char *argv[])
{
	char* InputFile;
	//create the main objects
	CVXC_Structure structure;
	CVX_Object Object;
	CVX_Environment Environment;
	CVX_SimGA Simulator;
	long int Step = 0;
	vfloat Time = 0.0; //in seconds
	bool print_scrn = false;

	//first, parse inputs. Use as: -f followed by the filename of the .vxa file that describes the simulation. Can also follow this with -p to cause console output to occur
	if (argc < 3) 
	{ // Check the value of argc. If not enough parameters have been passed, inform user and exit.
		std::cout << "\nInput file required. Quitting.\n";
		return(0);	//return, indicating via code (0) that we did not complete the simulation
	} 
	else 
	{ // if we got enough parameters...
		for (int i = 1; i < argc; i++) 
		{ 
			if (strcmp(argv[i],"-f") == 0) 
			{
				InputFile = argv[i + 1];	// We know the next argument *should* be the filename:
			} 
			else if (strcmp(argv[i],"-p") == 0) 
			{
				print_scrn=true;	//decide if output to the console is desired
			}
		}
	} 

	//setup main object
	Simulator.pEnv = &Environment;	//connect Simulation to environment
	Environment.pObj = &Object;		//connect environment to object

	//import the configuration file
	if (!Simulator.LoadVXAFile(InputFile)){
		if (print_scrn) std::cout << "\nProblem importing VXA file. Quitting\n";
		return(0);	//return, indicating via code (0) that we did not complete the simulation
		}
	std::string ReturnMessage;
	if (print_scrn) std::cout << "\nImporting Environment into simulator...\n";

	Simulator.Import(&Environment, 0, &ReturnMessage);
	if (print_scrn) std::cout << "Simulation import return message:\n" << ReturnMessage << "\n";
	
	Simulator.pEnv->UpdateCurTemp(Time);	//set the starting temperature (nac: pointer removed for debugging)

	Simulator.EnableFeature(VXSFEAT_EQUILIBRIUM_MODE);
	StopCondition oldStopConditionType = Simulator.GetStopConditionType();
	vfloat oldStopConditionValue = Simulator.GetStopConditionValue();
	Simulator.SetStopConditionType(SC_MIN_KE);
	Simulator.SetStopConditionValue(0.1);
	while (not Simulator.StopConditionMet())
	{
		// do some reporting via the stdoutput if required:
		if (Step%100 == 0.0 && print_scrn) //Only output every n time steps
		{
			// std::cout << "Time: " << Time << std::endl;
			std::cout << "CM: " << Simulator.GetCM().x << std::endl;
			
			// std::cout << " \tVox 0 X: " << Vox0Pos.x << "mm" << "\tVox 0 Y: " << Vox0Pos.y << "mm" << "\tVox 0 Z: " << Vox0Pos.z << "mm\n";	//just display the position of the first voxel in the voxelarray
			//std::cout << "Scale: " << Simulator.VoxArray[0].GetCurScale().x << std::endl;  // display the scale of voxel 0 as it (potentially) expands and contracts	
		}

		//do the actual simulation step
		Simulator.TimeStep(&ReturnMessage);
		Step += 1;	//increment the step counter
		Time += Simulator.dt;	//update the sim tim after the step
		Simulator.pEnv->UpdateCurTemp(Time);	//pass in the global time, and a pointer to the local object so its material temps can be modified (nac: pointer removed for debugging)	
	}
	std::cout << "Done Round One" << std::endl;

	Simulator.IniCM=Simulator.GetCM();	//set the starting CM position
	std::cout << "Initial CM: " << Simulator.IniCM.x << std::endl;	

	Simulator.EnableFeature(VXSFEAT_EQUILIBRIUM_MODE,false);
	Simulator.SetStopConditionType(SC_MIN_MAXMOVE);

	while (not Simulator.StopConditionMet())
	{
		// do some reporting via the stdoutput if required:
		if (Step%100 == 0.0 && print_scrn) //Only output every n time steps
		{
			// std::cout << "Time: " << Time << std::endl;
			std::cout << "CM: " << Simulator.GetCM().x << std::endl;
			
			// std::cout << " \tVox 0 X: " << Vox0Pos.x << "mm" << "\tVox 0 Y: " << Vox0Pos.y << "mm" << "\tVox 0 Z: " << Vox0Pos.z << "mm\n";	//just display the position of the first voxel in the voxelarray
			//std::cout << "Scale: " << Simulator.VoxArray[0].GetCurScale().x << std::endl;  // display the scale of voxel 0 as it (potentially) expands and contracts	
		}

		//do the actual simulation step
		Simulator.TimeStep(&ReturnMessage);
		Step += 1;	//increment the step counter
		Time += Simulator.dt;	//update the sim tim after the step
		Simulator.pEnv->UpdateCurTemp(Time);	//pass in the global time, and a pointer to the local object so its material temps can be modified (nac: pointer removed for debugging)	
	}
	std::cout << "Done Round Two" << std::endl;



	Simulator.SetStopConditionType(oldStopConditionType);
	Simulator.SetStopConditionValue(oldStopConditionValue+Time);

	// Time = 0.0;
	// Step = 0;
	Simulator.pEnv->UpdateCurTemp(Time);
	std::cout << "Time: " << Time << std::endl;
	
	while (not Simulator.StopConditionMet())
	{
		// do some reporting via the stdoutput if required:
		if (Step%100 == 0.0 && print_scrn) //Only output every n time steps
		{
			// std::cout << "Time: " << Time << std::endl;
			std::cout << "CM: " << Simulator.GetCM().x << std::endl;
			// std::cout << " \tVox 0 X: " << Vox0Pos.x << "mm" << "\tVox 0 Y: " << Vox0Pos.y << "mm" << "\tVox 0 Z: " << Vox0Pos.z << "mm\n";	//just display the position of the first voxel in the voxelarray
			//std::cout << "Scale: " << Simulator.VoxArray[0].GetCurScale().x << std::endl;  // display the scale of voxel 0 as it (potentially) expands and contracts	
		}

		//do the actual simulation step
		Simulator.TimeStep(&ReturnMessage);
		Step += 1;	//increment the step counter
		Time += Simulator.dt;	//update the sim tim after the step
		Simulator.pEnv->UpdateCurTemp(Time);	//pass in the global time, and a pointer to the local object so its material temps can be modified (nac: pointer removed for debugging)	
	}

	if (print_scrn) std::cout << "Ended at: " << Time << std::endl;

	std::cout << "Final CM: " << Simulator.GetCM().x << std::endl;	
	
	Simulator.SaveResultFile(Simulator.FitnessFileName);

	return 1;	//code for successful completion  // could return fitness value if greater efficiency is desired
}
