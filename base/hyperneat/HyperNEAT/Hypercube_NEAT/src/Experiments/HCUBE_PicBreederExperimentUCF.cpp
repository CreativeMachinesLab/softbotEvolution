

//#define POSTHOC_RECORD
//#define PicBreeder_INTERACTIVE_EVOLUTION
//#define TEST_MUTATE
#define EUCLIDEAN_DISTANCE_FITNESS
#define DOUBLE
#ifndef HCUBE_NOGUI
//#define COLOR_MAX (16777215)
#include "Experiments/HCUBE_PicBreederExperimentUCF.h"

namespace
{
	double dist(double x, double y)
	{
		return sqrt(x*x+y*y);
	}
	
	unsigned n2c(double nodeval)
	{
		return nodeval*127;
	}
	
	void printmatrix(vector<vector<long> >  mat)
	{
		for(int i=0; i<mat.size(); i++)
		{
			for(int j=0; j<mat[i].size(); j++)
			{
				cout << mat[i][j] << ",";
			}
				
			cout<<endl;
		}
	}
	
	double normalize(const int & r_xyVal, const int & r_numNodesXorY) 
	{
		// turn the xth or yth node into its coordinates on a grid from -1 to 1, e.g. x values (1,2,3,4,5) become (-1, -.5 , 0, .5, 1)
		// this works with even numbers, and for x or y grids only 1 wide/tall, which was not the case for the original
		// e.g. see findCluster for the orignal versions where it was not a funciton and did not work with odd or 1 tall/wide #s
		double coord;
		
		double increment = 2.0/(r_numNodesXorY-1);
		
		if(r_numNodesXorY==1) coord = 0;
		else                  coord = -1+(r_xyVal*increment);
		
		return(coord);    
	}
				
}

namespace HCUBE
{
    using namespace NEAT;
	
    PicBreederExperimentUCF::PicBreederExperimentUCF(string _experimentName)
	:
	Experiment(_experimentName),
	numPixelsX(500),
	numPixelsY(500)
    {
		colval = 0;
		color = new wxColour(colval);
		pen = new wxPen(*color);
        generateSubstrate();
		
    }
	

	
	
    GeneticPopulation* PicBreederExperimentUCF::createInitialPopulation(int populationSize)
    {
        GeneticPopulation *population = new GeneticPopulation();
        vector<GeneticNodeGene> genes;
		
        genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("X","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Y","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("color_value","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
		genes.push_back(GeneticNodeGene("d","NetworkSensor",0,false));
		
		
        for (int a=0;a<populationSize;a++)
        {
            shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,true,1.0));
			
            for (int b=0;b<0;b++)
            {
                individual->testMutate();
            }
			
            population->addIndividual(individual);
        }
		
        cout << "Finished creating population\n";
        return population;
    }
	
    void PicBreederExperimentUCF::generateSubstrate()
    {
        cout << "Setting up canvas...";
		
		for (int i=0; i<numPixelsX; i++)
		{
			substrate.push_back(vector<long>(numPixelsY,0));
		}
		
		cout << "done" << endl;
	}
	
    void PicBreederExperimentUCF::populateSubstrate(shared_ptr<const NEAT::GeneticIndividual> individual)
    {
		cout<<"populateSubstrate...";
		
        NEAT::FastNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
		//drawContext->SetPen(*pen);
		
        //int linkCounter=0;
        for (int x1=0;x1<substrate.size();x1++)
        {
            for (int y1=0;y1<substrate[x1].size();y1++)
            {

				double xnormal = normalize(x1,numPixelsX);
				double ynormal = normalize(y1,numPixelsY);

				network.reinitialize();
				network.setValue("X",xnormal);
				network.setValue("Y",ynormal);

				network.setValue("Bias",0.0);
				network.setValue("d",dist(xnormal,ynormal));
				network.update();
				
				double color_value = network.getValue("color_value");
				//cout<<"color_value: "<<endl;
				
				color_value += 1;
				//cout<<color_value<<endl;
				
				//cout<< "wxcoord: ("<<x1<<","<<y1<<")  normcoord: ("<<xnormal<<","<<ynormal<<")  shade: "<<n2c(color_value)<<endl;
				
				double out = min(abs(color_value-1), 1.0);
				
				substrate[x1][y1] = n2c(out);
				
						

						
				//linkCounter++;
            }
        }
		
		cout<<"done"<<endl;
    }
	
    double PicBreederExperimentUCF::processEvaluation(
													  wxDC *drawContext
													  )
    {
		cout << "processEvaluation" <<endl;
		drawContext->SetPen(*pen);
		
		if (drawContext)drawContext->Clear();

		double fitness = 0;
		
		//substrate.reinitialize();
		//substrate.dummyActivation();
		
		//printmatrix(substrate);
		
		for(int x1=0;x1<substrate.size();x1++){
			for(int y1=0;y1<substrate[x1].size();y1++){
				
				//color->Set(255,0,0);
				unsigned colval = substrate[x1][y1];
				color->Set(colval,colval,colval);	//makes RGB values the same, and thus grayscale
				pen->SetColour(*color);
				drawContext->SetPen(*pen);
				drawContext->DrawPoint(x1,y1);
				
			}
		}
		
		
		return 5.0;
	
	}
	
    void PicBreederExperimentUCF::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
    {
		cout<<"processgroup"<<endl;
        shared_ptr<NEAT::GeneticIndividual> individual = group.front();
		
        //You get 10 points just for entering the game, wahooo!
        individual->setFitness(10);
		
        populateSubstrate(individual);
		
        shared_ptr<PicBreederStatsUCF> individualStats = shared_ptr<PicBreederStatsUCF>(new PicBreederStatsUCF());
		
        individual->setUserData(individualStats);
		
       // searchTree.getFullResults(individualStats.get(),substrate,numPixelsX,numPixelsY,nameLookup);
		
        double totalFitness;
		
        individual->reward( max(0.0,totalFitness) );
    }
	
    void PicBreederExperimentUCF::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
    {
		cout<<"\tprocessindividual"<<endl;
        //You get 10 points just for entering the game, wahooo!
        individual->setFitness(10);
		
        populateSubstrate(individual);
		
        shared_ptr<PicBreederStatsUCF> individualStats = shared_ptr<PicBreederStatsUCF>(new PicBreederStatsUCF());
		
        individual->setUserData(individualStats);
		
        //searchTree.getFullResults(individualStats.get(),substrate,numPixelsX,numPixelsY,nameLookup);
		
        double totalFitness;
        //totalFitness = individualStats->wins + individualStats->ties;
		
        individual->reward( max(0.0,totalFitness) );
    }
	
