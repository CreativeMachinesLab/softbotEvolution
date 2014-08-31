#include "mpi.h"

#include "HCUBE_MPIExperimentRun.h"

#include "Experiments/HCUBE_Experiment.h"
#include "Experiments/HCUBE_FindClusterExperiment.h"
#include "Experiments/HCUBE_CheckersExperiment.h"

#define DEBUG_MPI_MAIN (0)

int main(int argc,char **argv)
{
    try
    {
        /*
        for(int a=0;a<argc;a++)
        {
            cout << "ARGUMENT: " << argv[a] << endl;
        }
        */

        int  numtasks, rank, rc;

        rc = MPI_Init(&argc,&argv);

        if (rc != MPI_SUCCESS)
        {
            printf ("Error starting MPI program. Terminating.\n");
            MPI_Abort(MPI_COMM_WORLD, rc);
            return 1;
        }

        MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);

        for (int a=0;a<argc;a++)
        {
            cout << "ARGUMENT: " << argv[a] << endl;
        }

        if (argc==1)
        {
            cout << "You must pass the parameters and the output file as command "
            << "parameters!\n";
        }
        /*
        else if (argc==2)
        {
            //Run the post-hoc analysis on every generation
            HCUBE::ExperimentRun experimentRun;

            experimentRun.setupExperimentInProgress(
                string(argv[1]),
                ""
            );

            int numGenerations = experimentRun.getPopulation()->getGenerationCount();

            HCUBE::Experiment *experiment = experimentRun.getExperiment()->clone();

            {
                string outString = (erase_tail_copy(string(argv[1]),4)+string("_fitness.out"));
                cout << "Creating file " << outString << endl;
                ofstream outfile( outString.c_str() );
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

                    cout << "Beginning fitness evaluation " << (generation+1) << "/" << numGenerations << "...";
                    experiment->addIndividualToGroup(indiv);
                    experiment->processGroup(experimentRun.getGeneration(generation));
                    experiment->clearGroup();
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
                    else
                    {
                        throw CREATE_LOCATEDEXCEPTION_INFO("No user data!\n");
                    }

                }
            }

            {
                string outString = (erase_tail_copy(string(argv[1]),4)+string("_lowres.out"));
                cout << "Creating file " << outString << endl;
                ofstream outfile( outString.c_str() );
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
        }
        */
        else if (argc==3)
        {
            NEAT::Globals::init(string(argv[1]));

            int experimentType = int(NEAT::Globals::getSingleton()->getParameterValue("ExperimentType")+0.001);

            cout << "Loading Experiment: " << experimentType << endl;

            HCUBE::MPIExperimentRun experimentRun;

            experimentRun.setupExperiment(experimentType,string(argv[2]));

            cout << "Experiment set up\n";

            if (rank==0)
            {
                experimentRun.createPopulation();

                experimentRun.setCleanup(true);

                cout << "Population Created\n";

                experimentRun.start();
            }
            else
            {
                shared_ptr<HCUBE::Experiment> experiment(
			experimentRun.getExperiment()->clone()
			);

                //cout << "Experiment pointer: " << experiment << endl;

                char *buffer=NULL;
                int bufferSize=0;

                while (true)
                {
#if DEBUG_MPI_MAIN
                    cout << "Listening for individual chunk size...\n";
#endif

                    int msgSize;

                    {
                        MPI_Status Stat;
                        MPI_Recv (&msgSize,1,MPI_INT,0,1,MPI_COMM_WORLD,&Stat);
                    }

#if DEBUG_MPI_MAIN
                    cout << "Got chunk size: " << msgSize << endl;
#endif

                    if (msgSize==-1)
                    {
                        break;
                    }

                    if (msgSize==0)
                    {
                        continue; // ????  This shouldn't happen, but handle anyways
                    }

                    if (bufferSize<msgSize)
                    {
                        bufferSize = msgSize;
                        buffer = (char*)realloc(buffer,bufferSize);
                    }

                    memset(buffer,0,bufferSize);

#if DEBUG_MPI_MAIN
                    cout << "Getting buffer...\n";
#endif

                    {
                        MPI_Status Stat;
                        MPI_Recv (buffer,msgSize,MPI_CHAR,0,1,MPI_COMM_WORLD,&Stat);
                    }

#if DEBUG_MPI_MAIN
                    cout << "Got Buffer\n";

                    //cout << buffer << endl;
#endif

                    istringstream istr(buffer);

#if DEBUG_MPI_MAIN
                    cout << "Loaded stringstream\n";
#endif


							int testCount;
							istr >> testCount;


                     shared_ptr<NEAT::GeneticGeneration> generation;

			{
				generation = shared_ptr<NEAT::GeneticGeneration>(
		                        new NEAT::GeneticGeneration(0)
			                     );
			}

#if DEBUG_MPI_MAIN
			cout << "Test count: " << testCount << endl;
#endif

 							int individualCount;

                    istr >> individualCount;

#if DEBUG_MPI_MAIN
                    cout << "Individualcount: " << individualCount << endl;
#endif

                    double *newFitness = (double*)malloc(sizeof(double)*individualCount);

#if DEBUG_MPI_MAIN
                    cout << "Fitness buffer created\n";
#endif

                    for (int a=0;a<individualCount;a+=experiment->getGroupCapacity())
                    {
                        for (int b=0;b<experiment->getGroupCapacity();b++)
                        {
                            shared_ptr<NEAT::GeneticIndividual> ind(
                                new NEAT::GeneticIndividual(istr)
                            );

                            experiment->addIndividualToGroup(ind);
                        }

                        experiment->processGroup(generation);

                        for (int b=0;b<experiment->getGroupCapacity();b++)
                        {
                            newFitness[a+b] = experiment->getGroupMember(b)->getFitness();
                        }

                        //Clear the experiment for the next individuals
                        experiment->clearGroup();
                    }

#if DEBUG_MPI_MAIN
                    cout << "Sending new fitness values\n";
#endif

                    MPI_Send (newFitness,individualCount,MPI_DOUBLE,0,1,MPI_COMM_WORLD);

#if DEBUG_MPI_MAIN
                    cout << "Cleaning up new fitness values\n";
#endif

                    free(newFitness);
                }
            }
        }
        else
        {
            cout << "Invalid # of parameters (" << argc << ")!\n";
        }


        MPI_Finalize();
    }
    catch (const std::exception &ex)
    {
        cout << "An exception has occured: " << ex.what() << endl;
    }
    catch (string s)
    {
        cout << "An exception has occured: " << s << endl;
    }
    catch (...)
    {
        cout << "An error has occured!\n";
    }

    return 0;
}

