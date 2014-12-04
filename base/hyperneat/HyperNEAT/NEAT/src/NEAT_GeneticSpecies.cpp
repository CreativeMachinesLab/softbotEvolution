#include "NEAT_GeneticSpecies.h"

#include "NEAT_Globals.h"
#include "NEAT_Random.h"

#include "NEAT_GeneticIndividual.h"

namespace NEAT
{

    GeneticSpecies::GeneticSpecies(shared_ptr<GeneticIndividual> firstIndividual)
            :
            age(0),
            ageOfLastImprovement(0),
            speciesFitness(0),
            oldAverageFitness(0)
    {
        ID = Globals::getSingleton()->generateSpeciesID();
        firstIndividual->setSpeciesID(ID);
        bestIndividualEver = firstIndividual;
    }

    GeneticSpecies::~GeneticSpecies()
    {}

    void GeneticSpecies::setBestIndividual(shared_ptr<GeneticIndividual> ind)
    {
        bestIndividualEver = ind;
        // Base improvement age off of best individual update.
        ageOfLastImprovement=age;
    }

    void GeneticSpecies::setMultiplier()
    {
        int dropoffAge = int(Globals::getSingleton()->getParameterValue("DropoffAge"));

        multiplier = 1;

        int ageSinceImprovement = age-ageOfLastImprovement;

        if (ageSinceImprovement>=dropoffAge)
        {
            multiplier *= 0.001;
        }

        //Give a fitness boost up to some young age (niching)
        //The age_significance parameter is a system parameter
        //  if it is 1, then young species get no fitness boost
        double AgeSignificanceParamVal = Globals::getSingleton()->getParameterValue("AgeSignificance");
        if (age<10)
            multiplier *= AgeSignificanceParamVal;

        //Share fitness with the species
        multiplier /= currentIndividuals.size();
    }

    void GeneticSpecies::dump(TiXmlElement *speciesElement)
    {
        //Not implemented.  Not sure what I would want to dump.
    }

    void GeneticSpecies::setFitness()
    {
        speciesFitness=0;
        for (int a=0;a<(int)currentIndividuals.size();a++)
        {
            shared_ptr<GeneticIndividual> individual = currentIndividuals[a];

            speciesFitness+=individual->getFitness();
        }

        // Base improvement age off of average.  Rarely executed because average is always going up/down
        //if(speciesFitness/currentIndividuals.size() > oldAverageFitness)
        // ageSinceLastImprovement=age;

        oldAverageFitness = speciesFitness/currentIndividuals.size();
    }

    void GeneticSpecies::incrementAge()
    {
        age++;
        for (int a=0;a<(int)currentIndividuals.size();a++)
        {
            currentIndividuals[a]->incrementAge();
        }
    }

