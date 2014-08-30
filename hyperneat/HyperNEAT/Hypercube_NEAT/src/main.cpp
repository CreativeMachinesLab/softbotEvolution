#include "HCUBE_Defines.h"

#include "JGTL_CommandLineParser.h"


#include "HCUBE_ExperimentRun.h"

#include "Experiments/HCUBE_Experiment.h"
#include "Experiments/HCUBE_FindClusterExperiment.h"
//#include "Experiments/HCUBE_CheckersExperiment.h"
#ifdef ONAMAC
#include <ApplicationServices/ApplicationServices.h>
#endif //ONAMAC

#define PRINT_GENCHAMPFITNESSVIAPOSTHOCEVAL (0)


#define EPOCHS_PER_PRINT (100000)

void printNetwork(NEAT::FastNetwork<double> &testNetwork)
{
	return;
	
	cout << "Network links:\n";
	cout << testNetwork.getLink("Input0","Hidden1")->weight << endl;
	cout << testNetwork.getLink("Input0","Hidden2")->weight << endl;
	cout << endl;
	
	cout << testNetwork.getLink("Input1","Hidden1")->weight << endl;
	cout << testNetwork.getLink("Input1","Hidden2")->weight << endl;
	cout << endl;
	
	cout << testNetwork.getLink("Input2","Hidden1")->weight << endl;
	cout << testNetwork.getLink("Input2","Hidden2")->weight << endl;
	cout << endl;
	
	cout << testNetwork.getLink("Hidden0","Output0")->weight << endl;
	cout << testNetwork.getLink("Hidden1","Output0")->weight << endl;
	cout << testNetwork.getLink("Hidden2","Output0")->weight << endl;
	cout << endl;
	
	CREATE_PAUSE("");
}
using  namespace HCUBE;
int HyperNEAT_main(int argc,char **argv)
{
	
	CommandLineParser commandLineParser(argc,argv);
	//This is for memory debugging
	//_CrtSetBreakAlloc(137405);
	
	try
	{
		if(commandLineParser.HasSwitch("-I"))
		   {
			   string paramFileName = commandLineParser.GetSafeArgument("-I",0,"input.dat");
			   ifstream paramFile;
			   paramFile.open(paramFileName.c_str());
			   if(!paramFile)
			   {
				   cout << "The following parameter file does not exist: " << paramFileName << endl;
				   exit(3);
			   }
			   else
				   cout << "Using the following parameter file: " << paramFileName << endl;
		   }
		
		if (argc<=1)
		{
			cout << "You must pass the parameters and the output file as command "
			<< "parameters!\n";
		}
		else if (argc==2) //running post-hoc eval
		{
			
			//Run the post-hoc analysis on every generation
			HCUBE::ExperimentRun experimentRun;
			
			experimentRun.setupExperimentInProgress(string(argv[1]),"");
			
			int numGenerations = experimentRun.getPopulation()->getGenerationCount();
			
			HCUBE::Experiment *experiment = experimentRun.getExperiment()->clone();
			if(false) //this block is for printing post hoc info on every generation, but not necessary for evaluating an org
			{
#ifndef PRINT_GENCHAMPFITNESSVIAPOSTHOCEVAL
				string outString = (erase_tail_copy(string(argv[1]),3)+string("_fitness.out"));
				cout << "Creating file " << outString << endl;
				ofstream outfile( outString.c_str() );
				string previousSummary;                
				bool doOnce=false;
#endif 
				
				for (int generation=0;generation<numGenerations;generation++)
				{
					//CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
					
					//CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
					
					if ( generation &&
							(*(experimentRun.getIndividual(generation,0).get())) == (*(experimentRun.getIndividual(generation-1,0).get())) )
					{
#ifndef PRINT_GENCHAMPFITNESSVIAPOSTHOCEVAL
						outfile << (generation+1) << ' ' << previousSummary << endl;
#endif
						continue;
					}
					
					shared_ptr<NEAT::GeneticIndividual> indiv =
					shared_ptr<NEAT::GeneticIndividual>(
																							new NEAT::GeneticIndividual(
																																					*(experimentRun.getIndividual(generation,0).get())
																																					)
																							);
					
					//CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
					
					//cout << "Beginning post-hoc evaluation " << (generation+1) << "/" << numGenerations << "...";
					//experiment->processIndividualPostHoc(indiv);
					//cout << "done!\n";
					
					//CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
#ifndef PRINT_GENCHAMPFITNESSVIAPOSTHOCEVAL
					if (indiv->getUserData())
					{
						if (!doOnce)
						{
							doOnce=true;
							outfile
							<< "#Generation: "
							<< indiv->getUserData()->summaryHeaderToString()
							<< endl;
						}
						
						previousSummary = indiv->getUserData()->summaryToString();
						outfile << (generation+1) << ' ' << previousSummary << endl;
					}
					else
					{
						if (!doOnce)
						{
							doOnce=true;
							outfile
							<< "#Generation: "
							<< "Fitness:"
							<< endl;
						}
						
						previousSummary = toString(indiv->getFitness());
						outfile << (generation+1) << ' ' << previousSummary << endl;
					}
#endif
					
				}
			}
			int generation = numGenerations-1; // jmc: interested in last generation (I don't know why it is -1)
			//                    int generation = 0; // jmc: interested in first generation
			cout << "Beginning post-hoc evaluation " << (generation) << "/" << numGenerations-1 << "...\n";
			
			shared_ptr<NEAT::GeneticIndividual> indiv =
			shared_ptr<NEAT::GeneticIndividual>(new NEAT::GeneticIndividual(*(experimentRun.getIndividual(generation,0).get())));
			
			
			
			experiment->processIndividualPostHoc(indiv);
			cout << "done!\n";
#if 0
			
			//CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
			
			if (indiv->getUserData())
			{
				if (!doOnce)
				{
					doOnce=true;
					outfile
					<< "#Generation: "
					<< indiv->getUserData()->summaryHeaderToString()
					<< endl;
				}
				
				previousSummary = indiv->getUserData()->summaryToString();
				outfile << (generation+1) << ' ' << previousSummary << endl;
			}
			else
			{
				if (!doOnce)
				{
					doOnce=true;
					outfile
					<< "#Generation: "
					<< "Fitness:"
					<< endl;
				}
				
				previousSummary = toString(indiv->getFitness());
				outfile << (generation+1) << ' ' << previousSummary << endl;
			}
			
		}
	}
#endif
	
	/*
	 cout << "Done with run!  Running medium resolution tests...\n";
	 ((HCUBE::FindClusterExperiment*)experiment)->increaseResolution();
	 
	 {
	 string outString = (erase_tail_copy(string(argv[1]),4)+string("_midres.out"));
	 ofstream outfile( outString.c_str() );
	 cout << "Creating file " << outString << endl;
	 string previousSummary;
	 
	 bool doOnce=false;
	 
	 for (int generation=0;generation<numGenerations;generation++)
	 {
	 //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
	 
	 //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
	 
	 if ( generation &&
	 (*(experimentRun.getIndividual(generation,0).get())) == (*(experimentRun.getIndividual(generation-1,0).get())) )
	 {
	 outfile << (generation+1) << ' ' << previousSummary << endl;
	 continue;
	 }
	 
	 shared_ptr<NEAT::GeneticIndividual> indiv =
	 shared_ptr<NEAT::GeneticIndividual>(
	 new NEAT::GeneticIndividual(
	 *(experimentRun.getIndividual(generation,0).get())
	 )
	 );
	 
	 //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
	 
	 cout << "Beginning post-hoc evaluation " << (generation+1) << "/" << numGenerations << "...";
	 experiment->processIndividualPostHoc(indiv);
	 cout << "done!\n";
	 
	 //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
	 
	 if (indiv->getUserData())
	 {
	 if (!doOnce)
	 {
	 doOnce=true;
	 outfile
	 << "#Generation: "
	 << indiv->getUserData()->summaryHeaderToString()
	 << endl;
	 }
	 
	 previousSummary = indiv->getUserData()->summaryToString();
	 outfile << (generation+1) << ' ' << previousSummary << endl;
	 }
	 
	 }
	 }
	 */
	}
	else if(commandLineParser.HasSwitch("-X"))
	{
		
		   
		int extraGenerationsToRun;
		if(commandLineParser.HasSwitch("-XG"))
		{
			//			NEAT::Globals::getSingleton()->seedRandom(stringTo<unsigned int>(commandLineParser.GetSafeArgument("-R",0,"0")));
			extraGenerationsToRun = atoi(commandLineParser.GetSafeArgument("-XG",0,"0").c_str());
			cout << "This population will be evolved for the following number of additional generations: " << extraGenerationsToRun << endl;			
		}
		else
		{
			cout << "You must specify how many more generations you want to run for when restarting a run from a saved population file." << endl;
			exit(3);
		}
					
		cerr << "Restarting from where a previous run left off." << endl;
		string outputFilePrefix = commandLineParser.GetSafeArgument("-O",0,"output.xml").c_str();
		string restartingPopFile = commandLineParser.GetSafeArgument("-X",0,"0");

		cout << "Restarting with population from file named: " << restartingPopFile << endl;
		cout << "Going to generate a file with prefix: " << outputFilePrefix << endl;
		
		HCUBE::ExperimentRun experimentRun;
		if(commandLineParser.HasSwitch("-I"))
		{
			
			string inFile = commandLineParser.GetSafeArgument("-I",0,"input.dat");
			cout<< "overriding values from dat file: "<< inFile<<endl;
			experimentRun.setupExperimentInProgressUseDat(inFile, restartingPopFile, outputFilePrefix);
		}
		else
		{
			experimentRun.setupExperimentInProgress(restartingPopFile,outputFilePrefix);
		}

		vector <int> insertedFitnessValues;
		if(commandLineParser.HasSwitch("-F"))
		{
			cout << "using command line to set fitness values for certain orgs to 1000" << endl;
			string insertFitnessValuesString =commandLineParser.GetSafeArgument("-F",0,"0").c_str();//use input fitness values to produce a new generation, otherwise ask for fitness evals first
			cout << "the values being used are: ";
			PRINT(insertFitnessValuesString);
			
			//split the string to doubles
			std::vector<std::string> strs;
			boost::split(strs, insertFitnessValuesString, boost::is_any_of(","));
			for(int z=0;z<strs.size();z++) insertedFitnessValues.push_back(atoi(strs[z].c_str()));
			NEAT::Globals::getSingleton()->addInjectOrgsSelected(insertedFitnessValues);
			NEAT::Globals::getSingleton()->setParameterValue("NeedToInjectFitnessValuesFromCommandLine", 1.0);
		}
			
		if(commandLineParser.HasSwitch("-B"))
		{
			cout << "Received request to start outputting .vf files starting at 1, not 0" << endl;
			NEAT::Globals::getSingleton()->setParameterValue("StartVfFileIndexesAtOne", 1.0);			
		}

		cout << "SEEING IF HAVE SWITCH S" << endl;
		if(commandLineParser.HasSwitch("-S"))
		{
			cout << "Seeding, so decrement all gen counters by one. " << endl;
			NEAT::Globals::getSingleton()->setParameterValue("SeedingSoDecrementByOne", 1.0);			
		}
			
		if(commandLineParser.HasSwitch("-R"))
		{
			NEAT::Globals::getSingleton()->seedRandom(stringTo<unsigned int>(commandLineParser.GetSafeArgument("-R",0,"0")));
		}
		
		SET_PARAMETER("MaxGenerations", extraGenerationsToRun+1); 
		
		if(commandLineParser.HasSwitch("-T")){
			SET_PARAMETER("TerminalFitness",atof(commandLineParser.GetSafeArgument("-T",0,"0").c_str()));
		}
		
		experimentRun.start();
		
		
	}			
		
	else if(commandLineParser.HasSwitch("-I") && commandLineParser.HasSwitch("-O"))
	{
		string inputFile = commandLineParser.GetSafeArgument("-I",0,"input.dat");
		string outputFilePrefix = commandLineParser.GetSafeArgument("-O",0,"output.xml").c_str();
													 														 
		NEAT::Globals::init(inputFile);
		NEAT::Globals::getSingleton()->setOutputFilePrefix(outputFilePrefix); //set the name of the outputPrefixFile
	
		if(commandLineParser.HasSwitch("-R"))
		{
			NEAT::Globals::getSingleton()->seedRandom(stringTo<unsigned int>(commandLineParser.GetSafeArgument("-R",0,"0")));
		}
														 
		if(commandLineParser.HasSwitch("-G")){
			SET_PARAMETER("MaxGenerations", atoi(commandLineParser.GetSafeArgument("-G",0,"0").c_str()));
		}
		
		if(commandLineParser.HasSwitch("-T")){
			SET_PARAMETER("TerminalFitness",atof(commandLineParser.GetSafeArgument("-T",0,"0").c_str()));
		}
		
		
	
		int experimentType = int(GET_PARAMETER("ExperimentType")+0.001);
		
		cout << "Loading Experiment: " << experimentType << endl;
		   cout<<"With Generations: "<< GET_PARAMETER("MaxGenerations")<<endl;
		
		//jmc: remove the file to make the way for the creation of a new one for this run
		ofstream output_file;        
		output_file.open ("gen-Genchamp-AvgFit.txt", ios::trunc );
		output_file.close();
		
		HCUBE::ExperimentRun experimentRun;
		
		experimentRun.setupExperiment(experimentType, outputFilePrefix);
		
		cout << "Experiment set up\n";
		
		experimentRun.createPopulation();
		
		experimentRun.setCleanup(true);
		
		cout << "Population Created\n";
		
		experimentRun.start();
	}
	else
	{
		cout << "Syntax for passing command-line options to HyperNEAT (do not actually type '(' or ')' ):\n";
		cout << "./HyperNEAT [-R (seed)] -I (datafile) -O (outputfile)\n";
	}
	}
	catch (string s)
	{
		cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
		cout << "An exception has occured: " << s << endl;
	}
	catch(LocatedException e) {
		cout << "CAUGHT Exception: " << e.what() << endl;
	}
	catch (...)
	{
		cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
		cout << "An error has occured!\n";
	}
	
	NEAT::Globals::deinit();
	
	return 0;
	}

		//IMPLEMENT_APP(MainApp)
		
	
	int main(int argc,char **argv)
	{
#ifdef ONAMAC
		ProcessSerialNumber PSN;
		GetCurrentProcess(&PSN);
		TransformProcessType(&PSN,kProcessTransformToForegroundApplication);
#endif
		HyperNEAT_main(argc,argv);
	}