#ifndef HCUBE_NOGUI
    void PicBreederExperimentUCF::createIndividualImage(wxDC &drawContext,shared_ptr<NEAT::GeneticIndividual> individual)
    {
        //if (lastIndividualSeen!=individual)
        //{
            cout << "Repopulating substrate\n";
            populateSubstrate(individual);
            lastIndividualSeen = individual;
        //}
		
        processEvaluation(&drawContext);
    }
	
    bool PicBreederExperimentUCF::handleMousePress(wxMouseEvent& event,wxSize &bitmapSize)
    {
        wxPoint clickPoint = event.GetPosition();
        int x = clickPoint.x/20-1;
        int y = clickPoint.y/20-1;
        return true;
    }
#endif
	
    Experiment* PicBreederExperimentUCF::clone()
    {
        PicBreederExperimentUCF* experiment = new PicBreederExperimentUCF(*this);
		
        return experiment;
    }
	
    void PicBreederExperimentUCF::resetGenerationData(shared_ptr<NEAT::GeneticGeneration> generation)
    {
        generation->setUserData(shared_ptr<PicBreederStatsUCF>(new PicBreederStatsUCF()));
    }
	
    void PicBreederExperimentUCF::addGenerationData(shared_ptr<NEAT::GeneticGeneration> generation,shared_ptr<NEAT::GeneticIndividual> individual)
    {
       /* shared_ptr<PicBreederStatsUCF> PicBreederStatsUCF = static_pointer_cast<PicBreederStatsUCF>(generation->getUserData());
		
        if (individual->getUserData())
            (*PicBreederStatsUCF) += *(static_pointer_cast<PicBreederStatsUCF>(individual->getUserData()).get());
		*/
    }
	
	void PicBreederExperimentUCF::printCPPN(shared_ptr<const NEAT::GeneticIndividual> individual)
{
	cout << "Printing cppn network" << endl;
	ofstream network_file;        
	network_file.open ("CPPN-ThatSolvedTheProblem.txt", ios::trunc );
	
	NEAT::FastNetwork <double> network = individual->spawnFastPhenotypeStack<double>();        //JMC: this creates the network CPPN associated with this individual used to produce the substrate (neural network)
	
	network_file << "num links:" << network.getLinkCount() << endl;
	network_file << "num nodes:" << network.getNodeCount() << endl;
	
	int numLinks = network.getLinkCount();
	int numPixels = network.getNodeCount();
	ActivationFunction activationFunction;
	
	//print out which node corresponds to which integer (e.g. so you can translate a fromNode of 1 to "x1"  
	map<string,int> localNodeNameToIndex = *network.getNodeNameToIndex();
	for( map<string,int>::iterator iter = localNodeNameToIndex.begin(); iter != localNodeNameToIndex.end(); iter++ ) {
		network_file << (*iter).first << " is node number: " << (*iter).second << endl;
	}
	
	for (size_t a=0;a<numLinks;a++)
	{
		NetworkIndexedLink<double> link = *network.getLink(a);
		
		network_file << link.fromNode << "->" << link.toNode << " : " << link.weight << endl;
	}
	for (size_t a=0;a<numPixels;a++)
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
}

#endif