    void GeneticSpecies::makeBabies(vector<shared_ptr<GeneticIndividual> > &babies)
    {
        int lastIndex = int(Globals::getSingleton()->getParameterValue("SurvivalThreshold")*currentIndividuals.size())-1; //jmc: minus one since orgs are counted starting at zero 
		bool alreadyForcedCross = false;
		
		//set those invididuals beyond SurvivalThreshold percent of the species as not able to reproduce (supposed to filter the top SurvivalThreshold% of the species. 

        // Sort by fitness (nac)
        for (int a=0;a<(int)currentIndividuals.size();a++)
        {
            for (int b=0;b<((int)currentIndividuals.size()-(a+1));b++)
            {
                if (currentIndividuals[b]->getFitness()<currentIndividuals[b+1]->getFitness())
                {
                    shared_ptr<GeneticIndividual> ind = currentIndividuals[b];
                    currentIndividuals[b] = currentIndividuals[b+1];
                    currentIndividuals[b+1] = ind;
                }
            }
        }


        // IF MULTI-OBJECTIVE (nac)
        if (int(NEAT::Globals::getSingleton()->getParameterValue("PenaltyType")) > 0) 
        {

            // CALCULATE DOMINATED RANKS
            for (int a=0;a<(int)currentIndividuals.size();a++)
            {
                currentIndividuals[a]->setDominatedBy(0);
                for (int b=0;b<(int)currentIndividuals.size();b++)
                {
                    // HERE: MAXIMIZE FIRST OBJECTIVE, MINIMIZE SECOND
                    if (a!=b and currentIndividuals[b]->getFitness() > currentIndividuals[a]->getFitness() and currentIndividuals[b]->getFitness2() < currentIndividuals[a]->getFitness2() )
                    {
                        currentIndividuals[a]->setDominatedBy( currentIndividuals[a]->getDominatedBy()+1 );
                    }
                }
            }

            // CALCULATE DIVERSITY
            for (int a=0;a<(int)currentIndividuals.size();a++)
            {
                currentIndividuals[a]->setDiversity(0.0);
            }

            for (int a=0;a<(int)currentIndividuals.size();a++)
            {
                for (int b=0;b<(int)currentIndividuals.size();b++)
                {
                    // HERE: MAXIMIZE FIRST OBJECTIVE, MINIMIZE SECOND
                    if (a!=b and currentIndividuals[b]->getFitness() == currentIndividuals[a]->getFitness() and currentIndividuals[b]->getFitness2() == currentIndividuals[a]->getFitness2()  and currentIndividuals[b]->getDiversity() == 0 )
                    {
                        // cout << a << " and " << b << " tied. Div a: " << currentIndividuals[a]->getDiversity() << ", Div b: " << currentIndividuals[b]->getDiversity() << endl;
                        currentIndividuals[a]->setDiversity( currentIndividuals[a]->getDiversity()+1 );
                        currentIndividuals[a]->setDominatedBy( currentIndividuals[a]->getDominatedBy()+1 );
                    }
                }
            }

            // SORT BY DOMINATED RANK
            for (int a=0;a<(int)currentIndividuals.size();a++)
            {
                for (int b=0;b<((int)currentIndividuals.size()-(a+1));b++)
                {
                    if (currentIndividuals[b]->getDominatedBy() > currentIndividuals[b+1]->getDominatedBy())
                    {
                        shared_ptr<GeneticIndividual> ind = currentIndividuals[b];
                        currentIndividuals[b] = currentIndividuals[b+1];
                        currentIndividuals[b+1] = ind;
                    }
                }
            }

            // PRINT TO DEBUG
            for (int a=0;a<(int)currentIndividuals.size();a++)
            {
                cout << currentIndividuals[a]->getFitness() << ", " << currentIndividuals[a]->getFitness2() << ": " << currentIndividuals[a]->getDominatedBy() << endl;
                // PRINT( currentIndividuals[a]->getFitness() );
                // PRINT( currentIndividuals[a]->getFitness2() );
                // PRINT( currentIndividuals[a]->getDominatedBy() );
            }
        }

        // PRINT(currentIndividuals.size());
        // for (int i=0; i<currentIndividuals.size(); i++)
        // {
        //     PRINT(currentIndividuals[i]->getFitness());
        // }

		for (int a=lastIndex+1;a<(int)currentIndividuals.size();a++)
		{ 
			// PRINT(a);
			// PRINT(currentIndividuals.size());
			
			currentIndividuals[a]->setCanReproduce(false);
			// cout << "jmc: this code is buggy: it disallows the last n percent of offspring to reproduce, but on a randomly ordered vector: so highly fit orgs can be disallowed" << endl;
			// exit(30);
		}
		

//		// working temp tweak: seeing if I can get all children after clicking evolve to be from one org
//		#ifdef INTERACTIVELYEVOLVINGSHAPES
//			if(Globals::getSingleton()->getParameterValue("OnlyAllowElitesToBreed"))
//			{
//				for (int a=1;a<(int)currentIndividuals.size();a++)
//				{ 
//					currentIndividuals[a]->setCanReproduce(false);
//				}
//				Globals::getSingleton()->setParameterValue("OnlyAllowElitesToBreed",0.0);  //only do this once		
//			}
//			PRINT(Globals::getSingleton()->getParameterValue("OnlyAllowElitesToBreed"));
//		#endif
		
			

		
		//debugging: checking whether currentIndividuals is sorted by fitness
		cout << "IS FITNESS SORTED?  Can they reproduce?" << endl;
		for (int a=0;a<(int)currentIndividuals.size();a++)
		{ 
			cout << a << ": " << currentIndividuals[a]->getFitness() << ": "<< currentIndividuals[a]->getCanReproduce() << endl;

		}


		// //only allow selected organisms to reproduce
		// for (int a=0;a<(int)currentIndividuals.size();a++)
		// { 
		// 	if (currentIndividuals[a]->getFitness() < 500) //has not been selected by user
		// 	{
  //               cout << "test1" << endl;
		// 		currentIndividuals[a]->setCanReproduce(false);
		// 	}
			
		// }
		
		
		PRINT(offspringCount);
		
        double mutateOnlyProb = Globals::getSingleton()->getParameterValue("MutateOnlyProbability");
        for (int a=0;offspringCount>0;a++)
        {
            if (a>=1000000)
            {
                cout << "Error while making babies, need to choose the best of the species and bail!\n";
                //Something messed up, bail
                int parent = 0;
                shared_ptr<GeneticIndividual> ind = currentIndividuals[parent];
                babies.push_back(shared_ptr<GeneticIndividual>(new GeneticIndividual(ind,true,babies.size())));
                offspringCount--;
                continue;
            }

            bool onlyOneParent = (int(lastIndex)==0);

			//jmc added force cross of two selected
#ifdef INTERACTIVELYEVOLVINGSHAPES 
	    bool forceCrossIfTwoSelected = true;
#elif ENDLESSFORMS
	    bool forceCrossIfTwoSelected = true;
#else
	    bool forceCrossIfTwoSelected = false;
#endif
	    if(forceCrossIfTwoSelected and !alreadyForcedCross)
			{
				if (currentIndividuals.size()>=2) //make sure there are at least two in the species
				{
					
					double maxFitInPop =0;
					int indexOfFirstSelected = -1;
					int indexOfSecondSelected = -1;
					int numberWithTheMaxFit = 0;
					for (int z=0;z<(int)currentIndividuals.size();z++)
					{ 
						if (currentIndividuals[z]->getFitness() > maxFitInPop) //new max fit found
						{
							maxFitInPop = currentIndividuals[z]->getFitness(); //record it
							indexOfFirstSelected = z;							//store the location
							indexOfSecondSelected = -1;							//anything found previously doesn't count
							numberWithTheMaxFit = 1;							//this is the first with this fitness
						}
						else if (currentIndividuals[z]->getFitness() == maxFitInPop)
						{
							indexOfSecondSelected = z; 
							numberWithTheMaxFit++;
							
						}

					}
					if(numberWithTheMaxFit == 2)	//if only two were selected (have the max fitness)
					{
						shared_ptr<GeneticIndividual> parent1,parent2;
						parent1 = currentIndividuals[indexOfFirstSelected];
						parent2 = currentIndividuals[indexOfSecondSelected];
						babies.push_back(shared_ptr<GeneticIndividual>(new GeneticIndividual(parent1,parent2,false,babies.size()))); //create offspring that is a cross of two selected
						offspringCount--;
					}					
				}				
				alreadyForcedCross = true;	//don't do this again this time around
			}			
            else if (onlyOneParent||Globals::getSingleton()->getRandom().getRandomDouble()<mutateOnlyProb)
            {
                int parent = getIndexOfAParentViaATournament();
				PRINT(parent); //TODO delme
                shared_ptr<GeneticIndividual> ind = currentIndividuals[parent];
                babies.push_back(shared_ptr<GeneticIndividual>(new GeneticIndividual(ind,true,babies.size())));
                offspringCount--;
            }
            else	//crossover
            {
                shared_ptr<GeneticIndividual> parent1,parent2;
                //while(parent1==NULL)
                {
                    int parentIndex = getIndexOfAParentViaATournament();
                    parent1 = currentIndividuals[parentIndex];
                }
                //while(parent2==NULL)
                int tt=0;
                do
                {
                    tt++;
                    if (tt==1000000)
                    {
                        cout << "Error while choosing parents.  Doing asexual reproduction\n";
                    }

                    int parentIndex = getIndexOfAParentViaATournament();
                    parent2 = currentIndividuals[parentIndex];
                    //if(parent2==parent1)
                    //parent2=shared_ptr<GeneticIndividual>();
                }
                while (parent2==parent1&&tt<=1000000);

                if (parent1==parent2)
                {
					int boo = babies.size();
                    babies.push_back(shared_ptr<GeneticIndividual>(new GeneticIndividual(parent1,true,babies.size())));
                }
                else
                {
                    babies.push_back(shared_ptr<GeneticIndividual>(new GeneticIndividual(parent1,parent2,false, babies.size())));
                }
                offspringCount--;
            }
        }

    }
	
    int GeneticSpecies::getIndexOfAParentViaATournament()
    {
        int tournamentSize = Globals::getSingleton()->getParameterValue("TournamentSize");
		int orgIndex = 0;
		double bestFitSoFarInTourney = 0.0;
		int indexOfBestFitSoFarInTourney = 0;
		
        for (int a=0;a<tournamentSize;a++)
        {
            orgIndex = Globals::getSingleton()->getRandom().getRandomWithinRange(0,int(currentIndividuals.size()-1)); //get an index to an org in the population
			shared_ptr<GeneticIndividual> org = currentIndividuals[orgIndex];
            
			if(!org->getCanReproduce())
			{
				//cout << "not counting this org" << endl;
				a--;
				continue;
			}
			
			double orgFit = org->getFitness();
			if(orgFit > bestFitSoFarInTourney)
			{
				bestFitSoFarInTourney = orgFit;
				indexOfBestFitSoFarInTourney = orgIndex;
			}
        }

		return indexOfBestFitSoFarInTourney;
    }
	
	
	
}
