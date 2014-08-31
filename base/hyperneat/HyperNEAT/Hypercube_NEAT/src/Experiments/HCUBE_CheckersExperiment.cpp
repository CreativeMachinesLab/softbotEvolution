#include "Experiments/HCUBE_CheckersExperiment.h"

#define CHECKERS_EXPERIMENT_DEBUG (0)

#define DEBUG_CHECK_HAND_CODED_HEURISTIC (0)

#define DEBUG_USE_BOARD_EVALUATION_CACHE (1)

#define DEBUG_DUMP_BOARD_LEAF_EVALUATIONS (0)

#define DEBUG_DIRECT_LINKS (0)

#define DEBUG_USE_ABSOLUTE (1)

#define DEBUG_USE_DELTAS (0)

#define DEBUG_DO_ITERATIVE_DEEPENING (0)

#define BASE_EVOLUTION_SEARCH_DEPTH (4)

#define NEAT_SEARCH_HANDICAP (0)

#define HANDCODED_PLAYER_TESTS (1)

#define PLAY_BOTH_SIDES (0)

#define CHECKERS_PRINT_ALTERNATE_MOVES (0)

#define CHECKERS_EXPERIMENT_INTERACTIVE_PLAY (1)

#define CHECKERS_EXPERIMENT_LOG_EVALUATIONS (0)

#define CHECKERS_EXPERIMENT_PRINT_EVALUATIONS_ON_LOSS (0)

namespace HCUBE
{
    ofstream *outfile = NULL;
    extern int getmove(int b[8][8],int color, double maxtime, char str[255], int *playnow, int info, int unused, int &fromx,int &fromy,int &tox,int &toy);

    using namespace NEAT;

    CheckersExperiment::CheckersExperiment(string _experimentName)
        :
    Experiment(_experimentName),
        currentSubstrateIndex(0),
        nameLookup(65536),
        from(255,255),
        DEBUG_USE_HANDCODED_EVALUATION(0),
        DEBUG_USE_HYPERNEAT_EVALUATION(0),
        chanceToMakeSecondBestMove(0.0)
    {
        //boardEvaluationCaches[0].resize(10000);
        //boardEvaluationCaches[1].resize(10000);

        numNodesX[0] = numNodesY[0] = 8;
        numNodesX[1] = numNodesY[1] = 8;
        numNodesX[2] = numNodesY[2] = 1;

        generateSubstrate();

        resetBoard(userEvaluationBoard);
        userEvaluationRound = (0);

        //boardStateCache.reserve(10000);
    }

