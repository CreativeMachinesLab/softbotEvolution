#ifndef HCUBE_EXPERIMENT_H_INCLUDED
#define HCUBE_EXPERIMENT_H_INCLUDED

#include "HCUBE_Defines.h"

extern int numSheets;

namespace HCUBE
{
	class Experiment
    {
    protected:
			string experimentName;
			
			bool displayGenerationResult;
			
			shared_ptr<NEAT::GeneticIndividual> lastIndividualSeen;
			
			vector<shared_ptr<NEAT::GeneticIndividual> > group;

    public:
			Experiment(string _experimentName)
			:
			experimentName(_experimentName),
			displayGenerationResult(true)
			{}
			
			virtual ~Experiment()
			{}
			
			string getExperimentName() const
			{
				return experimentName;
			}
			
			void setExperimentName(string _experimentName)
			{
				experimentName = _experimentName;
			}
			
			inline void setLastIndividualSeen(shared_ptr<NEAT::GeneticIndividual> _lastIndividualSeen)
			{
				lastIndividualSeen = _lastIndividualSeen;
			}
			
			virtual NEAT::GeneticPopulation* createInitialPopulation(int populationSize) = 0;
			
			virtual NEAT::GeneticPopulation* createInitialPopulation(shared_ptr<NEAT::GeneticPopulation> CPPN_pop, shared_ptr<Experiment>) { cerr<< "virtual createInitialPopulation was called\n"; assert(false);}
			
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
			virtual NEAT::FastBiasNetwork<double> populateAndReturnSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual) { assert(false); }
#else
			virtual NEAT::FastNetwork<double> populateAndReturnSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual) { assert(false); }
#endif
			
			
			virtual int getGroupCapacity()
			{
				#ifdef INTERACTIVELYEVOLVINGSHAPES
				return int(NEAT::Globals::getSingleton()->getParameterValue("PopulationSize"));
				#else
				return 1; //default
				#endif
			}
			
			inline int getGroupSize()
			{
				return int(group.size());
			}
			
			inline shared_ptr<NEAT::GeneticIndividual> getGroupMember(int index)
			{
				return group[index];
			}
			
			inline void clearGroup()
			{
				group.clear();
			}
			
			inline void addIndividualToGroup(shared_ptr<NEAT::GeneticIndividual> individual)
			{
				group.push_back(individual);
			}
			
			virtual void processGroup(shared_ptr<NEAT::GeneticGeneration> generation) = 0;
			
			virtual void processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
			{}
			
			inline void printGeneticPertubationEffects(shared_ptr<const NEAT::GeneticIndividual> individual, double fitness)
			{
#if PRINT_GENETIC_PERTUBATION_INFO
        
				static bool doOnce = true;
				if(doOnce) //clear the files
				{
					ofstream mutationEffects_file;        
					mutationEffects_file.open ("mutationEffects.txt", ios::trunc );
					mutationEffects_file << "Generation 0: \n"; 
					mutationEffects_file.close();
					
					ofstream mutationAndCrossoverEffects_file;        
					mutationAndCrossoverEffects_file.open ("mutationAndCrossoverEffects.txt", ios::trunc );
					mutationAndCrossoverEffects_file << "Generation 0: \n"; 
					mutationAndCrossoverEffects_file.close();
					
					ofstream crossoverEffects_file;        
					crossoverEffects_file.open ("crossoverEffects.txt", ios::trunc );
					crossoverEffects_file << "Generation 0: \n"; 
					crossoverEffects_file.close();
					
					doOnce = false;
				}
				
        
        
				//cout << "mutated at birth: ?: " << individual->getMutatedAtBirth() << endl;
				double parent1Fit = individual->getParent1Fitness();
				double parent2Fit = individual->getParent2Fitness();
        double bestFitnessInLineage = individual->getBestFitnessInLineage();
				//cerr << "par1 fit: " << parent1Fit << endl;
				//cerr << "par2 fit: " << parent2Fit << endl;
				
				if(individual->getMutatedAtBirth() and parent2Fit==-1.0){
					//printf("product of mutation only. Fitness gain: %f\n", fitnessGain);
					ofstream mutationEffects_file;        
					mutationEffects_file.open ("mutationEffects.txt", ios::app );
					mutationEffects_file << parent1Fit << " " << fitness << " " << bestFitnessInLineage << endl;
					mutationEffects_file.close();
				}
				else if(individual->getMutatedAtBirth() and parent2Fit>-1.0){
					//printf("product of mutation and crossover. \n");
					ofstream mutationAndCrossoverEffects_file;        
					mutationAndCrossoverEffects_file.open ("mutationAndCrossoverEffects.txt", ios::app );
					mutationAndCrossoverEffects_file << parent1Fit << " " << parent2Fit << " " <<  fitness << " " << bestFitnessInLineage << endl;
					mutationAndCrossoverEffects_file.close();
				}
				else if(!individual->getMutatedAtBirth() and parent2Fit>-1.0){
					//printf("product of crossover only. \n");
					ofstream crossoverEffects_file;        
					crossoverEffects_file.open ("crossoverEffects.txt", ios::app );
					crossoverEffects_file << parent1Fit << " " << parent2Fit << " " <<  fitness << " " << bestFitnessInLineage << endl;
					crossoverEffects_file.close();
				}
#endif
				
			}
			
			

			virtual bool performUserEvaluations()
			{
				return false;
			}
						
			virtual inline bool isDisplayGenerationResult()
			{
				return displayGenerationResult;
			}
			
			virtual inline void setDisplayGenerationResult(bool _displayGenerationResult)
			{
				displayGenerationResult=_displayGenerationResult;
			}
			
			virtual inline void toggleDisplayGenerationResult()
			{
				displayGenerationResult=!displayGenerationResult;
			}
			
			
			virtual Experiment* clone() = 0;
			
			virtual void resetGenerationData(shared_ptr<NEAT::GeneticGeneration> generation)
			{}
			
			virtual void addGenerationData(shared_ptr<NEAT::GeneticGeneration> generation,shared_ptr<NEAT::GeneticIndividual> individual)
			{}
			
			virtual bool converged(int) { return false; }
			
			virtual vector<double> getTargetWeights() { assert(false); }
			
			virtual double getHybrid_FTMutateOnlyProbability() const { assert(false); return -1.0; }
			virtual double getHybrid_FTMutateLinkProbability() const { assert(false); return -1.0; }
		
			virtual string getExperimentStats() { return "";}


    };
}

#endif // HCUBE_EXPERIMENT_H_INCLUDED
