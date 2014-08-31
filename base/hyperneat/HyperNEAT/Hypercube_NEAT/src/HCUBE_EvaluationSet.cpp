#include "HCUBE_EvaluationSet.h"



namespace HCUBE
{
    void EvaluationSet::run()
    {
#ifndef _DEBUG
        try
#endif
        {
            //Process individuals sequentially
            running=true;

            vector<shared_ptr<NEAT::GeneticIndividual> >::iterator tmpIterator;

            tmpIterator = individualIterator;
            for (int a=0;a<individualCount;a++,tmpIterator++)
            {
                while (!running)
                {
                    boost::xtime xt;
                    boost::xtime_get(&xt, boost::TIME_UTC_); //(nac: changed for latest version of boost)
                    xt.sec += 1;
					cout << "jmc: used1!\n"; exit(3);
                    boost::thread::sleep(xt); // Sleep for 1 second
                }

                experiment->addIndividualToGroup(*tmpIterator);

                if (experiment->getGroupSize()==experiment->getGroupCapacity())
                {
                    //cout << "Processing group...\n";
                    experiment->processGroup(generation);
                    //cout << "Done Processing group\n";
                    experiment->clearGroup();
                }
            }

            if (experiment->getGroupSize()>0)
            {
                //Oops, maybe you specified a bad # of threads?
                throw CREATE_LOCATEDEXCEPTION_INFO("Error, individuals were left over after run finished! Do you have the c++ flag -DINTERACTIVELYEVOLVINGSHAPES set? Did you set getGroupCapacity to return the pop size in HCUBE_Experiment.h?");
            }

            finished=true;
        }
#ifndef _DEBUG
        catch (string s)
        {
			cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
            CREATE_PAUSE(s);
        }
        catch (const char *s)
        {
			cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
            CREATE_PAUSE(s);
        }
        catch (const std::exception &ex)
        {
			cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
            CREATE_PAUSE(ex.what());
        }
        catch (...)
        {
			cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
            CREATE_PAUSE("An unknown exception has occured!");
        }
#endif
    }

}