    GeneticPopulation* CheckersExperiment::createInitialPopulation(int populationSize)
    {
        //Make stdout unbuffered.  cliche needs that.
        //setvbuf (stdout, NULL, _IONBF, 0);

        GeneticPopulation *population = new GeneticPopulation();
        vector<GeneticNodeGene> genes;

        genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("X1","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Y1","NetworkSensor",0,false));
#if DEBUG_USE_ABSOLUTE
        genes.push_back(GeneticNodeGene("X2","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Y2","NetworkSensor",0,false));
#endif
#if DEBUG_USE_DELTAS
        genes.push_back(GeneticNodeGene("DeltaX","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("DeltaY","NetworkSensor",0,false));
#endif
        genes.push_back(GeneticNodeGene("Output_ab","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
        genes.push_back(GeneticNodeGene("Output_bc","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
#if DEBUG_DIRECT_LINKS
        genes.push_back(GeneticNodeGene("Output_ac","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
#endif
#if CHECKERS_EXPERIMENT_ENABLE_BIASES
        genes.push_back(GeneticNodeGene("Bias_b","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
        genes.push_back(GeneticNodeGene("Bias_c","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));
#endif

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

    void CheckersExperiment::generateSubstrate(int substrateNum)
    {
#if CHECKERS_EXPERIMENT_ENABLE_BIASES
        NEAT::FastBiasNetwork<CheckersNEATDatatype>* substrate;
#else
        NEAT::FastNetwork<CheckersNEATDatatype>* substrate;
#endif

        substrate = &substrates[substrateNum];

        DynamicPoolMap<Node,NEAT::NetworkNode*> nodeLookup;

        cout << "Generating substrate...";
        boost::object_pool<NEAT::NetworkNode> networkNodePool;
        boost::object_pool<NEAT::NetworkLink> networkLinkPool;

        NEAT::NetworkNode *nodes = NULL;
        NEAT::NetworkLink *links = NULL;
#if CHECKERS_EXPERIMENT_ENABLE_BIASES
        CheckersNEATDatatype *nodeBiases = NULL;
#endif

        int nodeCount =
            (numNodesX[0]*numNodesY[0]) +
            (numNodesX[1]*numNodesY[1]) +
            (numNodesX[2]*numNodesY[2]);

        //In the future the first part can be divided by 2 because half are invalid (from free squares) but it's really hard to pull off
        int linkCount =
            (numNodesX[0]*numNodesY[0]*numNodesX[1]*numNodesY[1]) +
            (numNodesX[1]*numNodesY[1]*numNodesX[2]*numNodesY[2]) +
            (numNodesX[0]*numNodesY[0]*numNodesX[2]*numNodesY[2]);

        try
        {
            cout << "Allocating " << (sizeof(NEAT::NetworkNode)*nodeCount)/1024.0 << " KB for nodes\n";
            nodes = (NEAT::NetworkNode*)malloc(nodeCount*sizeof(NEAT::NetworkNode));
            cout << "Allocating " << (sizeof(NEAT::NetworkLink)*linkCount)/1024.0 << " KB for links\n";
            links = (NEAT::NetworkLink*)malloc(linkCount*sizeof(NEAT::NetworkLink));
            cout << "Done allocating\n";
#if CHECKERS_EXPERIMENT_ENABLE_BIASES
            nodeBiases = (CheckersNEATDatatype*)malloc(nodeCount*sizeof(CheckersNEATDatatype));
#endif
        }
        catch(const std::exception &e)
        {
            cout << e.what() << endl;
            CREATE_PAUSE(string("Error!: ")+toString(__LINE__));
        }

        cout << "Clearing map...\n";

        nameLookup.clear();

        cout << "Creating nodes...\n";

        int nodeCounter=0;

        for (int z=0;z<3;z++)
        {
            for (int y1=0;y1<numNodesY[z];y1++)
            {
                for (int x1=0;x1<numNodesX[z];x1++)
                {
                    if (x1==7 && y1==7 && z==1)
                    {
                        int breakme=0;
                    }

                    int xmod = x1;
                    int ymod = y1;
                    Node node(xmod,ymod,z);
                    //cout << (y1-numNodesY/2) << '/' << (x1-numNodesX/2) << endl;
                    string name = (toString(xmod)+string("/")+toString(ymod) + string("/") + toString(z));
                    nameLookup.insert(node,name);
                    bool update;
                    if (z==0)
                    {
                        update=false; //base level nodes are constant
                    }
                    else
                    {
                        update=true;
                    }
                    new (&nodes[nodeCounter]) NetworkNode(name,update);
                    nodeLookup.insert(node,&nodes[nodeCounter]);
#if CHECKERS_EXPERIMENT_ENABLE_BIASES
                    nodeBiases[nodeCounter]=0.0;
#endif
                    if (nodeCounter>=nodeCount)
                    {
                        throw CREATE_LOCATEDEXCEPTION_INFO("Too many nodes for what was allocated!");
                    }
                    nodeCounter++;
                }
            }
        }

        cout << "Creating links...\n";

        int linkCounter=0;

        for (int z1=0;z1<3;z1++)
        {
            for (int z2=z1+1;z2<3;z2++)
            {
                for (int y1=0;y1<numNodesY[z1];y1++)
                {
                    for (int x1=0;x1<numNodesX[z1];x1++)
                    {
                        for (int y2=0;y2<numNodesY[z2];y2++)
                        {
                            for (int x2=0;x2<numNodesX[z2];x2++)
                            {
#if DEBUG_DIRECT_LINKS
#else
                                if (z1==0 && z2==2)
                                {
                                    continue;
                                }
#endif
                                if (
                                    !nodeLookup.getData(Node(x1,y1,z1)) ||
                                    !nodeLookup.getData(Node(x2,y2,z2))
                                    )
                                {
                                    throw CREATE_LOCATEDEXCEPTION_INFO("COULD NOT FIND NODE FROM LOCATION!");
                                }

                                if (linkCounter>=linkCount)
                                {
                                    throw CREATE_LOCATEDEXCEPTION_INFO("Too many links for what was allocated!");
                                }

                                NetworkNode *n1 = *nodeLookup.getData(Node(x1,y1,z1));

                                if (!n1)
                                {
                                    throw CREATE_LOCATEDEXCEPTION_INFO("OOPS");
                                }

                                int n1pos = int(n1-&nodes[0]);

                                if (n1pos>=nodeCounter)
                                {
                                    throw CREATE_LOCATEDEXCEPTION_INFO("INVALID NODE 1");
                                }

                                NetworkNode *n2 = *nodeLookup.getData(Node(x2,y2,z2));

                                if (!n2)
                                {
                                    throw CREATE_LOCATEDEXCEPTION_INFO("OOPS");
                                }

                                int n2pos = int(n2-&nodes[0]);

                                if (n2pos>=nodeCounter)
                                {
                                    throw CREATE_LOCATEDEXCEPTION_INFO("INVALID NODE 2");
                                }

                                new (&links[linkCounter]) NetworkLink(
                                    *nodeLookup.getData(Node(x1,y1,z1)),
                                    *nodeLookup.getData(Node(x2,y2,z2)),
                                    0.0
                                    );

                                if (linkCounter>=linkCount)
                                {
                                    throw CREATE_LOCATEDEXCEPTION_INFO("Too many nodes for what was allocated!");
                                }
                                linkCounter++;
                            }
                        }
                    }
                }
            }
        }

#if CHECKERS_EXPERIMENT_ENABLE_BIASES
        cout << "Creating FastBiasNetwork\n";

        (*substrate) = NEAT::FastBiasNetwork<CheckersNEATDatatype>(
            nodes,
            nodeCounter,
            links,
            linkCounter,
            nodeBiases
            );
#else
        cout << "Creating FastNetwork...";
        (*substrate) = NEAT::FastNetwork<CheckersNEATDatatype>(
            nodes,
            nodeCounter,
            links,
            linkCounter
            );
        cout << "done!\n";
#endif

        cout << "Cleaning up...";
        for (int a=0;a<nodeCounter;a++)
        {
            nodes[a].~NetworkNode();
        }

        for (int a=0;a<linkCounter;a++)
        {
            links[a].~NetworkLink();
        }
        cout << "done\n";
        flush(cout);

        free(nodes);
        free(links);
#if CHECKERS_EXPERIMENT_ENABLE_BIASES
        free(nodeBiases);
#endif
        cout << "done\n";
        flush(cout);
    }

    inline CheckersNEATDatatype convertOutputToWeight(CheckersNEATDatatype output)
    {
        if (fabs(output)>0.2)
        {
            if (output>0.0)
                output = ( ((output-0.2)/0.8)*3.0 );
            else
                output = ( ((output+0.2)/0.8)*3.0 );
        }
        else
        {
            output = (0.0);
        }
        return output;
    }

    void CheckersExperiment::populateSubstrate(
        shared_ptr<const NEAT::GeneticIndividual> individual,
        int substrateNum
        )
    {
#if CHECKERS_EXPERIMENT_ENABLE_BIASES
        NEAT::FastBiasNetwork<CheckersNEATDatatype>* substrate;
#else
        NEAT::FastNetwork<CheckersNEATDatatype>* substrate;
#endif
        if (substrateNum>=2)
        {
            throw CREATE_LOCATEDEXCEPTION_INFO("ERROR: INVALID SUBSTRATE INDEX!");
        }

        if (substrateIndividuals[substrateNum]==individual)
        {
            //Don't bother remaking the same substrate
            return;
        }

        substrateIndividuals[substrateNum]=individual;

        substrate = &substrates[substrateNum];

        //Clear the evaluation cache
        for (int a=0;a<65536;a++)
        {
            boardEvaluationCaches[substrateNum][a].clear();
        }

        //Clear the state list caches
        for (int a=0;a<65536;a++)
        {
            boardStateLists[substrateNum][BLACK][a].clear();
        }
        for (int a=0;a<65536;a++)
        {
            boardStateLists[substrateNum][WHITE][a].clear();
        }



        NEAT::FastNetwork<CheckersNEATDatatype> network = individual->spawnFastPhenotypeStack<CheckersNEATDatatype>();

        int linkCounter=0;

#if CHECKERS_EXPERIMENT_DEBUG
        double linkChecksum=0.0;
#endif

        int connectionCount=0;

        for (int z1=0;z1<3;z1++)
        {
            for (int z2=z1+1;z2<3;z2++)
            {
                for (int y1=0;y1<numNodesY[z1];y1++)
                {
                    for (int x1=0;x1<numNodesX[z1];x1++)
                    {
                        for (int y2=0;y2<numNodesY[z2];y2++)
                        {
                            for (int x2=0;x2<numNodesX[z2];x2++)
                            {
                                /*Remap the nodes to the [-1,1] domain*/
                                CheckersNEATDatatype x1normal,y1normal,x2normal,y2normal;

                                if (numNodesX[z1]>1)
                                {
                                    x1normal = -1.0 + (CheckersNEATDatatype(x1)/(numNodesX[z1]-1))*2.0;
                                }
                                else
                                {
                                    x1normal = 0.0;
                                }

                                if (numNodesY[z1]>1)
                                {
                                    y1normal = -1.0 + (CheckersNEATDatatype(y1)/(numNodesY[z1]-1))*2.0;
                                }
                                else
                                {
                                    y1normal = 0.0;
                                }

                                if (numNodesX[z2]>1)
                                {
                                    x2normal = -1.0 + (CheckersNEATDatatype(x2)/(numNodesX[z2]-1))*2.0;
                                }
                                else
                                {
                                    x2normal = 0.0;
                                }

                                if (numNodesY[z2]>1)
                                {
                                    y2normal = -1.0 + (CheckersNEATDatatype(y2)/(numNodesY[z2]-1))*2.0;
                                }
                                else
                                {
                                    y2normal = 0.0;
                                }

                                network.reinitialize();
                                network.setValue("X1",x1normal);
                                network.setValue("Y1",y1normal);
                                if (network.hasNode("X2"))
                                {
                                    network.setValue("X2",x2normal);
                                    network.setValue("Y2",y2normal);
                                }
                                if (network.hasNode("DeltaX"))
                                {
                                    network.setValue("DeltaX",x2normal-x1normal);
                                    network.setValue("DeltaY",y2normal-y1normal);
                                }

                                if(network.hasNode("Bias"))
                                {
                                    network.setValue("Bias",(CheckersNEATDatatype)0.3);
                                }

                                network.update();

                                CheckersNEATDatatype output;

                                if (z1==0 && z2==1)
                                {
                                    output = network.getValue("Output_ab");
                                }
                                else if (z1==1 && z2==2)
                                {
                                    output = network.getValue("Output_bc");
                                }
                                else if (z1==0 && z2==2)
                                {
                                    if(network.hasNode("Output_ac"))
                                    {
                                        output = network.getValue("Output_ac");
                                    }
                                    else
                                    {
                                        output = 0;
                                    }
                                }
                                else
                                {
                                    throw CREATE_LOCATEDEXCEPTION_INFO("wtf");
                                }

                                output = convertOutputToWeight(output);

                                /*
                                if (z1)
                                {
                                cout << x2normal << ',' << y2normal << endl;
                                cout << "Setting weight for "
                                << nameLookup[Node(x1-numNodesX[z1]/2,y1-numNodesY[z1]/2,z1)] << " -> "
                                << nameLookup[Node(x2-numNodesX[z2]/2,y2-numNodesY[z2]/2,z2)]
                                << endl;
                                cout << "Weight: " << output << endl;
                                cout << "Outputs: " << network.getValue("Output_ab") << ','
                                << network.getValue("Output_bc") << endl;
                                CREATE_PAUSE("");
                                }
                                */

                                //if (z1)
                                {
                                    //To account for the fact that so many nodes are merging into 1 node.
                                    //TODO: Try to find a more intuitive way to do this

                                    output /= numNodesX[z1];
                                    //output /= numNodesY[z1];


                                    //output *= numNodesX[z2];
                                    //output *= numNodesY[z2];
                                }

                                if (
                                    (nameLookup.getData(Node(x1,y1,z1))==NULL) ||
                                    (nameLookup.getData(Node(x2,y2,z2))==NULL)
                                    )
                                {
                                    throw CREATE_LOCATEDEXCEPTION_INFO("COULD NOT FIND A NODE'S NAME!");
                                }

#if DEBUG_DIRECT_LINKS
#else
                                if (! (z1==0 && z2==2) )
                                {
#endif

                                    if(fabs(output)>1e-3)
                                    {
                                        connectionCount++;
                                    }

                                    substrate->getLink(
                                        *nameLookup.getData(Node(x1,y1,z1)),
                                        *nameLookup.getData(Node(x2,y2,z2))
                                        )->weight = output;
#if CHECKERS_EXPERIMENT_DEBUG
                                    linkChecksum += output;
#endif

#if DEBUG_DIRECT_LINKS
#else
                                }
#endif

                                linkCounter++;

#if CHECKERS_EXPERIMENT_ENABLE_BIASES
                                if (x1==0&&y1==0&&z1==0)
                                {
                                    CheckersNEATDatatype nodeBias;

                                    if (z2==1)
                                    {
                                        nodeBias = network.getValue("Bias_b");

                                        nodeBias = convertOutputToWeight(nodeBias);

                                        /*{
                                        cout << "Setting bias for "
                                        << nameLookup[Node(x2-numNodesX[z2]/2,y2-numNodesY[z2]/2,1)]
                                        << endl;
                                        cout << "Bias: " << nodeBias << endl;
                                        CREATE_PAUSE("");
                                        }*/

                                        substrate->setBias(
                                            *nameLookup.getData(Node(x2,y2,z2)),
                                            nodeBias
                                            );
                                    }
                                    else if (z2==2)
                                    {
                                        nodeBias = network.getValue("Bias_c");

                                        nodeBias = convertOutputToWeight(nodeBias);

                                        /*{
                                        cout << "Setting bias for "
                                        << nameLookup[Node(x2-numNodesX[z2]/2,y2-numNodesY[z2]/2,2)]
                                        << endl;
                                        cout << "Bias: " << nodeBias << endl;
                                        CREATE_PAUSE("");
                                        }*/

                                        substrate->setBias(
                                            *nameLookup.getData(Node(x2,y2,z2)),
                                            nodeBias
                                            );
                                    }
                                    else
                                    {
                                        throw CREATE_LOCATEDEXCEPTION_INFO("wtf");
                                    }
                                }
#endif
                            }
                        }
                    }
                }
            }
        }

        //cout << "CONNECTION COUNT: " << connectionCount << endl;
    }

    CheckersNEATDatatype CheckersExperiment::processEvaluation(
        wxDC *drawContext
        )
    {
        return 5.0;
    }

    //extern int getmove(int b[8][8],int color, CheckersNEATDatatype maxtime, char str[255], int *playnow, int info, int unused, CBmove *move);
    //extern int islegal(int b[8][8], int color, int from, int to, CBmove *move);
    //extern int enginecommand(char str[256], char reply[256]);

#define MAXDEPTH 99
#define MAXMOVES 28
#define OCCUPIED 0
    //extern int  generatemovelist(int b[46], struct move2 movelist[MAXMOVES], int color);
    //extern int  generatecapturelist(int b[46], struct move2 movelist[MAXMOVES], int color);

    int gridToIndex(int x,int y)
    {
        //cout << "MAPPING: (" << x << ',' << y << ") -> ";

        //First, flip the grid vertically then horizontally

        x = 7-x;
        //y = 7-y;

        //Now, if y is even or 0, x needs to be shifted to the left by 1

        if (y%2==0)
        {
            x--;
        }

        //Now, compress x
        x/=2;

        int retval = (y*4) + x + 1;

        //cout << retval << endl;
        //CREATE_PAUSE("");

        return retval;
    }

    //For evaluation.  Positive means winning for black, and negative means
    //winning for white.
    CheckersNEATDatatype CheckersExperiment::evaluateLeafWhite(uchar b[8][8])
    {
        if (DEBUG_USE_HYPERNEAT_EVALUATION)
        {
            throw CREATE_LOCATEDEXCEPTION_INFO("WRONG TIME FOR HYPERNEAT EVALUATION!");
        }

        //cout << "FOUND A WHITE EVALUATION!\n";
        int colorToMove = WHITE;

        CheckersNEATDatatype retval=0.0;

        const int turn=2;   //color to move gets +turn
        const int brv=3;    //multiplier for back rank
        const int kcv=5;    //multiplier for kings in center
        const int mcv=1;    //multiplier for men in center

        const int mev=1;    //multiplier for men on edge
        const int kev=5;    //multiplier for kings on edge
        const int cramp=5;  //multiplier for cramp

        const int opening=-2; // multipliers for tempo
        const int midgame=-1;
        const int endgame=2;
        const int intactCheckersNEATDatatypecorner=3;

        bool printJGResults=false;

        if (colorToMove==BLACK)
        {
            retval += turn;
        }
        else
        {
            retval -= turn;
        }

        /* cramp */

        if (b[0][4]==(BLACK|MAN) && b[1][5]==(WHITE|MAN)) retval+=cramp;
        if (b[7][3]==(WHITE|MAN) && b[6][2]==(BLACK|MAN)) retval-=cramp;

        int code=0;

        /* back rank guard */

        if (b[0][0] & MAN) code++;
        if (b[2][0] & MAN) code+=2;
        if (b[4][0] & MAN) code+=4;
        if (b[6][0] & MAN) code+=8;
        switch (code)
        {
        case 0:
            code=0;
            break;
        case 1:
            code=-1;
            break;
        case 2:
            code=1;
            break;
        case 3:
            code=0;
            break;
        case 4:
            code=1;
            break;
        case 5:
            code=1;
            break;
        case 6:
            code=2;
            break;
        case 7:
            code=1;
            break;
        case 8:
            code=1;
            break;
        case 9:
            code=0;
            break;
        case 10:
            code=7;
            break;
        case 11:
            code=4;
            break;
        case 12:
            code=2;
            break;
        case 13:
            code=2;
            break;
        case 14:
            code=9;
            break;
        case 15:
            code=8;
            break;
        }
        int backrank=code;


        code=0;
        if (b[1][7] & MAN) code+=8;
        if (b[3][7] & MAN) code+=4;
        if (b[5][7] & MAN) code+=2;
        if (b[7][7] & MAN) code++;
        switch (code)
        {
        case 0:
            code=0;
            break;
        case 1:
            code=-1;
            break;
        case 2:
            code=1;
            break;
        case 3:
            code=0;
            break;
        case 4:
            code=1;
            break;
        case 5:
            code=1;
            break;
        case 6:
            code=2;
            break;
        case 7:
            code=1;
            break;
        case 8:
            code=1;
            break;
        case 9:
            code=0;
            break;
        case 10:
            code=7;
            break;
        case 11:
            code=4;
            break;
        case 12:
            code=2;
            break;
        case 13:
            code=2;
            break;
        case 14:
            code=9;
            break;
        case 15:
            code=8;
            break;
        }
        backrank-=code;
        retval+=brv*backrank;

        int value[17];
        memset(value,0,sizeof(int)*17);

        value[WHITE|MAN] = 1;
        value[WHITE|KING] = 16;

        value[BLACK|MAN] = 256;
        value[BLACK|KING] = 4096;

        code=0;
        for (int boardx=0;boardx<8;boardx++)
        {
            for (int boardy=(boardx%2);boardy<8;boardy+=2)
            {
                code += value[b[boardx][boardy]];
            }
        }

        int nwm = code % 16;
        int nwk = (code>>4) % 16;
        int nbm = (code>>8) % 16;
        int nbk = (code>>12) % 16;


        int v1=100*nbm+130*nbk;
        int v2=100*nwm+130*nwk;

        retval+=v1-v2;                       /*material values*/
        retval+=(250*(v1-v2))/(v1+v2);      /*favor exchanges if in material plus*/

        int nm=nbm+nwm;
        int nk=nbk+nwk;

        /* intact CheckersNEATDatatype corner */
        if (b[6][0]==(BLACK|MAN))
        {
            if (b[5][1]==(BLACK|MAN) || b[7][1]==(BLACK|MAN))
                retval+=intactCheckersNEATDatatypecorner;
        }

        if (b[1][7]==(WHITE|MAN))
        {
            if (b[0][6]==(WHITE|MAN) || b[2][6]==(WHITE|MAN))
                retval-=intactCheckersNEATDatatypecorner;
        }

        int nbmc=0,nbkc=0,nwmc=0,nwkc=0;

        int nbme=0,nbke=0,nwme=0,nwke=0;

        int tempo=0;

        for (int x=0;x<8;x++)
        {
            for (int y=(x%2);y<8;y+=2)
            {
                if (x>=2&&x<=5&&y>=2&&y<=5)
                {
                    /* center control */
                    if (b[x][y] == (BLACK|MAN)) nbmc++;
                    if (b[x][y] == (BLACK|KING)) nbkc++;
                    if (b[x][y] == (WHITE|MAN)) nwmc++;
                    if (b[x][y] == (WHITE|KING)) nwkc++;
                }
                else if (x==0||x==7||y==0||y==7)
                {
                    /* edge */
                    if (b[x][y] == (BLACK|MAN)) nbme++;
                    if (b[x][y] == (BLACK|KING)) nbke++;
                    if (b[x][y] == (WHITE|MAN)) nwme++;
                    if (b[x][y] == (WHITE|KING)) nwke++;
                }

                /* tempo */
                if (b[x][y] == (BLACK|MAN))
                {
                    tempo += y;
                }
                if (b[x][y] == (WHITE|MAN))
                {
                    tempo -= (7-y);
                }
            }
        }

        retval+=(nbmc-nwmc)*mcv;
        retval+=(nbkc-nwkc)*kcv;

        retval-=(nbme-nwme)*mev;
        retval-=(nbke-nwke)*kev;

        if (nm>=16) retval+=opening*tempo;
        if ((nm<=15) && (nm>=12)) retval+=midgame*tempo;
        if (nm<9) retval+=endgame*tempo;

        if (nbk+nbm>nwk+nwm && nwk<3)
        {
            if (b[0][6]== (WHITE|KING)) retval-=15;
            if (b[1][7]== (WHITE|KING)) retval-=15;
            if (b[6][0]== (WHITE|KING)) retval-=15;
            if (b[7][1]== (WHITE|KING)) retval-=15;
        }
        if (nwk+nwm>nbk+nbm && nbk<3)
        {
            if (b[0][6]== (BLACK|KING)) retval+=15;
            if (b[1][7]== (BLACK|KING)) retval+=15;
            if (b[6][0]== (BLACK|KING)) retval+=15;
            if (b[7][1]== (BLACK|KING)) retval+=15;
        }

        int stonesinsystem=0;

        /* the move */
        if (nwm+nwk-nbk-nbm==0)
        {
            if (colorToMove==BLACK)
            {
                for (int y=0;y<8;y+=2)
                {
                    for (int x=(y%2);x<8;x+=2)
                    {
                        if (b[x][y]!=EMPTY) stonesinsystem++;
                    }
                }
                if (stonesinsystem % 2)
                {
                    if (nm+nk<=12) retval++;
                    if (nm+nk<=10) retval++;
                    if (nm+nk<=8) retval+=2;
                    if (nm+nk<=6) retval+=2;
                }
                else
                {
                    if (nm+nk<=12) retval--;
                    if (nm+nk<=10) retval--;
                    if (nm+nk<=8) retval-=2;
                    if (nm+nk<=6) retval-=2;
                }
            }
            else
            {
                for (int y=1;y<8;y+=2)
                {
                    for (int x=(y%2);x<8;x+=2)
                    {
                        if (b[x][y]!=EMPTY) stonesinsystem++;
                    }
                }
                if ((stonesinsystem % 2) == 0)
                {
                    if (nm+nk<=12) retval++;
                    if (nm+nk<=10) retval++;
                    if (nm+nk<=8) retval+=2;
                    if (nm+nk<=6) retval+=2;
                }
                else
                {
                    if (nm+nk<=12) retval--;
                    if (nm+nk<=10) retval--;
                    if (nm+nk<=8) retval-=2;
                    if (nm+nk<=6) retval-=2;
                }
            }
        }


#if DEBUG_CHECK_HAND_CODED_HEURISTIC
        {
            int board[256];
            int color = colorToMove;
            int i,j;
            /* initialize board */
            for (i=0;i<46;i++)
                board[i]=OCCUPIED;
            for (i=5;i<=40;i++)
                board[i]=FREE;
            board[5]=b[0][0];
            board[6]=b[2][0];
            board[7]=b[4][0];
            board[8]=b[6][0];
            board[10]=b[1][1];
            board[11]=b[3][1];
            board[12]=b[5][1];
            board[13]=b[7][1];
            board[14]=b[0][2];
            board[15]=b[2][2];
            board[16]=b[4][2];
            board[17]=b[6][2];
            board[19]=b[1][3];
            board[20]=b[3][3];
            board[21]=b[5][3];
            board[22]=b[7][3];
            board[23]=b[0][4];
            board[24]=b[2][4];
            board[25]=b[4][4];
            board[26]=b[6][4];
            board[28]=b[1][5];
            board[29]=b[3][5];
            board[30]=b[5][5];
            board[31]=b[7][5];
            board[32]=b[0][6];
            board[33]=b[2][6];
            board[34]=b[4][6];
            board[35]=b[6][6];
            board[37]=b[1][7];
            board[38]=b[3][7];
            board[39]=b[5][7];
            board[40]=b[7][7];
            for (i=5;i<=40;i++)
                if (board[i]==0) board[i]=FREE;
            for (i=9;i<=36;i+=9)
                board[i]=OCCUPIED;
            /* board initialized */

            int eval;
            int v1,v2;
            int nbm,nbk,nwm,nwk;
            int nbmc=0,nbkc=0,nwmc=0,nwkc=0;
            int nbme=0,nbke=0,nwme=0,nwke=0;
            int code=0;
            static const int value[17]=
            {
                0,0,0,0,0,1,256,0,0,16,4096,0,0,0,0,0,0
            };
            static const int edge[14]=
            {
                5,6,7,8,13,14,22,23,31,32,37,38,39,40
            };
            static const int center[8]=
            {
                15,16,20,21,24,25,29,30
            };
            static const int row[41]=
            {
                0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,0,3,3,3,3,4,4,4,4,0,5,5,5,5,6,6,6,6,0,7,7,7,7
            };
            static const int safeedge[4]=
            {
                8,13,32,37
            };

            int tempo=0;
            int nm,nk;

            const int turn=2;   //color to move gets +turn
            const int brv=3;    //multiplier for back rank
            const int kcv=5;    //multiplier for kings in center
            const int mcv=1;    //multiplier for men in center

            const int mev=1;    //multiplier for men on edge
            const int kev=5;    //multiplier for kings on edge
            const int cramp=5;  //multiplier for cramp

            const int opening=-2; // multipliers for tempo
            const int midgame=-1;
            const int endgame=2;
            const int intactCheckersNEATDatatypecorner=3;


            int backrank;

            int stonesinsystem=0;

#ifdef STATISTICS
            evaluations++;
#endif


            for (i=5;i<=40;i++)
                code+=value[board[i]];

            nwm = code % 16;
            nwk = (code>>4) % 16;
            nbm = (code>>8) % 16;
            nbk = (code>>12) % 16;


            v1=100*nbm+130*nbk;
            v2=100*nwm+130*nwk;

            eval=v1-v2;                       /*material values*/
            eval+=(250*(v1-v2))/(v1+v2);      /*favor exchanges if in material plus*/

            nm=nbm+nwm;
            nk=nbk+nwk;
            /*--------- fine evaluation below -------------*/

            if (color == BLACK) eval+=turn;
            else eval-=turn;
            /*    (white)
            37  38  39  40
            32  33  34  35
            28  29  30  31
            23  24  25  26
            19  20  21  22
            14  15  16  17
            10  11  12  13
            5   6   7   8
            (black)   */
            /* cramp */
            if (board[23]==(BLACK|MAN) && board[28]==(WHITE|MAN)) eval+=cramp;
            if (board[22]==(WHITE|MAN) && board[17]==(BLACK|MAN)) eval-=cramp;

            /* back rank guard */

            code=0;
            if (board[5] & MAN) code++;
            if (board[6] & MAN) code+=2;
            if (board[7] & MAN) code+=4;
            if (board[8] & MAN) code+=8;
            switch (code)
            {
            case 0:
                code=0;
                break;
            case 1:
                code=-1;
                break;
            case 2:
                code=1;
                break;
            case 3:
                code=0;
                break;
            case 4:
                code=1;
                break;
            case 5:
                code=1;
                break;
            case 6:
                code=2;
                break;
            case 7:
                code=1;
                break;
            case 8:
                code=1;
                break;
            case 9:
                code=0;
                break;
            case 10:
                code=7;
                break;
            case 11:
                code=4;
                break;
            case 12:
                code=2;
                break;
            case 13:
                code=2;
                break;
            case 14:
                code=9;
                break;
            case 15:
                code=8;
                break;
            }
            backrank=code;


            code=0;
            if (board[37] & MAN) code+=8;
            if (board[38] & MAN) code+=4;
            if (board[39] & MAN) code+=2;
            if (board[40] & MAN) code++;
            switch (code)
            {
            case 0:
                code=0;
                break;
            case 1:
                code=-1;
                break;
            case 2:
                code=1;
                break;
            case 3:
                code=0;
                break;
            case 4:
                code=1;
                break;
            case 5:
                code=1;
                break;
            case 6:
                code=2;
                break;
            case 7:
                code=1;
                break;
            case 8:
                code=1;
                break;
            case 9:
                code=0;
                break;
            case 10:
                code=7;
                break;
            case 11:
                code=4;
                break;
            case 12:
                code=2;
                break;
            case 13:
                code=2;
                break;
            case 14:
                code=9;
                break;
            case 15:
                code=8;
                break;
            }
            backrank-=code;
            eval+=brv*backrank;


            /* intact CheckersNEATDatatype corner */
            if (board[8]==(BLACK|MAN))
            {
                if (board[12]==(BLACK|MAN) || board[13]==(BLACK|MAN))
                    eval+=intactCheckersNEATDatatypecorner;
            }

            if (board[37]==(WHITE|MAN))
            {
                if (board[32]==(WHITE|MAN) || board[33]==(WHITE|MAN))
                    eval-=intactCheckersNEATDatatypecorner;
            }
            /*    (white)
            37  38  39  40
            32  33  34  35
            28  29  30  31
            23  24  25  26
            19  20  21  22
            14  15  16  17
            10  11  12  13
            5   6   7   8
            (black)   */

            /* center control */
            for (i=0;i<8;i++)
            {
                if (board[center[i]] != FREE)
                {
                    if (board[center[i]] == (BLACK|MAN)) nbmc++;
                    if (board[center[i]] == (BLACK|KING)) nbkc++;
                    if (board[center[i]] == (WHITE|MAN)) nwmc++;
                    if (board[center[i]] == (WHITE|KING)) nwkc++;
                }
            }
            eval+=(nbmc-nwmc)*mcv;
            eval+=(nbkc-nwkc)*kcv;

            /*edge*/
            for (i=0;i<14;i++)
            {
                if (board[edge[i]] != FREE)
                {
                    if (board[edge[i]] == (BLACK|MAN)) nbme++;
                    if (board[edge[i]] == (BLACK|KING)) nbke++;
                    if (board[edge[i]] == (WHITE|MAN)) nwme++;
                    if (board[edge[i]] == (WHITE|KING)) nwke++;
                }
            }
            eval-=(nbme-nwme)*mev;
            eval-=(nbke-nwke)*kev;



            /* tempo */
            for (i=5;i<41;i++)
            {
                if (board[i]== (BLACK | MAN))
                    tempo+=row[i];
                if (board[i]== (WHITE | MAN))
                    tempo-=7-row[i];
            }

            if (nm>=16) eval+=opening*tempo;
            if ((nm<=15) && (nm>=12)) eval+=midgame*tempo;
            if (nm<9) eval+=endgame*tempo;



            for (i=0;i<4;i++)
            {
                if (nbk+nbm>nwk+nwm && nwk<3)
                {
                    if (board[safeedge[i]]== (WHITE|KING))
                        eval-=15;
                }
                if (nwk+nwm>nbk+nbm && nbk<3)
                {
                    if (board[safeedge[i]]==(BLACK|KING))
                        eval+=15;
                }
            }





            /* the move */
            if (nwm+nwk-nbk-nbm==0)
            {
                if (color==BLACK)
                {
                    for (i=5;i<=8;i++)
                    {
                        for (j=0;j<4;j++)
                        {
                            if (board[i+9*j]!=FREE) stonesinsystem++;
                        }
                    }
                    if (stonesinsystem % 2)
                    {
                        if (nm+nk<=12) eval++;
                        if (nm+nk<=10) eval++;
                        if (nm+nk<=8) eval+=2;
                        if (nm+nk<=6) eval+=2;
                    }
                    else
                    {
                        if (nm+nk<=12) eval--;
                        if (nm+nk<=10) eval--;
                        if (nm+nk<=8) eval-=2;
                        if (nm+nk<=6) eval-=2;
                    }
                }
                else
                {
                    for (i=10;i<=13;i++)
                    {
                        for (j=0;j<4;j++)
                        {
                            if (board[i+9*j]!=FREE) stonesinsystem++;
                        }
                    }
                    if ((stonesinsystem % 2) == 0)
                    {
                        if (nm+nk<=12) eval++;
                        if (nm+nk<=10) eval++;
                        if (nm+nk<=8) eval+=2;
                        if (nm+nk<=6) eval+=2;
                    }
                    else
                    {
                        if (nm+nk<=12) eval--;
                        if (nm+nk<=10) eval--;
                        if (nm+nk<=8) eval-=2;
                        if (nm+nk<=6) eval-=2;
                    }
                }
            }

            if (eval != retval)
            {
                printJGResults=true;
                cout << "SIMPLEAI HEURISTIC SAYS: " << eval << endl;
                cout << nbm << ',' << nbk << ',' << nwm << ',' << nwk << endl;
                cout << nbmc << ',' << nbkc << ',' << nwmc << ',' << nwkc << endl;
                cout << nbme << ',' << nbke << ',' << nwme << ',' << nwke << endl;
                cout << tempo << endl;
                cout << stonesinsystem << endl;
            }
        }
#endif

        if (printJGResults)
        {
            cout << "JG HEURISTIC SAYS: " << retval << endl;
            cout << nbm << ',' << nbk << ',' << nwm << ',' << nwk << endl;
            cout << nbmc << ',' << nbkc << ',' << nwmc << ',' << nwkc << endl;
            cout << nbme << ',' << nbke << ',' << nwme << ',' << nwke << endl;
            cout << tempo << endl;
            cout << stonesinsystem << endl;
            printBoard(b);
            CREATE_PAUSE("");
        }
#if CHECKERS_EXPERIMENT_DEBUG
        cout << "JG HEURISTIC SAYS: " << retval << endl;
        cout << nbm << ',' << nbk << ',' << nwm << ',' << nwk << endl;
        cout << nbmc << ',' << nbkc << ',' << nwmc << ',' << nwkc << endl;
        cout << nbme << ',' << nbke << ',' << nwme << ',' << nwke << endl;
        cout << tempo << endl;
        printBoard(b);
        CREATE_PAUSE("");
#endif

        switch (handCodedType)
        {
        case 0:
            {
                //FIRST CASE: NO CHANGE
            }
            break;
        case 1:
            {
                //SECOND CASE, RIGHT BIAS
                for (int x=0;x<8;x++)
                {
                    for (int y=(x%2);y<8;y+=2)
                    {
                        if (b[x][y]&BLACK)
                        {
                            retval += 0.005*x;
                        }
                        else if (b[x][y]&WHITE)
                        {
                            retval -= 0.005*x;
                        }
                    }
                }
            }
            break;
        case 2:
            {
                //THIRD CASE, LEFT BIAS
                for (int x=0;x<8;x++)
                {
                    for (int y=(x%2);y<8;y+=2)
                    {
                        if (b[x][y]&BLACK)
                        {
                            retval += 0.005*(8-x);
                        }
                        else if (b[x][y]&WHITE)
                        {
                            retval -= 0.005*(8-x);
                        }
                    }
                }
            }
            break;
        case 3:
            {
                //FOURTH CASE, BOTTOM BIAS
                for (int x=0;x<8;x++)
                {
                    for (int y=(x%2);y<8;y+=2)
                    {
                        if (b[x][y]&BLACK)
                        {
                            retval += 0.005*(y);
                        }
                        else if (b[x][y]&WHITE)
                        {
                            retval -= 0.005*(y);
                        }
                    }
                }
            }
            break;
        case 4:
            {
                //FIFTH CASE, TOP BIAS
                for (int x=0;x<8;x++)
                {
                    for (int y=(x%2);y<8;y+=2)
                    {
                        if (b[x][y]&BLACK)
                        {
                            retval += 0.005*(8-y);
                        }
                        else if (b[x][y]&WHITE)
                        {
                            retval -= 0.005*(8-y);
                        }
                    }
                }
            }
            break;
        }

        return retval;
    }

    CheckersNEATDatatype CheckersExperiment::evaluateLeafHyperNEAT(
        uchar b[8][8]
    )
    {
#if CHECKERS_EXPERIMENT_ENABLE_BIASES
        NEAT::FastBiasNetwork<CheckersNEATDatatype>* substrate;
#else
        NEAT::FastNetwork<CheckersNEATDatatype>* substrate;
#endif

        substrate = &substrates[currentSubstrateIndex];

        if (DEBUG_USE_HANDCODED_EVALUATION)
        {
            throw CREATE_LOCATEDEXCEPTION_INFO("WRONG TIME FOR HANDCODED EVALUATION");
        }
        CheckersNEATDatatype output;

#if DEBUG_USE_BOARD_EVALUATION_CACHE
        tmpboard.loadBoard(b);
        ushort hashVal = tmpboard.getShortHash();

        BoardCacheList::iterator bIterator =
            boardEvaluationCaches[currentSubstrateIndex][hashVal].begin();
        BoardCacheList::iterator bEnd =
            boardEvaluationCaches[currentSubstrateIndex][hashVal].end();

        for (;bIterator != bEnd;bIterator++)
        {
            if (bIterator->first == tmpboard)
            {
                //We have a match!
                break;
            }
        }

        if (bIterator != bEnd)
        {
            output = bIterator->second;
        }
        else
#endif
        {

            substrate->reinitialize();
            substrate->dummyActivation();

            if (substrate->hasNode("Bias"))
            {
                substrate->setValue("Bias",(CheckersNEATDatatype)0.3);
            }

            for (int y=0;y<numNodesY[0];y++)
            {
                for (int x=0;x<numNodesX[0];x++)
                {
                    if ( (x+y)%2==1 ) //ignore empty squares.
                        continue;

                    int boardx = x;
                    int boardy = y;

                    //cout << boardx << ',' << boardy << ": " << b[boardx][boardy] << endl;

                    if ( (b[boardx][boardy]&WHITE) )
                    {
                        //cout << "FOUND WHITE\n";
                        if ( (b[boardx][boardy]&KING) )
                        {
                            substrate->setValue( getNameFromNode(Node(x,y,0)) , -0.75 );
                        }
                        else if ( (b[boardx][boardy]&MAN) )
                        {
                            substrate->setValue( getNameFromNode(Node(x,y,0)) , -0.5 );
                        }
                        else
                        {
                            throw CREATE_LOCATEDEXCEPTION_INFO("Could not determine piece type (man/king)!");
                        }
                    }
                    else if ( (b[boardx][boardy]&BLACK) )
                    {
                        //cout << "FOUND BLACK\n";
                        if ( (b[boardx][boardy]&KING) )
                        {
                            substrate->setValue( getNameFromNode(Node(x,y,0)) , 0.75 );
                        }
                        else if ( (b[boardx][boardy]&MAN) )
                        {
                            substrate->setValue( getNameFromNode(Node(x,y,0)) , 0.5 );
                        }
                        else
                        {
                            throw CREATE_LOCATEDEXCEPTION_INFO("Could not determine piece type (man/king)!");
                        }
                    }
                    else
                    {
                        //cout << "FOUND NOTHING\n";
                        substrate->setValue( getNameFromNode(Node(x,y,0)) , 0.0 );
                    }

                }
            }

            substrate->update(2);
            output = substrate->getValue(getNameFromNode(Node(0,0,2)));

#if CHECKERS_EXPERIMENT_DEBUG
            static CheckersNEATDatatype prevOutput;

            if (fabs(output-prevOutput)>1e-3)
            {
                prevOutput = output;
                cout << "BOARD RATING:" << output << endl;
                CREATE_PAUSE("");
            }
#endif

#if DEBUG_USE_BOARD_EVALUATION_CACHE
            if (boardEvaluationCaches[currentSubstrateIndex][hashVal].size()<10000)
            {
                boardEvaluationCaches[currentSubstrateIndex][hashVal].push_back(
                    BoardCachePair(tmpboard,output)
                    );
            }
#endif
        }

#if DEBUG_DUMP_BOARD_LEAF_EVALUATIONS
        cout << "Printing board leaf evaluations\n";

        int whiteMen,blackMen,whiteKings,blackKings;
        countPieces(b,whiteMen,blackMen,whiteKings,blackKings);

        *outfile << "Info for board below: WM:" << whiteMen << " WK:" << whiteKings
            << " BM:" << blackMen << " BK:" << blackKings << endl;
        for (int y=0;y<8;y++)
        {
            for (int x=7;x>=0;x--)
            {
                if ((x+y)%2==1)
                {
                    *outfile << " ";
                }
                else if (b[x][y]&FREE)
                {
                    *outfile << " ";
                }
                else if (b[x][y]&BLACK)
                {
                    if (b[x][y]&MAN)
                    {
                        *outfile << "b";
                    }
                    else
                    {
                        *outfile << "B";
                    }
                }
                else if (b[x][y]&WHITE)
                {
                    if (b[x][y]&MAN)
                    {
                        *outfile << "w";
                    }
                    else
                    {
                        *outfile << "W";
                    }
                }
                else
                {
                    *outfile << "_";
                }
            }
            *outfile << endl;
        }
        *outfile << "BOARD RATING: " << output << endl;

        *outfile << "Hidden layer values:\n";
        for (int y=0;y<numNodesY[1];y++)
        {
            for (int x=0;x<numNodesX[1];x++)
            {
                if (x)
                {
                    *outfile << ", ";
                }

                *outfile << setprecision(3) << substrate->getValue( getNameFromNode(Node(x,y,1)) );
            }
            *outfile << endl;
        }

        *outfile << endl;
#endif
        return output;
    }

    class CheckersBoardMoveState
    {
    public:
        CheckersNEATDatatype parentBeta;
        int depth,maxDepth;
        CheckersBoardMoveState()
        {}
        CheckersBoardMoveState(
            CheckersNEATDatatype _parentBeta,
            int _depth,
            int _maxDepth
            )
            :
        parentBeta(_parentBeta),
            depth(_depth),
            maxDepth(_maxDepth)
        {}
    };

    CheckersNEATDatatype CheckersExperiment::evaluatemax(uchar b[8][8],  CheckersNEATDatatype parentBeta, int depth,int maxDepth)
    {
        if (depth==0)
        {
            totalMoveList.clear();
#if DEBUG_DUMP_BOARD_LEAF_EVALUATIONS
            cout << "Creating new outfile\n";
            if (outfile) delete outfile;
            outfile = new ofstream("BoardEvaluations.txt");
#endif

        }

        int moveListCount;

        CheckersNEATDatatype alpha=CheckersNEATDatatype(INT_MIN);

        bool foundJump;
        int bestMoveSoFarIndex=-1;

		int oldMoveListSize = (int)totalMoveList.size();

#if DEBUG_DO_ITERATIVE_DEEPENING
        bool useCachedTable=false;

        CheckersBoardState bState(b,BLACK);

        if (depth==0)
        {
        }
        else
        {
            vector<BoardStatePair> &bsVector = boardStateLists[currentSubstrateIndex][BLACK][bState.getShortHash()];
            vector<BoardStatePair>::iterator stateIterator = bsVector.begin();
            vector<BoardStatePair>::iterator stateEnd = bsVector.end();

            for (;stateIterator != stateEnd;stateIterator++)
            {
                if (stateIterator->first==bState)
                {
                    //We have a match
                    int depthSearched =
                        stateIterator->second.maxDepth - stateIterator->second.depth;

                    int depthToSearch = maxDepth - depth;

                    if (depthSearched >= depthToSearch)
                    {
                        //We've been down this path before (or we are currently
                        //going down it in another branch of the tree),
                        //just return the best score
                        if (stateIterator->second.getNumMoves()==0)
                        {
                            //No moves possible, return the min score
                            return INT_MIN/2;
                        }

                        return stateIterator->second.getHighestScore();
                    }
                    else
                    {
                        //We've been down this path before, but not to this level of detail.
                        //Use the previous alpha values, but continue
                        useCachedTable=true;

                        moveListCount = stateIterator->second.getNumMoves();

                        /* FOR evaluatemin() FUNCTION!
                        int index=0;
                        for(int a=(moveListCount-1);a>=0;a--)
                        {
                        //We have to iterate backwards because they are sorted lowest
                        //to highest score
                        moveList[index] = stateIterator->second.moves.getIndexData[a];
                        index++;
                        }
                        */

                        for (int a=0;a<moveListCount;a++)
                        {
                            totalMoveList.push_back(stateIterator->second.getMove(a));
                        }

                        foundJump = stateIterator->second.foundJump;

                        //Erase this, because it will get replaced.
                        bsVector.erase(stateIterator);

                        //Break out of the loop so we do not keep searching
                        //(especially because the vector has now been invalidated with
                        //the erase)
                        break;
                    }
                }
            }
        }
#endif

#if DEBUG_DO_ITERATIVE_DEEPENING
        if (!useCachedTable)
#endif
        {
            moveListCount = generateMoveList(totalMoveList,int(totalMoveList.size()),b,BLACK,foundJump);
        }
#if DEBUG_DO_ITERATIVE_DEEPENING
        else
        {
#if CHECKERS_DEBUG
            //Run a test to cross-verify moves
            HCUBE::CheckersMove tmpMoves[128];
            bool tmpFoundJump;

            int tmpMoveListCount = generateMoveList(b,tmpMoves,BLACK,tmpFoundJump);

            if (moveListCount != tmpMoveListCount)
            {
                for (int b=0;b<moveListCount;b++)
                {
                    cout << (int)moveList[b].from.x << ',' << (int)moveList[b].from.y << " -> ";
                    cout << (int)moveList[b].to.x << ',' << (int)moveList[b].to.y << endl;
                }

                cout << "***" << endl;

                for (int a=0;a<tmpMoveListCount;a++)
                {
                    cout << (int)tmpMoves[a].from.x << ',' << (int)tmpMoves[a].from.y << " -> ";
                    cout << (int)tmpMoves[a].to.x << ',' << (int)tmpMoves[a].to.y << endl;
                }

                throw CREATE_LOCATEDEXCEPTION_INFO("FOUND A DIFFERING NUMBER OF MOVES!");
            }

            if (foundJump != tmpFoundJump)
            {
                throw CREATE_LOCATEDEXCEPTION_INFO("FOUND A DIFFERING JUMP VALUE!");
            }

            for (int a=0;a<moveListCount;a++)
            {
                for (int b=0;b<=moveListCount;b++)
                {
                    if (b==moveListCount)
                    {
                        cout << tmpMoves[a].from.x << ',' << tmpMoves[a].from.y << endl;
                        cout << tmpMoves[a].to.x << ',' << tmpMoves[a].to.y << endl;

                        cout << "***" << endl;

                        for (int b=0;b<moveListCount;b++)
                        {
                            cout << moveList[b].from.x << ',' << moveList[b].from.y << endl;
                            cout << moveList[b].to.x << ',' << moveList[b].to.y << endl;

                        }

                        throw CREATE_LOCATEDEXCEPTION_INFO("ERROR! MISSING A MOVE!");
                    }

                    if (moveList[b]==tmpMoves[a])
                    {
                        break;
                    }
                }
            }
#endif
        }
#endif

#if DEBUG_DO_ITERATIVE_DEEPENING
        //We have a movelist, let's cache it.
        CheckersBoardStateData bStateData;

        bStateData.depth = depth;
        bStateData.maxDepth = maxDepth;
        bStateData.foundJump = foundJump;

        /*
        for(int a=0;a<moveListCount;a++)
        {
        bStateData.moves.insert(CheckersNEATDatatype(INT_MIN/2.0),moveList[a]);
        }

        boardStateLists[currentSubstrateIndex][BLACK][bState.getShortHash()].push_back(
        BoardStatePair(bState,bStateData)
        );
        */
#endif

        if (!moveListCount)
        {
            //No possible moves, this is a loss!
            return CheckersNEATDatatype(INT_MIN/2.0);
        }

        if (depth==0 && moveListCount==1)
        {
            //Forced move, don't bother doing any evaluations
            secondBestMoveToMake = moveToMake = totalMoveList.at(oldMoveListSize);
            totalMoveList.erase(totalMoveList.begin()+oldMoveListSize,totalMoveList.end());
            return 0;
        }

#if 0
        //Check for duplicate moves
        for (int a=0;a<moveListCount;a++)
        {
            for (int b=0;b<moveListCount;b++)
            {
                if (a==b)
                    continue;

                if (moveList[a]==moveList[b])
                {
                    throw CREATE_LOCATEDEXCEPTION_INFO("OOPS!");
                }
            }
        }
#endif


#if CHECKERS_EXPERIMENT_DEBUG
        printBoard(b);
        cout << "Moves for black: " << endl;
        for (int a=0;a<moveListCount;a++)
        {
            cout << "MOVE: (" << ((int)moveList[a].from.x) << ',' << ((int)moveList[a].from.y) << ") -> "
                << "(" << ((int)moveList[a].to.x) << ',' << ((int)moveList[a].to.y) << ")" << endl;
        }
        CREATE_PAUSE("Done listing moves");
#endif

        if (depth==maxDepth)
        {
            //This is a leaf node, return the neural network's evaluation
            totalMoveList.erase(totalMoveList.begin()+oldMoveListSize,totalMoveList.end());
            return evaluateLeafHyperNEAT(b);
        }

        CheckersNEATDatatype childBeta;

        if (depth==0)
        {
            secondBestMoveToMake = moveToMake = totalMoveList.at(oldMoveListSize);
            childBetaForSecondBestMove = (CheckersNEATDatatype)(INT_MIN/2.0);
        }

        for (int a=0;a<moveListCount;a++)
        {
            CheckersMove currentMove = totalMoveList.at(oldMoveListSize+a);

            makeMove(currentMove,b);

            int winner = getWinner(b);

            if (winner==BLACK)
            {
                //CREATE_PAUSE("FOUND WIN FOR BLACK!");
                reverseMove(currentMove,b);

                if (depth==0)
                    secondBestMoveToMake = moveToMake = currentMove;

				totalMoveList.erase(totalMoveList.begin()+oldMoveListSize,totalMoveList.end());
                return CheckersNEATDatatype(INT_MAX/2);
            }

            childBeta = evaluatemin(b,alpha,depth+1,maxDepth);
            reverseMove(currentMove,b);

#if DEBUG_DO_ITERATIVE_DEEPENING
            bStateData.insertMove(childBeta,currentMove);
#endif

#if CHECKERS_EXPERIMENT_DEBUG
            for (int dd=0;dd<depth;dd++)
            {
                cout << "*";
            }
            cout << childBeta << endl;
#endif

            if (childBeta > alpha)
            {
#if CHECKERS_EXPERIMENT_DEBUG
                cout << "Found new alpha\n";
#endif
                if (depth==0)
                {
                    secondBestMoveToMake = moveToMake;
                    childBetaForSecondBestMove = alpha;
                }

                alpha = childBeta;
                bestMoveSoFarIndex = a;
                if (depth==0)
                {
                    //This means that this is the root max, so store the best move.
#if CHECKERS_EXPERIMENT_DEBUG
                    cout << "BLACK: MOVE_TO_MAKE: (" << moveList[a].from.x << ',' << moveList[a].from.y << ") -> "
                        << "(" << moveList[a].to.x << ',' << moveList[a].to.y << ")" << endl;
                    CREATE_PAUSE("SETTING MOVE_TO_MAKE");
#endif

                    moveToMake = currentMove;
                }
                else
                {
                    if (parentBeta <= childBeta)
                    {
#if CHECKERS_EXPERIMENT_DEBUG
                        cout << "Parent will never choose this path\n";
                        CREATE_PAUSE("");
#endif
                        //parent will never choose this alpha

                        //because we are ending prematurely, we have to fill the rest of the cached
                        //data

#if DEBUG_DO_ITERATIVE_DEEPENING
                        for (int restOfList=(a+1);restOfList<moveListCount;restOfList++)
                        {
                            bStateData.insertMove(CheckersNEATDatatype(INT_MIN/2.0),totalMoveList.at(oldMoveListSize+restOfList));
                        }

                        if (memcmp(bState.b,b,sizeof(uchar)*8*8))
                        {
                            throw CREATE_LOCATEDEXCEPTION_INFO("BOARD STATE CHANGED SOMEHOW!");
                        }

                        boardStateLists[currentSubstrateIndex][BLACK][bState.getShortHash()].push_back(
                            BoardStatePair(bState,bStateData)
                            );
#endif

						totalMoveList.erase(totalMoveList.begin()+oldMoveListSize,totalMoveList.end());
                        return alpha;
                    }
                }
            }
            else
            {
                if (depth==0 && childBeta>childBetaForSecondBestMove)
                {
                    secondBestMoveToMake = currentMove;
                    childBetaForSecondBestMove = childBeta;
                }
            }
        }

#if DEBUG_DO_ITERATIVE_DEEPENING
        ushort shortHash = bState.getShortHash();
        boardStateLists[currentSubstrateIndex][BLACK][shortHash].push_back(
            BoardStatePair(bState,bStateData)
            );
#endif

        totalMoveList.erase(totalMoveList.begin()+oldMoveListSize,totalMoveList.end());
        return alpha;
    }

    CheckersNEATDatatype CheckersExperiment::evaluatemin(uchar b[8][8],  CheckersNEATDatatype parentAlpha, int depth,int maxDepth)
    {
        if (depth==0)
        {
            totalMoveList.clear();
#if DEBUG_DUMP_BOARD_LEAF_EVALUATIONS
            cout << "Creating new outfile\n";
            if (outfile) delete outfile;
            outfile = new ofstream("BoardEvaluations.txt");
#endif

        }

        int moveListCount;

        CheckersNEATDatatype beta=CheckersNEATDatatype(INT_MAX);

        bool foundJump;
        int bestMoveSoFarIndex=-1;

		int oldMoveListSize = (int)totalMoveList.size();

#if DEBUG_DO_ITERATIVE_DEEPENING
        bool useCachedTable=false;

        CheckersBoardState bState(b,WHITE);

        if (depth==0)
        {
        }
        else
        {
            vector<BoardStatePair> &bsVector = boardStateLists[currentSubstrateIndex][WHITE][bState.getShortHash()];
            vector<BoardStatePair>::iterator stateIterator = bsVector.begin();
            vector<BoardStatePair>::iterator stateEnd = bsVector.end();

            for (;stateIterator != stateEnd;stateIterator++)
            {
                if (stateIterator->first==bState)
                {
                    //We have a match
                    int depthSearched =
                        stateIterator->second.maxDepth - stateIterator->second.depth;

                    int depthToSearch = maxDepth - depth;

                    if (depthSearched >= depthToSearch)
                    {
                        //We've been down this path before (or we are currently
                        //going down it in another branch of the tree),
                        //just return the best score
                        if (stateIterator->second.getNumMoves()==0)
                        {
                            //No moves possible, return the worst score
                            return CheckersNEATDatatype(INT_MAX/2.0);
                        }

                        return stateIterator->second.getLowestScore();
                    }
                    else
                    {
                        //We've been down this path before, but not to this level of detail.
                        //Use the previous alpha values, but continue
                        useCachedTable=true;

                        moveListCount = stateIterator->second.getNumMoves();

                        /* FOR evaluatemin() FUNCTION!
                        int index=0;
                        for(int a=(moveListCount-1);a>=0;a--)
                        {
                        //We have to iterate backwards because they are sorted lowest
                        //to highest score
                        moveList[index] = stateIterator->second.moves.getIndexData[a];
                        index++;
                        }
                        */

                        for (int a=0;a<moveListCount;a++)
                        {
                            totalMoveList.push_back(stateIterator->second.getMove(a));
                        }

                        foundJump = stateIterator->second.foundJump;

                        //Erase this, because it will get replaced.
                        bsVector.erase(stateIterator);

                        //Break out of the loop so we do not keep searching
                        //(especially because the vector has now been invalidated with
                        //the erase)
                        break;
                    }
                }
            }
        }
#endif

#if DEBUG_DO_ITERATIVE_DEEPENING
        if (!useCachedTable)
#endif
        {
            moveListCount = generateMoveList(totalMoveList,int(totalMoveList.size()),b,WHITE,foundJump);
        }
#if DEBUG_DO_ITERATIVE_DEEPENING
        else
        {
#if CHECKERS_DEBUG
            //Run a test to cross-verify moves
            HCUBE::CheckersMove tmpMoves[128];
            bool tmpFoundJump;

            int tmpMoveListCount = generateMoveList(b,tmpMoves,WHITE,tmpFoundJump);

            if (moveListCount != tmpMoveListCount)
            {
                for (int b=0;b<moveListCount;b++)
                {
                    cout << (int)moveList[b].from.x << ',' << (int)moveList[b].from.y << " -> ";
                    cout << (int)moveList[b].to.x << ',' << (int)moveList[b].to.y << endl;
                }

                cout << "***" << endl;

                for (int a=0;a<tmpMoveListCount;a++)
                {
                    cout << (int)tmpMoves[a].from.x << ',' << (int)tmpMoves[a].from.y << " -> ";
                    cout << (int)tmpMoves[a].to.x << ',' << (int)tmpMoves[a].to.y << endl;
                }

                throw CREATE_LOCATEDEXCEPTION_INFO("FOUND A DIFFERING NUMBER OF MOVES!");
            }

            if (foundJump != tmpFoundJump)
            {
                throw CREATE_LOCATEDEXCEPTION_INFO("FOUND A DIFFERING JUMP VALUE!");
            }

            for (int a=0;a<moveListCount;a++)
            {
                for (int b=0;b<=moveListCount;b++)
                {
                    if (b==moveListCount)
                    {
                        cout << tmpMoves[a].from.x << ',' << tmpMoves[a].from.y << endl;
                        cout << tmpMoves[a].to.x << ',' << tmpMoves[a].to.y << endl;

                        cout << "***" << endl;

                        for (int b=0;b<moveListCount;b++)
                        {
                            cout << moveList[b].from.x << ',' << moveList[b].from.y << endl;
                            cout << moveList[b].to.x << ',' << moveList[b].to.y << endl;

                        }

                        throw CREATE_LOCATEDEXCEPTION_INFO("ERROR! MISSING A MOVE!");
                    }

                    if (moveList[b]==tmpMoves[a])
                    {
                        break;
                    }
                }
            }
#endif
        }
#endif

#if DEBUG_DO_ITERATIVE_DEEPENING
        //We have a movelist, let's cache it.
        CheckersBoardStateData bStateData;

        bStateData.depth = depth;
        bStateData.maxDepth = maxDepth;
        bStateData.foundJump = foundJump;

        /*
        for(int a=0;a<moveListCount;a++)
        {
        bStateData.moves.insert(CheckersNEATDatatype(INT_MIN/2.0),moveList[a]);
        }

        boardStateLists[currentSubstrateIndex][BLACK][bState.getShortHash()].push_back(
        BoardStatePair(bState,bStateData)
        );
        */
#endif

        if (!moveListCount)
        {
            //No possible moves, this is a loss!
            return CheckersNEATDatatype(INT_MAX/2.0);
        }

        if (depth==0 && moveListCount==1)
        {
            //Forced move, don't bother doing any evaluations
            secondBestMoveToMake = moveToMake = totalMoveList.at(oldMoveListSize);
            totalMoveList.erase(totalMoveList.begin()+oldMoveListSize,totalMoveList.end());
            return 0;
        }

#if 0
        //Check for duplicate moves
        for (int a=0;a<moveListCount;a++)
        {
            for (int b=0;b<moveListCount;b++)
            {
                if (a==b)
                    continue;

                if (moveList[a]==moveList[b])
                {
                    throw CREATE_LOCATEDEXCEPTION_INFO("OOPS!");
                }
            }
        }
#endif

#if CHECKERS_EXPERIMENT_DEBUG
        printBoard(b);
        cout << "Moves for black: " << endl;
        for (int a=0;a<moveListCount;a++)
        {
            cout << "MOVE: (" << ((int)moveList[a].from.x) << ',' << ((int)moveList[a].from.y) << ") -> "
                << "(" << ((int)moveList[a].to.x) << ',' << ((int)moveList[a].to.y) << ")" << endl;
        }
        CREATE_PAUSE("Done listing moves");
#endif

        if (depth==maxDepth)
        {
            //This is a leaf node, return the hand-coded evaluation
            totalMoveList.erase(totalMoveList.begin()+oldMoveListSize,totalMoveList.end());
            return evaluateLeafWhite(b);
        }

        CheckersNEATDatatype childAlpha;

        if (depth==0)
        {
            secondBestMoveToMake = moveToMake = totalMoveList.at(oldMoveListSize);
            childAlphaForSecondBestMove = (CheckersNEATDatatype)(INT_MAX/2.0);
        }

        for (int a=0;a<moveListCount;a++)
        {
            CheckersMove currentMove = totalMoveList.at(oldMoveListSize+a);

            makeMove(currentMove,b);

            int winner = getWinner(b);

            if (winner==WHITE)
            {
                //CREATE_PAUSE("FOUND WIN FOR WHITE!");
                reverseMove(currentMove,b);

                if (depth==0)
                    secondBestMoveToMake = moveToMake = currentMove;

				totalMoveList.erase(totalMoveList.begin()+oldMoveListSize,totalMoveList.end());
                return CheckersNEATDatatype(INT_MIN/2);
            }

            childAlpha = evaluatemax(b,beta,depth+1,maxDepth);
            reverseMove(currentMove,b);

#if DEBUG_DO_ITERATIVE_DEEPENING
            bStateData.insertMove(childAlpha,currentMove);
#endif

#if CHECKERS_EXPERIMENT_DEBUG
            for (int dd=0;dd<depth;dd++)
            {
                cout << "*";
            }
            cout << childAlpha << endl;
#endif

            if (childAlpha < beta)
            {
#if CHECKERS_EXPERIMENT_DEBUG
                cout << "Found new beta\n";
#endif
                if (depth==0)
                {
                    //Set the second best move to the old best move
                    secondBestMoveToMake = moveToMake;
                    childAlphaForSecondBestMove = beta;
                }

                beta = childAlpha;
                bestMoveSoFarIndex = a;
                if (depth==0)
                {
                    //This means that this is the root max, so store the best move.
#if CHECKERS_EXPERIMENT_DEBUG
                    cout << "WHITE: MOVE_TO_MAKE: (" << moveList[a].from.x << ',' << moveList[a].from.y << ") -> "
                        << "(" << moveList[a].to.x << ',' << moveList[a].to.y << ")" << endl;
                    CREATE_PAUSE("SETTING MOVE_TO_MAKE");
#endif

                    moveToMake = currentMove;
                }
                else
                {
                    if (parentAlpha >= childAlpha)
                    {
#if CHECKERS_EXPERIMENT_DEBUG
                        cout << "Parent will never choose this path\n";
                        CREATE_PAUSE("");
#endif
                        //parent will never choose this alpha

                        //because we are ending prematurely, we have to fill the rest of the cached
                        //data

#if DEBUG_DO_ITERATIVE_DEEPENING
                        for (int restOfList=(a+1);restOfList<moveListCount;restOfList++)
                        {
                            bStateData.insertMove(CheckersNEATDatatype(INT_MAX/2.0),totalMoveList.at(oldMoveListSize+restOfList));
                        }

                        if (memcmp(bState.b,b,sizeof(uchar)*8*8))
                        {
                            throw CREATE_LOCATEDEXCEPTION_INFO("BOARD STATE CHANGED SOMEHOW!");
                        }

                        boardStateLists[currentSubstrateIndex][WHITE][bState.getShortHash()].push_back(
                            BoardStatePair(bState,bStateData)
                            );
#endif

						totalMoveList.erase(totalMoveList.begin()+oldMoveListSize,totalMoveList.end());
                        return beta;
                    }
                }
            }
            else
            {
                if (depth==0 && childAlpha<childAlphaForSecondBestMove)
                {
                    secondBestMoveToMake = currentMove;
                    childAlphaForSecondBestMove = childAlpha;
                }
            }
        }

#if DEBUG_DO_ITERATIVE_DEEPENING
        ushort shortHash = bState.getShortHash();
        boardStateLists[currentSubstrateIndex][WHITE][shortHash].push_back(
            BoardStatePair(bState,bStateData)
            );
#endif

        totalMoveList.erase(totalMoveList.begin()+oldMoveListSize,totalMoveList.end());
        return beta;
    }

    CheckersNEATDatatype CheckersExperiment::firstevaluatemax(
        uchar b[8][8],
        int maxDepth,
        double timeLimit
        )
    {
#if DEBUG_DO_ITERATIVE_DEEPENING
        /*
        for(int a=0;a<65536;a++)
        {
        boardStateLists[currentSubstrateIndex][BLACK][a].clear();
        }
        for(int a=0;a<65536;a++)
        {
        boardStateLists[currentSubstrateIndex][WHITE][a].clear();
        }
        */

        int useOdd = maxDepth%2;
        CheckersNEATDatatype retval;
        timer t;
        for (int a=2-useOdd;;a+=2)
        {
            retval = evaluatemax(b,CheckersNEATDatatype(INT_MAX/2.0),0,a);

            if (a+2>maxDepth || t.elapsed()>timeLimit)
            {
                //cout << "Ran to depth " << a << " (" << t.elapsed() << " sec.)" << endl;
                return retval;
            }
        }
#else
        //Just pass it on
        return evaluatemax(b,CheckersNEATDatatype(INT_MAX/2),0,maxDepth);
#endif
    }

    CheckersNEATDatatype CheckersExperiment::firstevaluatemin(
        uchar b[8][8],
        int maxDepth,
        double timeLimit
        )
    {
#if DEBUG_DO_ITERATIVE_DEEPENING
        /*
        for(int a=0;a<65536;a++)
        {
        boardStateLists[currentSubstrateIndex][BLACK][a].clear();
        }
        for(int a=0;a<65536;a++)
        {
        boardStateLists[currentSubstrateIndex][WHITE][a].clear();
        }
        */

        int useOdd = ((maxDepth)%2);
        CheckersNEATDatatype retval;
        timer t;
        for (int a=2-useOdd;;a+=2)
        {
            retval = evaluatemin(b,CheckersNEATDatatype(INT_MIN/2.0),0,a);

            if (a+2>maxDepth || t.elapsed()>timeLimit)
            {
                //cout << "Ran to depth " << a << " (" << t.elapsed() << " sec.)" << endl;
                return retval;
            }
        }
#else
        //Just pass it on
        return evaluatemin(b,INT_MIN/2,0,maxDepth);
#endif
    }

    void CheckersExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation)
    {
        //cout << "Processing evaluation...\n";
        shared_ptr<NEAT::GeneticIndividual> individual = group.front();
        //You get 10 points just for entering the game, wahooo!
        individual->setFitness(10);

        populateSubstrate(individual);

        /*
        gameinfo gi;

        // init gameinfo
        memset (&gi, 0, sizeof (gi));

        gi.experiment = this;
        gi.result = UNKNOWN;

        gi.player = "HyperNEAT";

        // check opponent
        gi.opponent = "simplech";
        */

        uchar b[8][8];

        //cout << "Playing games with HyperNEAT as black\n";
        for (handCodedType=0;handCodedType<HANDCODED_PLAYER_TESTS;handCodedType++)
        {

            resetBoard(b);

            int retval=-1;
            int rounds=0;

            double inGameFitness=0.0;

            for (rounds=0;rounds<CHECKERS_MAX_ROUNDS&&retval==-1;rounds++)
            {
                //cout << "Round: " << rounds << endl;
                moveToMake = CheckersMove();
                secondBestMoveToMake = CheckersMove();
                DEBUG_USE_HANDCODED_EVALUATION = 0;
                DEBUG_USE_HYPERNEAT_EVALUATION = 1;
                currentSubstrateIndex=0;
                firstevaluatemax(b,BASE_EVOLUTION_SEARCH_DEPTH+NEAT_SEARCH_HANDICAP);

#if CHECKERS_PRINT_ALTERNATE_MOVES
                printBoard(b);
                cout << "EVAL MAX:\n";
                cout << "MOVE TO MAKE: "
                    << (int)moveToMake.from.x << ','
                    << (int)moveToMake.from.y
                    << " -> " << (int)moveToMake.getFinalDestination().x << ','
                    << (int)moveToMake.getFinalDestination().y << endl;
                cout << "ALTERNATE MOVE TO MAKE: "
                    << (int)secondBestMoveToMake.from.x << ','
                    << (int)secondBestMoveToMake.from.y
                    << " -> " << (int)secondBestMoveToMake.getFinalDestination().x << ','
                    << (int)secondBestMoveToMake.getFinalDestination().y << endl;
                CREATE_PAUSE("");
#endif

#if CHECKERS_EXPERIMENT_DEBUG
                cout << "BLACK MAKING MOVE\n";

                printBoard(b);
#endif

                if (moveToMake.from.x==255)
                {
                    //black loses
                    cout << "BLACK LOSES!\n";
                    retval = WHITE;
                }
                else
                {
                    makeMove(moveToMake,b);
                    retval = getWinner(b,WHITE);
                }

#if CHECKERS_EXPERIMENT_LOG_EVALUATIONS
                memcpy(gameLog[rounds*2],b,sizeof(uchar)*8*8);
#endif

#if CHECKERS_EXPERIMENT_DEBUG
                printBoard(b);
                CREATE_PAUSE("");
#endif

                if (retval==-1)
                {
                    //printBoard(b);

                    moveToMake = CheckersMove();
                    secondBestMoveToMake = CheckersMove();

                    {
                        //progress_timer t;
                        DEBUG_USE_HANDCODED_EVALUATION = 1;
                        DEBUG_USE_HYPERNEAT_EVALUATION = 0;
                        currentSubstrateIndex=handCodedAISubstrateIndex;
                        firstevaluatemin(b,BASE_EVOLUTION_SEARCH_DEPTH);
                        //cout << "SimpleCheckers time: ";
                    }

#if CHECKERS_PRINT_ALTERNATE_MOVES
                    printBoard(b);
                    cout << "EVAL MIN:\n";
                    cout << "MOVE TO MAKE: "
                        << (int)moveToMake.from.x << ','
                        << (int)moveToMake.from.y
                        << " -> " << (int)moveToMake.getFinalDestination().x << ','
                        << (int)moveToMake.getFinalDestination().y << endl;
                    cout << "ALTERNATE MOVE TO MAKE: "
                        << (int)secondBestMoveToMake.from.x << ','
                        << (int)secondBestMoveToMake.from.y
                        << " -> " << (int)secondBestMoveToMake.getFinalDestination().x << ','
                        << (int)secondBestMoveToMake.getFinalDestination().y << endl;
                    CREATE_PAUSE("");
#endif

#if CHECKERS_EXPERIMENT_DEBUG
                    cout << "WHITE MAKING MOVE\n";

                    printBoard(b);
#endif

                    if (moveToMake.from.x==255)
                    {
                        //white loses
                        cout << "WHITE LOSES BECAUSE THERE'S NO MOVES LEFT!\n";
                        retval = BLACK;
                        //printBoard(b);
                        CREATE_PAUSE("");
                    }
                    else
                    {
                        if (
                            chanceToMakeSecondBestMove > 0.01 &&
                            (
                            NEAT::Globals::getSingleton()->getRandom().getRandomDouble() <
                            chanceToMakeSecondBestMove
                            )
                            )
                        {
                            if (secondBestMoveToMake.from.x==255)
                            {
                                throw CREATE_LOCATEDEXCEPTION_INFO("THIS SHOULDN'T HAPPEN!");
                            }

#if CHECKERS_PRINT_ALTERNATE_MOVES
                            cout << "MADE SECOND BEST MOVE!!!!\n";
#endif

                            makeMove(secondBestMoveToMake,b);
                        }
                        else
                        {
                            makeMove(moveToMake,b);
                        }

                        retval = getWinner(b,BLACK);
                    }

#if CHECKERS_EXPERIMENT_DEBUG
                    printBoard(b);
                    CREATE_PAUSE("");
#endif
                }

#if CHECKERS_EXPERIMENT_LOG_EVALUATIONS
                memcpy(gameLog[rounds*2+1],b,sizeof(uchar)*8*8);
#endif


                int whiteMen,blackMen,whiteKings,blackKings;

                //countPieces(gi.board,whiteMen,blackMen,whiteKings,blackKings);
                countPieces(b,whiteMen,blackMen,whiteKings,blackKings);

                //Reward for # of pieces at every turn.
                individual->reward(2 * (12-whiteMen) );
                individual->reward(2 * (blackMen) );

                individual->reward(3 * (12-whiteKings) );
                individual->reward(3 * (blackKings) );

            }


            if (retval==BLACK)
            {
                //#if CHECKERS_EXPERIMENT_DEBUG
                cout << "WE WON!\n";
#if CHECKERS_EXPERIMENT_LOG_EVALUATIONS
                for (int a=0;a<rounds;a++)
                {
                    printBoard(gameLog[a*2]);

                    //if (a+1<=rounds)
                    {
                        //White didn't make the final move because black won
                        printBoard(gameLog[a*2+1]);
                    }
                }
                CREATE_PAUSE("");
#endif
                //CREATE_PAUSE("");
                //#endif

                if (individual->getUserData())
                {
                    (static_pointer_cast<CheckersStats>(individual->getUserData()))->wins++;
                }

                individual->reward(40000);

                //You get bonus fitness for every round you don't have to play
                //if you win
                int roundsLeftOver = CHECKERS_MAX_ROUNDS-rounds;
                individual->reward(roundsLeftOver*72.0);

            }
            /*NOTE: Tying counts as a loss
            else if (retval==-1) //draw
            {
            cout << "WE TIED!\n";
            individual->reward(200);
            }
            */
            else //loss
            {
#if CHECKERS_EXPERIMENT_DEBUG
                //Final board:
                printBoard(b);
                //CREATE_PAUSE("LOSS!");
#endif
                //if(gi.nummoves<90)
                //{
                //individual->reward(rounds/(((CheckersNEATDatatype)CHECKERS_MAX_ROUNDS)/10));
                //}


#if CHECKERS_EXPERIMENT_PRINT_EVALUATIONS_ON_LOSS
                for (int a=0;a<rounds;a++)
                {
                    printBoard(gameLog[a*2]);

                    //if (a+1<=rounds)
                    {
                        //White didn't make the final move because black won
                        printBoard(gameLog[a*2+1]);
                    }
                }
                CREATE_PAUSE("");
#endif

                if (retval==-1)
                {
                    //tie
                    if (individual->getUserData())
                    {
                        (static_pointer_cast<CheckersStats>(individual->getUserData()))->ties++;
                    }
                }
                else
                {
                    //loss
                    if (individual->getUserData())
                    {
                        (static_pointer_cast<CheckersStats>(individual->getUserData()))->losses++;
                    }
                }

                //Reward losing.  This discourages ties
                individual->reward(10000);
            }
        }

        if (PLAY_BOTH_SIDES)
        {
            //Now, let's do some trials where HyperNEAT evaluates for white
            //cout << "Playing games with HyperNEAT as white\n";
            for (handCodedType=0;handCodedType<HANDCODED_PLAYER_TESTS;handCodedType++)
            {

                resetBoard(b);

                int retval=-1;
                int rounds=0;

                for (rounds=0;rounds<CHECKERS_MAX_ROUNDS&&retval==-1;rounds++)
                {
                    //cout << "Round: " << rounds << endl;
                    moveToMake = CheckersMove();
                    secondBestMoveToMake = CheckersMove();
                    DEBUG_USE_HANDCODED_EVALUATION = 1;
                    DEBUG_USE_HYPERNEAT_EVALUATION = 0;
                    currentSubstrateIndex=handCodedAISubstrateIndex;
                    firstevaluatemax(b,BASE_EVOLUTION_SEARCH_DEPTH+1);

#if CHECKERS_EXPERIMENT_DEBUG
                    cout << "BLACK MAKING MOVE\n";

                    printBoard(b);
#endif

                    if (moveToMake.from.x==255)
                    {
                        //white loses
                        cout << "BLACK LOSES!\n";
                        retval = WHITE;
                    }
                    else
                    {
                        makeMove(moveToMake,b);
                        retval = getWinner(b,WHITE);
                    }

#if CHECKERS_EXPERIMENT_LOG_EVALUATIONS
                    memcpy(gameLog[rounds*2],b,sizeof(uchar)*8*8);
#endif

#if CHECKERS_EXPERIMENT_DEBUG
                    printBoard(b);
                    CREATE_PAUSE("");
#endif

                    if (retval==-1)
                    {
                        //printBoard(b);

                        moveToMake = CheckersMove();
                        secondBestMoveToMake = CheckersMove();
                        {
                            //progress_timer t;
                            DEBUG_USE_HANDCODED_EVALUATION = 0;
                            DEBUG_USE_HYPERNEAT_EVALUATION = 1;
                            currentSubstrateIndex=0;
                            firstevaluatemin(b,BASE_EVOLUTION_SEARCH_DEPTH+1+NEAT_SEARCH_HANDICAP);
                            //cout << "SimpleCheckers time: ";
                        }

                        /*
                        cout << "JGAI SAYS TO MAKE MOVE FROM ("
                        << moveToMake.from.x << ','
                        << moveToMake.from.y << ") -> (" << moveToMake.to.x
                        << ',' << moveToMake.to.y << ")\n";

                        char str[255];
                        int playnow=0;
                        int fromx,fromy,tox,toy;
                        uchar tmpBoard[8][8];
                        memcpy(tmpBoard,b,sizeof(uchar)*8*8);
                        getmove(tmpBoard,WHITE,1000.0,str,&playnow,0,0,fromx,fromy,tox,toy);

                        cout << "AI SAYS TO MAKE MOVE FROM (" << fromx << ','
                        << fromy << ") -> (" << tox << ',' << toy << ")\n";

                        printBoard(b);
                        CREATE_PAUSE("");
                        */

#if CHECKERS_EXPERIMENT_DEBUG
                        cout << "WHITE MAKING MOVE\n";

                        printBoard(b);
#endif

                        if (moveToMake.from.x==255)
                        {
                            //white loses
                            cout << "WHITE LOSES BECAUSE THERE'S NO MOVES LEFT!\n";
                            retval = BLACK;
                            printBoard(b);
                            CREATE_PAUSE("");
                        }
                        else
                        {
                            makeMove(moveToMake,b);
                            retval = getWinner(b,BLACK);
                        }

#if CHECKERS_EXPERIMENT_DEBUG
                        printBoard(b);
                        CREATE_PAUSE("");
#endif
                    }

#if CHECKERS_EXPERIMENT_LOG_EVALUATIONS
                    memcpy(gameLog[rounds*2+1],b,sizeof(uchar)*8*8);
#endif

                    int whiteMen,blackMen,whiteKings,blackKings;

                    //countPieces(gi.board,whiteMen,blackMen,whiteKings,blackKings);
                    countPieces(b,whiteMen,blackMen,whiteKings,blackKings);

                    //Reward for # of pieces at every turn.
                    individual->reward(2 * (12-whiteMen) );
                    individual->reward(2 * (blackMen) );

                    individual->reward(3 * (12-whiteKings) );
                    individual->reward(3 * (blackKings) );
                }

                if (retval==WHITE)
                {
                    //#if CHECKERS_EXPERIMENT_DEBUG
                    cout << "WE WON!\n";
#if CHECKERS_EXPERIMENT_LOG_EVALUATIONS
                    for (int a=0;a<rounds;a++)
                    {
                        printBoard(gameLog[a*2]);

                        //if (a+1<=rounds)
                        {
                            //White made the final move because white won
                            printBoard(gameLog[a*2+1]);
                        }
                    }
#endif
                    //CREATE_PAUSE("");
                    //#endif

                    individual->reward(40000);

                    //You get bonus fitness for every round you don't have to play
                    //if you win
                    int roundsLeftOver = CHECKERS_MAX_ROUNDS-rounds;
                    individual->reward(roundsLeftOver*72.0);
                }
                /*NOTE: Tying counts as a loss
                else if (retval==-1) //draw
                {
                cout << "WE TIED!\n";
                individual->reward(200);
                }
                */
                else //loss
                {
#if CHECKERS_EXPERIMENT_DEBUG
                    //Final board:
                    printBoard(b);
                    //CREATE_PAUSE("LOSS!");
#endif
                    //if(gi.nummoves<90)
                    //{
                    //individual->reward(rounds/(((CheckersNEATDatatype)CHECKERS_MAX_ROUNDS)/10));
                    //}

                    //Reward losing.  This discourages ties
                    individual->reward(10000);
                }

                int whiteMen,blackMen,whiteKings,blackKings;

                //countPieces(gi.board,whiteMen,blackMen,whiteKings,blackKings);
                countPieces(b,whiteMen,blackMen,whiteKings,blackKings);

                individual->reward(2 * (12-blackMen) );
                individual->reward(2 * (whiteMen) );

                individual->reward(3 * (12-blackKings) );
                individual->reward(3 * (whiteKings) );
            }
        }
    }

    void CheckersExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual)
    {
        //cout << "INDIVIDUAL FITNESS BEFORE: " << individual->getFitness() << endl;
        clearGroup();
        addIndividualToGroup(individual);
        individual->setUserData(shared_ptr<CheckersStats>(new CheckersStats()));

        for (int a=0;a<100;a++)
        {
            shared_ptr<GeneticGeneration> dummy;
            chanceToMakeSecondBestMove=0.10;
            processGroup(dummy);
            chanceToMakeSecondBestMove=0.0;
            //cout << "INDIVIDUAL FITNESS: " << individual->getFitness() << endl;
        }
    }

#ifndef HCUBE_NOGUI
    void CheckersExperiment::createIndividualImage(wxDC &drawContext,shared_ptr<NEAT::GeneticIndividual> individual)
    {
        currentSubstrateIndex=0;

        if (lastIndividualSeen!=individual)
        {
            //cout << "Repopulating substrate\n";
            populateSubstrate(individual);
            lastIndividualSeen = individual;
        }

        drawContext.Clear();

        for (int x=0;x<8;x++)
        {
            for (int y=(x%2);y<8;y+=2)
            {
                //array goes in opposite direction of drawing
                int drawX = x;
                int drawY = 8-y;

                //if (userEvaluationRound%2==1)
                {
                    //Flip the board for white
                    drawX = 8-x;
                    drawY = y;
                }

                if (userEvaluationBoard[x][y]&FREE)
                {}
                else if (userEvaluationBoard[x][y]&BLACK)
                {
                    if (userEvaluationBoard[x][y]&MAN)
                    {
                        drawContext.DrawText(_T("b"),20+drawX*20,20+drawY*20);
                    }
                    else
                    {
                        drawContext.DrawText(_T("B"),20+drawX*20,20+drawY*20);
                    }
                }
                else if (userEvaluationBoard[x][y]&WHITE)
                {
                    if (userEvaluationBoard[x][y]&MAN)
                    {
                        drawContext.DrawText(_T("w"),20+drawX*20,20+drawY*20);
                    }
                    else
                    {
                        drawContext.DrawText(_T("W"),20+drawX*20,20+drawY*20);
                    }
                }
                else
                {
                    drawContext.DrawText(_T("-"),20+drawX*20,20+drawY*20);
                }
            }
        }

        int playerToMove;
        if (userEvaluationRound%2==0)
        {
            playerToMove = BLACK;
        }
        else
        {
            playerToMove = WHITE;
        }

        int winner = getWinner(userEvaluationBoard,playerToMove);

        if (winner==WHITE)
        {
            drawContext.DrawText(_T("WHITE WINS"),30,300);
        }
        else if (winner==BLACK)
        {
            drawContext.DrawText(_T("BLACK WINS"),30,300);
        }
        else
        {
            if (userEvaluationRound%2==0)
            {
                drawContext.DrawText(_T("Black's turn.  Click to see black's move"),30,300);
            }
            else if (from.x!=255)
            {
                drawContext.DrawText(_T("Move to?"),30,300);
            }
            else
            {
                drawContext.DrawText(_T("Move from?"),30,300);
            }
        }
    }

    bool CheckersExperiment::handleMousePress(wxMouseEvent& event,wxSize &bitmapSize)
    {
        int playerToMove;
        if (userEvaluationRound%2==0)
        {
            playerToMove = BLACK;
        }
        else
        {
            playerToMove = WHITE;
        }

        int winner = getWinner(userEvaluationBoard,playerToMove);

        if (winner==WHITE||winner==BLACK)
        {
            resetBoard(userEvaluationBoard);
            return true;
        }

        if (userEvaluationRound%2==0)
        {
            //Computer makes his move
            {
                moveToMake = CheckersMove();
                secondBestMoveToMake = CheckersMove();
                cout << __LINE__ << " CALLING EVALUATEMAX\n";
                DEBUG_USE_HANDCODED_EVALUATION = 0;
                DEBUG_USE_HYPERNEAT_EVALUATION = 1;
                currentSubstrateIndex=0;
                firstevaluatemax(userEvaluationBoard,BASE_EVOLUTION_SEARCH_DEPTH);
            }

            if (moveToMake.from.x==255)
            {
                //black loses
                cout << "BLACK LOSES!\n";
            }
            else
            {
                makeMove(moveToMake,userEvaluationBoard);
                userEvaluationRound++;
            }

        }
        else
        {
#if CHECKERS_EXPERIMENT_INTERACTIVE_PLAY
            Vector2<uchar> boardPosition(
                (event.GetX()-20)/20,
                (event.GetY()-20)/20
                );

            //if (userEvaluationRound%2==1)
            {
                //Flip the board for white
                boardPosition.x = 8-boardPosition.x;
            }
            //else
            {
                //boardPosition.y = 8-boardPosition.y;
            }

            if (from.x!=255)
            {
                bool foundJump;
                totalMoveList.clear();
                int numMoves = generateMoveList(
                    totalMoveList,
                    0,
                    userEvaluationBoard,
                    WHITE,
                    foundJump
                    );

                for (int a=0;a<numMoves;a++)
                {
                    if (
                        totalMoveList[a].from==from &&
                        totalMoveList[a].getFinalDestination()==boardPosition
                        )
                    {
                        //Move was a valid move, make it happen
                        makeMove(totalMoveList[a],userEvaluationBoard);
                        userEvaluationRound++;
                    }
                }

                from = Vector2<uchar>(255,255);
            }
            else
            {
                from = boardPosition;
                cout << " Added from " << (int)from.x << ',' << (int)from.y << endl;
            }
#else
            //Heuristic makes his move
            {
                moveToMake = CheckersMove();
                secondBestMoveToMake = CheckersMove();
                cout << __LINE__ << " CALLING EVALUATEMIN\n";
                DEBUG_USE_HANDCODED_EVALUATION = 1;
                DEBUG_USE_HYPERNEAT_EVALUATION = 0;
                firstevaluatemin(userEvaluationBoard,BASE_EVOLUTION_SEARCH_DEPTH);

                if(
                    NEAT::Globals::getSingleton()->getRandom().getRandomDouble() <
                    0.10*max(1,10-userEvaluationRound)
                    )
                {
                    cout << __FILE__ << ":" << __LINE__ << ": PICKING SECOND BEST MOVE!\n";
                    moveToMake = secondBestMoveToMake;
                }
            }

            if (moveToMake.from.x==255)
            {
                //white loses
                cout << "WHITE LOSES!\n";
            }
            else
            {
                makeMove(moveToMake,userEvaluationBoard);
                userEvaluationRound++;
            }
#endif
        }

        return true;
    }
#endif

    Experiment* CheckersExperiment::clone()
    {
        CheckersExperiment* experiment = new CheckersExperiment(*this);

        return experiment;
    }

    void CheckersExperiment::resetGenerationData(shared_ptr<NEAT::GeneticGeneration> generation)
    {
        //generation->setUserData(new TicTacToeStats());
    }

    void CheckersExperiment::addGenerationData(shared_ptr<NEAT::GeneticGeneration> generation,shared_ptr<NEAT::GeneticIndividual> individual)
    {
        //TicTacToeStats* ticTacToeStats = (TicTacToeStats*)generation->getUserData();

        //if (individual->getUserData())
        //(*ticTacToeStats) += *((TicTacToeStats*)individual->getUserData());
    }
}
