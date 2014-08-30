#include "mpi.h"

#include "HCUBE_MPIEvaluationSet.h"

#define MPI_EVALUATION_SET_DEBUG (0)

namespace HCUBE
{
    void MPIEvaluationSet::run()
    {
#if MPI_EVALUATION_SET_DEBUG
        cout << "Starting EvaluationSet\n";
#endif

        try
        {
            //Process individuals sequentially
            running=true;

            vector<shared_ptr<NEAT::GeneticIndividual> >::iterator tmpIterator;

            ostringstream ostr;

            if (targetProcessor!=0)
            {
                int testCount = 0;

                ostr << testCount << " ";

            }

            ostr << individualCount << " ";

#if MPI_EVALUATION_SET_DEBUG
            cout << "Collecting individuals...\n";
#endif

            tmpIterator = individualIterator;
            for (int a=0;a<individualCount;a++,tmpIterator++)
            {
                while (!running)
                {
                    boost::xtime xt;
                    boost::xtime_get(&xt, boost::TIME_UTC);
                    xt.sec += 1;
                    boost::thread::sleep(xt); // Sleep for 1 second
                }

                if (targetProcessor==0)
                {
                    //main processor, just perform the experiment
                    experiment->addIndividualToGroup(*tmpIterator);

                    if (experiment->getGroupSize()==experiment->getGroupCapacity())
                    {
                        //cout << "Processing group...\n";
                        experiment->processGroup(generation);
                        //cout << "Done Processing group\n";
                        experiment->clearGroup();
                    }

                    if ((*tmpIterator)->getFitness() <= 1e-6)
                    {
                        throw CREATE_LOCATEDEXCEPTION_INFO("ERROR, 0 Fitness");
                    }
                }
                else
                {
                    //other processor, package the individuals for transfer
                    (*tmpIterator)->dump(ostr);
                }
            }

            if (targetProcessor==0)
            {
                if (experiment->getGroupSize()>0)
                {
                    //Oops, maybe you specified a bad # of threads?
                    throw CREATE_LOCATEDEXCEPTION_INFO("Error, individuals were left over after run finished!");
                }
            }
            else
            {

#if MPI_EVALUATION_SET_DEBUG
                cout << "Sending individuals...\n";
#endif

                string str = ostr.str();

                char *buffer = new char[str.length()+1];

                memcpy(buffer,str.c_str(),str.length());

                buffer[str.length()] = '\0'; //null terminate

                int lengthInt = (int)str.length() + 1;

                MPI_Send (&lengthInt,1,MPI_INT,targetProcessor,1,MPI_COMM_WORLD);
                MPI_Send (buffer,str.length(),MPI_CHAR,targetProcessor,1,MPI_COMM_WORLD);

                double *newFitness = new double[individualCount];

#if MPI_EVALUATION_SET_DEBUG
                cout << "Waiting for results...\n";
#endif

                {
                    MPI_Status Stat;
                    MPI_Recv (newFitness,individualCount,MPI_DOUBLE,targetProcessor,1,MPI_COMM_WORLD,&Stat);
                }

#if MPI_EVALUATION_SET_DEBUG
                cout << "Received new fitness values\n";
#endif

                tmpIterator = individualIterator;
                for (int a=0;a<individualCount;a++,tmpIterator++)
                {
                    if (fabs(newFitness[a])<1e-6)
                    {
                        throw CREATE_LOCATEDEXCEPTION_INFO("Error, 0 fitness!");
                    }
                    (*tmpIterator)->setFitness(newFitness[a]);
                }


                delete newFitness;
            }

#if MPI_EVALUATION_SET_DEBUG
            cout << "Done with EvaluationSet!\n";
#endif

            finished=true;
        }
        catch (string s)
        {
            CREATE_PAUSE(s);
        }
        catch (const char *s)
        {
            CREATE_PAUSE(s);
        }
        catch (const std::exception &ex)
        {
            CREATE_PAUSE(ex.what());
        }
        catch (...)
        {
            CREATE_PAUSE("An unknown exception has occured!");
        }
    }

}
