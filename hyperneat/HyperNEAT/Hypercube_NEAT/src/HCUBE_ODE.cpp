#ifdef COMPILE_ODE
#include "HCUBE_ODE.h"

#define SUBSTRATE_DEBUG (0)

dWorldID      world;        // world for kinetics calculation
dSpaceID      space;        // space for collision detection
dGeomID       ground;       // ground
dJointGroupID contactgroup; // Contact group
#ifndef NOVIZ
dsFunctions   fn;           // Drawing function of draw stuff
float hpr[3];  // gaze[°] 
float xyz[3];  // viewpoint
#endif


#if LEGSWING_EXPERIMENT_ENABLE_BIASES
NEAT::FastBiasNetwork<double> * substrate_pointer;
#else
NEAT::FastNetwork<double> * substrate_pointer;
#endif

map<HCUBE::Node,string> nameLookupGlobal;



bool someNonFootIsTouchingTheGround = false;
bool userSaysQuit = false;
int rewardOrg = 0;
int thisLegIsTouching [BODY_SEGMENTS][num_legs]; 
bool pauseGlobal = false;

int experimentType;
int dimensionality;
int simulation_steps;
int fitness_function;
dVector3 totalForce;
int numDirectionChanges = 0;
//#ifndef USE_NLEG_ROBOT //when not using n-leg robot
//const int maxnumDirectionChanges = .16*simulation_steps*(static_cast<double>(num_legs*BODY_SEGMENTS)/4.0);
//#else //when using n-leg
//const int maxnumDirectionChanges = INT_MAX;//.16*simulation_steps*(static_cast<double>(num_legs*BODY_SEGMENTS)/4.0);
//#endif
int maxnumDirectionChanges;  // set in experiment constructor
dReal maxXdistanceFrom0[BODY_SEGMENTS];// = 0;
dReal maxYdistanceFrom0[BODY_SEGMENTS];// = 0;
dReal currentXdistanceFrom0[BODY_SEGMENTS];// = 0;
dReal currentYdistanceFrom0[BODY_SEGMENTS];// = 0;

//bool visualize = true;
bool visualize = false;
//bool printJointAngles = true;
bool printJointAngles = false;
bool postHoc = false;
int timeSteps = 0;
dReal timeStepDivisor = 200; //controls the sine period

#ifndef USE_NLEG_ROBOT
// TODO: initialize some where else
dReal lastJangle                 [BODY_SEGMENTS][num_legs][num_joints] = { { {0,0,0},{0,0,0},{0,0,0},{0,0,0} } };  //joint0 = HipFrontBack (HipFB), joint1 = HipInOut (HipIO), join2 = knee 
int lastJangleDirectionPositve   [BODY_SEGMENTS][num_legs][num_joints] = { { {0,0,0},{0,0,0},{0,0,0},{0,0,0} } };
double newDesiredAngle           [BODY_SEGMENTS][num_legs][num_joints] = { { {0,0,0},{0,0,0},{0,0,0},{0,0,0} } };
dReal  THETA[BODY_SEGMENTS][num_legs][num_links] = {{{0},{0},{0},{0}}}; // Target angle
#else // USE_NLEG_ROBOT
dReal lastJangle                 [BODY_SEGMENTS][num_legs][num_joints] = { { {0,0,0},{0,0,0} } };
int lastJangleDirectionPositve   [BODY_SEGMENTS][num_legs][num_joints] = { { {0,0,0},{0,0,0} } };
double newDesiredAngle           [BODY_SEGMENTS][num_legs][num_joints] = { { {0,0,0},{0,0,0} } };
double torsoJointDesiredAngle    [BODY_SEGMENTS] = {0.0};
dReal  THETA[BODY_SEGMENTS][num_legs][num_links];// = {{{0},{0}}}; // Target angle
#endif
int hingeType = 0;

double jointError[BODY_SEGMENTS][num_legs][num_joints];
//int numJointsWithError = 0;
int currentJointsWithError = 0;
double widthOfErrorRange =0;

int numSheets = 0;


MyLink leg[BODY_SEGMENTS][num_legs][num_links]; //Legs
MyLink torso[BODY_SEGMENTS]; //  and body segments


const dReal percentOfLegs =.25;

const dReal  SX = 0, SY = 0, SZ = .2;           // Initial position of body center of gravity
//const dReal  l1 = 1.5*0.05, l2 = 0.3, l3  = 0.3;  // Leg segment lengths, l1=hip, l2=thigh, l3=calf
const dReal  l1 = 1.5*0.05, l2 = 0.3*percentOfLegs, l3  = 0.3*percentOfLegs;  // Leg segment lengths, l1=hip, l2=thigh, l3=calf

#ifndef USE_NLEG_ROBOT
const dReal  lx = 0.5, ly= 0.3, lz = 0.05;         // body sides
const dReal  r1 = 0.02, r2 = 0.02, r3 = 0.02 ;     // leg radius
const dReal  cx1 = (lx-r1)/2, cy1 = (ly+l1)/2;     // Temporary variable
#else
const dReal  lx = 0.3, ly= 0.3, lz = 0.05;         // body sides
const dReal  r1 = 0.02, r2 = 0.02, r3 = 0.02 ;     // leg radius
const dReal  cx1 = (lx-r1)/2, cy1 = (ly+l1)/2;     // Temporary variable
#endif

#ifndef USE_NLEG_ROBOT
// for four legs
const dReal  c_x[num_legs][num_links] = {{ cx1, cx1, cx1},{-cx1,-cx1,-cx1}, // Joint central x coordinate
{-cx1,-cx1,-cx1},{ cx1, cx1, cx1}};
const dReal  c_y[num_legs][num_links] = {{ cy1, cy1, cy1},{ cy1, cy1, cy1}, // Joint central y coordinate
{-cy1,-cy1,-cy1},{-cy1,-cy1,-cy1}};
const dReal  c_z[num_legs][num_links] =  {{0, 0, -l2},{0, 0, -l2},          // Joint central z coordinate
{0, 0, -l2},{0, 0, -l2}};
#else
// for four legs
const dReal  c_x[num_legs][num_links] = {{-cx1,-cx1,-cx1}, // Joint central x coordinate
{-cx1,-cx1,-cx1}};
const dReal  c_y[num_legs][num_links] = {{ cy1, cy1, cy1}, // Joint central y coordinate
{-cy1,-cy1,-cy1}};
const dReal  c_z[num_legs][num_links] = {{0, 0, -l2},          // Joint central z coordinate
{0, 0, -l2}};

#endif


void resetRelevantGlobals (){
	
	timeSteps = 0;  //reset the timestep counter
	someNonFootIsTouchingTheGround = false; //reset the touching flag
	userSaysQuit = false;
	rewardOrg = 0; //reset the reward org flag
	
	for(int segment = 0; segment < BODY_SEGMENTS; ++segment) {
		maxXdistanceFrom0[segment] = 0;
		maxYdistanceFrom0[segment] = 0;
		currentXdistanceFrom0[segment] = 0;
		currentYdistanceFrom0[segment] = 0;
#ifdef USE_NLEG_ROBOT
		torsoJointDesiredAngle[segment] = 0.0;
#endif
		
		for(int leg_no=0;leg_no<num_legs;leg_no++){
			thisLegIsTouching[segment][leg_no] = 0;
			for(int joint_no=0;joint_no<num_joints;joint_no++){
				lastJangle[segment][leg_no][joint_no]=0;
				newDesiredAngle[segment][leg_no][joint_no]=0;
				lastJangleDirectionPositve[segment][leg_no][joint_no]=0;
				THETA[segment][leg_no][joint_no] =0;
			}
		}
	}
	
	numDirectionChanges =0;
}

void get_euler(const dReal * matrix,dReal &kx,dReal &ky,dReal &kz)
{       
	const dReal epsilon=0.0000001;
	if(matrix[2] < 1-epsilon && matrix[2] > -1+epsilon)
	{
		ky=-asin(matrix[2]);
		dReal c=cos(ky);
		kx= atan2(matrix[6]/c,matrix[10]/c);
		kz= atan2(matrix[1]/c,matrix[0]/c);
	} else {       
		kz=0;   
		ky=-atan2(matrix[2],0);
		kx= atan2(-matrix[9],matrix[5]);
	}       
}       


bool shouldIKeepRunningTheSimulation (){
	//if(visualize) printf("timeSteps: %i\n", timeSteps);
	//simulation_steps = 6000;
	//simulation_steps = 500;
	//maxnumDirectionChanges = INT_MAX;
	

	if (timeSteps<simulation_steps and !someNonFootIsTouchingTheGround and !userSaysQuit and numDirectionChanges<=maxnumDirectionChanges) return true;
	else{
		//use if printing generalization info
		//ofstream generalization_file;        
		//generalization_file.open ("generalization_file.txt", ios::app );
		//
		//PRINT(timeSteps);
		//generalization_file << timeSteps << endl;
		//generalization_file.close();

		if(visualize and !(timeSteps<simulation_steps)) printf("quitting cause timeSteps>=simulation_steps\n");
		if(visualize and someNonFootIsTouchingTheGround) printf("quitting cause someNonFootIsTouchingTheGround\n");
		if(visualize and !(numDirectionChanges<=maxnumDirectionChanges)) printf("quitting cause numDirectionChanges>maxnumDirectionChanges\n");

		return false;
	}
}


#ifndef USE_NLEG_ROBOT
/*** Formation of robot ***/
void  makeRobot() 
{ 

  dReal torso_m = 10.0;                    // Mass of body
  dReal  l1m = 0.005,l2m = 0.5, l3m = 0.5; // Mass of leg segments 
	
  //for four legs
  dReal x[num_legs][num_links] = {{ cx1, cx1, cx1},{-cx1,-cx1,-cx1},// Position of each link (x coordinate)
		{-cx1,-cx1,-cx1},{ cx1, cx1, cx1}};
  dReal y[num_legs][num_links] = {{ cy1, cy1, cy1},{ cy1, cy1, cy1},// Position of each link (y coordinate)
		{-cy1,-cy1,-cy1},{-cy1,-cy1,-cy1}};
  dReal z[num_legs][num_links] = {                                  // Position of each link (z coordinate)
		{c_z[0][0],(c_z[0][0]+c_z[0][2])/2,c_z[0][2]-l3/2},
		{c_z[0][0],(c_z[0][0]+c_z[0][2])/2,c_z[0][2]-l3/2},
		{c_z[0][0],(c_z[0][0]+c_z[0][2])/2,c_z[0][2]-l3/2},
		{c_z[0][0],(c_z[0][0]+c_z[0][2])/2,c_z[0][2]-l3/2}};
	
	
  dReal r[num_links]          =  { r1, r2, r3}; // radius of leg segment
  dReal length[num_links]     =  { l1, l2, l3}; // Length of leg segment
  dReal weight[num_links]     =  {l1m,l2m,l3m}; // Mass of leg segment
	
	//  //for one leg
	//  dReal axis_x[num_legs_pneat][num_links_pneat] = {{ 0,1, 0}};
	//  dReal axis_y[num_legs_pneat][num_links_pneat] = {{ 1,0, 1}};
	//  dReal axis_z[num_legs_pneat][num_links_pneat] = {{ 0,0, 0}};
	
  //for four legs
  dReal axis_x[num_legs][num_links] = {{ 0,1, 0},{ 0,1,0},{ 0, 1, 0},{ 0, 1, 0}};
  dReal axis_y[num_legs][num_links] = {{ 1,0, 1},{ 1,0,1},{ 1, 0, 1},{ 1, 0, 1}};
  dReal axis_z[num_legs][num_links] = {{ 0,0, 0},{ 0,0,0},{ 0, 0, 0},{ 0, 0, 0}};
  
  // For mation of the body
  dMass mass; 
  torso[0].body  = dBodyCreate(world);
  dMassSetZero(&mass);
  dMassSetBoxTotal(&mass,torso_m, lx, ly, lz);
  dBodySetMass(torso[0].body,&mass);
  torso[0].geom = dCreateBox(space, lx, ly, lz); 
  dGeomSetBody(torso[0].geom, torso[0].body); 
  dBodySetPosition(torso[0].body, SX, SY, SZ); 
	
  // Formation of leg
  dMatrix3 R;                          // Revolution queue
  dRFromAxisAndAngle(R,1,0,0,M_PI/2);  // 90 degrees to turn, parallel with the land
  for (int i = 0; i < num_legs; i++) { 
    for (int j = 0; j < num_links; j++) { 
      leg[0][i][j].body = dBodyCreate(world);
      if (j == 0) dBodySetRotation(leg[0][i][j].body,R);
      dBodySetPosition(leg[0][i][j].body, SX+x[i][j], SY+y[i][j], SZ+z[i][j]);
      dMassSetZero(&mass);
      dMassSetCapsuleTotal(&mass,weight[j],3,r[j],length[j]);
      dBodySetMass(leg[0][i][j].body, &mass);
      //if(i==1 and j==2)                                         //to set the length of one leg differently
      //leg[i][j].geom = dCreateCapsule(space_pneat,r[j],length[j]+.5); //set the length of the leg
      //else
      leg[0][i][j].geom = dCreateCapsule(space,r[j],length[j]); //set the length of the leg
      dGeomSetBody(leg[0][i][j].geom,leg[0][i][j].body);
    }
  }
	
  // Formation of joints (and connecting them up)
  for (int i = 0; i < num_legs; i++) { 
    for (int j = 0; j < num_links; j++) { 
      leg[0][i][j].joint = dJointCreateHinge(world, 0);
      if (j == 0){ 
				dJointAttach(leg[0][i][j].joint, torso[0].body, leg[0][i][j].body); //connects hip to the environment
				dJointSetHingeParam(leg[0][i][j].joint, dParamLoStop, -.5*M_PI); //prevent the hip forward-back from going more than 90 degrees
				dJointSetHingeParam(leg[0][i][j].joint, dParamHiStop,  .5*M_PI);
      }
      else
        dJointAttach(leg[0][i][j].joint, leg[0][i][j-1].body, leg[0][i][j].body);
      dJointSetHingeAnchor(leg[0][i][j].joint, SX+c_x[i][j], SY+c_y[i][j],SZ+c_z[i][j]);
      dJointSetHingeAxis(leg[0][i][j].joint, axis_x[i][j], axis_y[i][j],axis_z[i][j]);
    } 
  } 
  
}

/*** Drawing of robot ***/
void drawRobot()
{
#ifndef NOVIZ
	if(visualize){ 
		dReal r,length;
		dVector3 sides;
		
		// Drawing of body
		dsSetTexture(1);
		dsSetColor(2.5,2.5,2.5);
		dGeomBoxGetLengths(torso[0].geom,sides);
		dsDrawBox(dBodyGetPosition(torso[0].body),
							dBodyGetRotation(torso[0].body),sides);
		
		// Drawing oflegs
		dsSetTexture(0);
		dsSetColor(1.3,0.0,0.0);
		
		for (int i = 0; i < num_legs; i++) {
			for (int j = 0; j < num_links; j++ ) { 
				dGeomCapsuleGetParams(leg[0][i][j].geom, &r,&length);
				if (j== 0) dsDrawCapsule(dBodyGetPosition(leg[0][i][j].body),
																 dBodyGetRotation(leg[0][i][j].body),0.5*length,1.2*r);
				else       dsDrawCapsule(dBodyGetPosition(leg[0][i][j].body),
																 dBodyGetRotation(leg[0][i][j].body),length,r);
			}
		}
		//		hpr[0] = 0.0f;
		//		hpr[1] = -20.0f;
		//		hpr[2] = 0.0f;  // point of view[m]
		//		//cerr <<dBodyGetPosition(torso[0].body)[0]<<" " <<dBodyGetPosition(torso[0].body)[1]<< " " << dBodyGetPosition(torso[0].body)[2]<<endl;
		//		xyz[0] = dBodyGetPosition(torso[0].body)[0]-.75f;
		//		xyz[1] = dBodyGetPosition(torso[0].body)[1];
		//		xyz[2] = dBodyGetPosition(torso[0].body)[2]+0.05f;  // gaze[°] 


		hpr[0] = 1.0f;
		hpr[1] = -1.2f;
		hpr[2] = 0.5f;  // point of view[m]
		
		//		hpr[0] = dBodyGetRotation(torso[0].body)[0];
		//		hpr[1] = dBodyGetRotation(torso[0].body)[1];
		//		hpr[2] = dBodyGetRotation(torso[0].body)[2];
		//cerr <<dBodyGetPosition(torso[0].body)[0]<<" " <<dBodyGetPosition(torso[0].body)[1]<< " " << dBodyGetPosition(torso[0].body)[2]<<endl;
		
		
		xyz[0] = dBodyGetPosition(torso[0].body)[0]-.75f;
		xyz[1] = dBodyGetPosition(torso[0].body)[1];
		xyz[2] = 0.1; //dBodyGetPosition(torso[0].body)[2]-0.15f;  // gaze[°] 

		//overrides to look from the other side of the robot
		//hpr[0] = -180.0f; //slides view to the right
		//xyz[0] = dBodyGetPosition(torso[0].body)[0]+.75f;

		//		hpr[1] = -30.2f; //slides down/up
//		hpr[2] = 20.5f;  // point of view[m]

		//different override for other corner
		xyz[0] = dBodyGetPosition(torso[0].body)[0]+.75f;
		xyz[1] = dBodyGetPosition(torso[0].body)[1]-.50;
		hpr[0] = -220.0f; //slides view to the right

		
		dsSetViewpoint(xyz,hpr);		
	}
#endif
}
#else //USE_NLEG_ROBOT

/******************************************/
/************* n-leg robot BEGIN **********/
/******************************************/
/*** Formation of robot ***/

void  makeRobot_Nleg() 
{ 
	for(int segment = 0; segment < BODY_SEGMENTS; ++segment) {
		dReal segmentOffsetFromMiddle = segment - MIDDLE_BODY_SEGMENT;
		
		dReal torso_m = 50.0;                    // Mass of body
		//	torso_m[segment] = 10.0;
		
		dReal  l1m = 0.005,l2m = 0.5, l3m = 0.5; // Mass of leg segments 
		
		//for four legs
		//		dReal x[num_legs][num_links] = {{-cx1,-cx1,-cx1},// Position of each link (x coordinate)
		//																		{-cx1,-cx1,-cx1}};
		dReal x[num_legs][num_links] = {{0,0,0},// Position of each link (x coordinate)
			{0,0,0}};
		
		dReal y[num_legs][num_links] = {{ cy1, cy1, cy1},// Position of each link (y coordinate)
			{-cy1,-cy1,-cy1}};
		dReal z[num_legs][num_links] = {                                  // Position of each link (z coordinate)
			{c_z[0][0],(c_z[0][0]+c_z[0][2])/2,c_z[0][2]-l3/2},
			{c_z[0][0],(c_z[0][0]+c_z[0][2])/2,c_z[0][2]-l3/2} };
		
		dReal r[num_links]          =  { r1, r2, r3}; // radius of leg segment
		dReal length[num_links]     =  { l1, l2, l3}; // Length of leg segment
		dReal weight[num_links]     =  {l1m,l2m,l3m}; // Mass of leg segment
		
		//  //for one leg
		//  dReal axis_x[num_legs_pneat][num_links_pneat] = {{ 0,1, 0}};
		//  dReal axis_y[num_legs_pneat][num_links_pneat] = {{ 1,0, 1}};
		//  dReal axis_z[num_legs_pneat][num_links_pneat] = {{ 0,0, 0}};
		
		//for four legs
		dReal axis_x[num_legs][num_links];
		dReal axis_y[num_legs][num_links];
		dReal axis_z[num_legs][num_links];
		for(int i = 0; i < num_legs; ++i) {
			axis_x[i][0] = 0.0;
			axis_x[i][1] = 1.0;
			axis_x[i][2] = 0.0;
			
			axis_y[i][0] = 1.0;
			axis_y[i][1] = 0.0;
			axis_y[i][2] = 1.0;
			
			axis_z[i][0] = 0.0;
			axis_z[i][1] = 0.0;
			axis_z[i][2] = 0.0;
		}
		// For mation of the body
		dMass mass; 
		torso[segment].body  = dBodyCreate(world);
		dMassSetZero(&mass);
		dMassSetBoxTotal(&mass,torso_m, lx, ly, lz);
		dBodySetMass(torso[segment].body,&mass);
		torso[segment].geom = dCreateBox(space,lx, ly, lz); 
		dGeomSetBody(torso[segment].geom, torso[segment].body);
		dBodySetPosition(torso[segment].body, SX+segmentOffsetFromMiddle*(lx+DISTANCE_BETWEEN_SEGMENTS), SY, SZ); 
		
		// Formation of leg
		dMatrix3 R;                          // Revolution queue
		dRFromAxisAndAngle(R,1,0,0,M_PI/2);  // 90 degrees to turn, parallel with the land
		for (int i = 0; i < num_legs; i++) { 
			for (int j = 0; j < num_links; j++) {
				THETA[segment][i][j] = 0;
				
				leg[segment][i][j].body = dBodyCreate(world);
				if (j == 0)
					dBodySetRotation(leg[segment][i][j].body,R);
				dBodySetPosition(leg[segment][i][j].body, SX+x[i][j]+segmentOffsetFromMiddle*(lx+DISTANCE_BETWEEN_SEGMENTS), SY+y[i][j], SZ+z[i][j]);				
				dMassSetZero(&mass);
				dMassSetCapsuleTotal(&mass,weight[j],3,r[j],length[j]);
				dBodySetMass(leg[segment][i][j].body, &mass);
				//if(i==1 and j==2)                                         //to set the length of one leg differently
				//leg[i][j].geom = dCreateCapsule(space_pneat,r[j],length[j]+.5); //set the length of the leg
				//else
				leg[segment][i][j].geom = dCreateCapsule(space,r[j],length[j]); //set the length of the leg
				dGeomSetBody(leg[segment][i][j].geom,leg[segment][i][j].body);
			}
		}
		
		// Formation of joints (and connecting them up)
		for (int i = 0; i < num_legs; i++) { 
			for (int j = 0; j < num_links; j++) { 
				leg[segment][i][j].joint = dJointCreateHinge(world, 0);
				if (j == 0){ 
					dJointAttach(leg[segment][i][j].joint, torso[segment].body, leg[segment][i][j].body); //connects hip to the environment
					dJointSetHingeParam(leg[segment][i][j].joint, dParamLoStop, -.50*M_PI); //prevent the hip forward-back from going more than 90 degrees
					dJointSetHingeParam(leg[segment][i][j].joint, dParamHiStop,  .50*M_PI);
				}
				else
					dJointAttach(leg[segment][i][j].joint, leg[segment][i][j-1].body, leg[segment][i][j].body);
				
				dJointSetHingeAnchor(leg[segment][i][j].joint, SX+x[i][j]+segmentOffsetFromMiddle*(lx+DISTANCE_BETWEEN_SEGMENTS), SY+c_y[i][j],SZ+c_z[i][j]);
				dJointSetHingeAxis(leg[segment][i][j].joint, axis_x[i][j], axis_y[i][j],axis_z[i][j]);
			} 
		}
	}
	
	
#ifdef USE_NLEG_ROBOT
	// link torsos
	for(int segment = 0; segment < BODY_SEGMENTS-1; ++segment) {
		dReal segmentOffsetFromMiddle = segment - MIDDLE_BODY_SEGMENT;
		
		switch (hingeType) {
			case 1: //Hinge Joint, X axis  (back-breaker)
				torso[segment].joint = dJointCreateHinge(world, 0);
				dJointAttach(torso[segment].joint, torso[segment].body, torso[segment+1].body);
				dJointSetHingeAnchor(torso[segment].joint, SX+segmentOffsetFromMiddle*(lx+DISTANCE_BETWEEN_SEGMENTS)+(lx+DISTANCE_BETWEEN_SEGMENTS)/2, SY,SZ);
				dJointSetHingeAxis (torso[segment].joint, 1.0, 0.0, 0.0);
				break;
			case 2: //Hinge Joint, Y axis (???)
				torso[segment].joint = dJointCreateHinge(world, 0);
				dJointAttach(torso[segment].joint, torso[segment].body, torso[segment+1].body);
				dJointSetHingeAnchor(torso[segment].joint, SX+segmentOffsetFromMiddle*(lx+DISTANCE_BETWEEN_SEGMENTS)+(lx+DISTANCE_BETWEEN_SEGMENTS)/2, SY,SZ);
				dJointSetHingeAxis (torso[segment].joint, 0.0, 1.0, 0.0);
				break;
			case 3: //Hinge Joint, Z axis (snake-like)
				torso[segment].joint = dJointCreateHinge(world, 0);
				dJointAttach(torso[segment].joint, torso[segment].body, torso[segment+1].body);
				dJointSetHingeAnchor(torso[segment].joint, SX+segmentOffsetFromMiddle*(lx+DISTANCE_BETWEEN_SEGMENTS)+(lx+DISTANCE_BETWEEN_SEGMENTS)/2, SY,SZ);
				dJointSetHingeAxis (torso[segment].joint, 0.0, 0.0, 1.0);
				break;
			case 4: // Slider, Y axis  (??)
				torso[segment].joint = dJointCreateSlider(world, 0);
				dJointAttach(torso[segment].joint, torso[segment].body, torso[segment+1].body);
				dJointSetSliderAxis (torso[segment].joint, 0.0, 1.0, 0.0);
				break;
			case 5: // Slider, X axis  (extendo)
				torso[segment].joint = dJointCreateSlider(world, 0);
				dJointAttach(torso[segment].joint, torso[segment].body, torso[segment+1].body);
				dJointSetSliderAxis (torso[segment].joint, 1.0, 0.0, 0.0);				
				break;
			case 6: //Universal Joint
				torso[segment].joint = dJointCreateUniversal(world, 0);
				dJointAttach(torso[segment].joint, torso[segment].body, torso[segment+1].body);
				dJointSetUniversalAnchor(torso[segment].joint, SX+segmentOffsetFromMiddle*(lx+DISTANCE_BETWEEN_SEGMENTS)+(lx+DISTANCE_BETWEEN_SEGMENTS)/2, SY,SZ);
				dJointSetUniversalAxis1(torso[segment].joint, 0.0, 1.0, 0.0);
				dJointSetUniversalAxis2(torso[segment].joint, 0.0, 0.0, 1.0);				
				break;
			case 7: //Ball Joint
				torso[segment].joint = dJointCreateBall(world, 0);
				dJointAttach(torso[segment].joint, torso[segment].body, torso[segment+1].body);
				dJointSetBallAnchor(torso[segment].joint, SX+segmentOffsetFromMiddle*(lx+DISTANCE_BETWEEN_SEGMENTS)+(lx+DISTANCE_BETWEEN_SEGMENTS)/2, SY,SZ);
				break;
			case 8:  // Fixed
				torso[segment].joint = dJointCreateHinge(world, 0);
				dJointAttach(torso[segment].joint, torso[segment].body, torso[segment+1].body);
				dJointSetHingeAnchor(torso[segment].joint, SX+segmentOffsetFromMiddle*(lx+DISTANCE_BETWEEN_SEGMENTS)+(lx+DISTANCE_BETWEEN_SEGMENTS)/2, SY,SZ);
				dJointSetHingeAxis (torso[segment].joint, 1.0, 0.0, 0.0);				
				dJointSetHingeParam(torso[segment].joint, dParamLoStop, -0.00*M_PI); //prevent the hip forward-back from going more than 90 degrees
				dJointSetHingeParam(torso[segment].joint, dParamHiStop,  0.00*M_PI);
				break;
			default:
				assert(false); // not a valid hinge type
				break;
		}
	}
#endif
}

// --- Drawing of robot --- //

void drawRobot_Nleg()
{
#ifndef NOVIZ
	if(visualize){
		for(int segment = 0; segment < BODY_SEGMENTS; ++segment) {
			dReal r,length;
			dVector3 sides;
			
			// Drawing of body
			// Drawing of body
			dsSetTexture(1);
			dsSetColor(2.5,2.5,2.5);
			dGeomBoxGetLengths(torso[segment].geom,sides);
			dsDrawBox(dBodyGetPosition(torso[segment].body),
								dBodyGetRotation(torso[segment].body),sides);
			
			// Drawing oflegs
			dsSetTexture(0);
			dsSetColor(1.3,0.0,0.0);
			
			for (int i = 0; i < num_legs; i++) {
				for (int j = 0; j < num_links; j++ ) { 
					dGeomCapsuleGetParams(leg[segment][i][j].geom, &r,&length);
					if (j== 0) dsDrawCapsule(dBodyGetPosition(leg[segment][i][j].body),
																	 dBodyGetRotation(leg[segment][i][j].body),0.5*length,1.2*r);
					else       dsDrawCapsule(dBodyGetPosition(leg[segment][i][j].body),
																	 dBodyGetRotation(leg[segment][i][j].body),length,r);
				}
			}
		}
		//		int middleBodySegment = BODY_SEGMENTS/2;
		//		cerr <<dBodyGetPosition(torso.body)[0]<<" " <<dBodyGetPosition(torso.body)[1]<< " " << dBodyGetPosition(torso.body)[2]<<endl;
		
		//FROM SIDE
		//		hpr[0] = 90.0f;
		//		hpr[1] = 0.0f;//-1.2f;
		//		hpr[2] = 0.0f;  // point of view[m]
		//		xyz[0] = dBodyGetPosition(torso[MIDDLE_BODY_SEGMENT].body)[0];
		//		xyz[1] = dBodyGetPosition(torso[MIDDLE_BODY_SEGMENT].body)[1]-ceil(log2(BODY_SEGMENTS));
		//		xyz[2] = dBodyGetPosition(torso[MIDDLE_BODY_SEGMENT].body)[2]+0.2f;  // gaze[°] 
		
		//FROM BACK		
		//		hpr[0] = 1.0f;
		//		hpr[1] = 0.0f;//-1.2f;
		//		hpr[2] = 0.0f;  // point of view[m]
		//		xyz[0] = dBodyGetPosition(torso[MIDDLE_BODY_SEGMENT].body)[0]-ceil(log2(BODY_SEGMENTS));
		//		xyz[1] = dBodyGetPosition(torso[MIDDLE_BODY_SEGMENT].body)[1];
		//		xyz[2] = dBodyGetPosition(torso[MIDDLE_BODY_SEGMENT].body)[2]+0.2f;  // gaze[°] 
		
		
		//FROM BELOW and BACK
		hpr[0] = 1.0f;
		hpr[1] = -1.2f;
		hpr[2] = 0.5f;  // point of view[m]
		
		xyz[0] = dBodyGetPosition(torso[MIDDLE_BODY_SEGMENT].body)[0]-BODY_SEGMENTS/2.0f;
		xyz[1] = dBodyGetPosition(torso[MIDDLE_BODY_SEGMENT].body)[1];
		xyz[2] = 0.1; // gaze[°]
 		dsSetViewpoint(xyz,hpr);		
	}
#endif
}
#endif

/******************************************/
/************* n-leg robot END ************/
/******************************************/

static void nearCallback(void *data, dGeomID o1, dGeomID o2) {
	
  dBodyID b1 = dGeomGetBody(o1), b2 = dGeomGetBody(o2);
	if (b1 && b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact)) return;
	
  bool neitherCollidingObjectsAreTheGround = false;
  if ((o1 != ground) && (o2 != ground)) neitherCollidingObjectsAreTheGround=true;
	
  //if(neitherCollidingObjectsAreTheGround) return; //only do collisions between something and the ground
	
  bool itsAFootTouching = false;
  if(o1 ==ground){
		printf("this code get run?");
		exit(30);
  } 
  if(o2 ==ground){
		for(int segment = 0; segment < BODY_SEGMENTS; ++segment) {
			for(int z=0;z<num_legs;z++){
				if (o1==leg[segment][z][2].geom){
					itsAFootTouching = true;
					thisLegIsTouching[segment][z]=1;  
				}
			}
		}
  }
  
  if(!neitherCollidingObjectsAreTheGround and !itsAFootTouching ) someNonFootIsTouchingTheGround = true;
  
  static const int N = 20;
  dContact contact[N];
  int n = dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));
  
	//if(neitherCollidingObjectsAreTheGround) return; //only do collisions between something and the ground
	/*  for(int segment = 0; segment < BODY_SEGMENTS; ++segment) {
	 for(int legNUM = 0; legNUM < num_legs; ++legNUM) {
	 for(int jointNUM = 0; jointNUM < num_joints; ++jointNUM) {
	 if(o1==leg[segment][legNUM][jointNUM].geom)
	 cerr <<"LEG o1: "<<segment << " " << legNUM << " "<<jointNUM <<endl;
	 if(o2==leg[segment][legNUM][jointNUM].geom)
	 cerr <<"LEG o2: "<<segment << " " << legNUM << " "<<jointNUM <<endl;				
	 }
	 }
	 if(o1 == torso[segment].geom)
	 cerr <<"TORSO o1: "<<segment <<endl;
	 if(o2 == torso[segment].geom)
	 cerr <<"TORSO o2: "<<segment <<endl;
	 }
	 */
	
	if (n > 0) {
    for (int i=0; i<n; i++) {
			//			cerr<<" i = " << i << " n = " << n<<endl;
      contact[i].surface.mode = dContactSoftERP | dContactSoftCFM;
      contact[i].surface.mu   = dInfinity; //2.0;
      contact[i].surface.soft_erp = 0.8;
      contact[i].surface.soft_cfm = 1e-5; 
			//			cerr<<"0\n";
      dJointID c = dJointCreateContact(world,contactgroup,&contact[i]);
			//			cerr<<"1\n";
      dJointAttach(c,b1,b2);
			//			cerr<<"2\n";
    }
		//		cerr<<"3\n";
  }
	//	cerr<<"4\n";
	//	if(neitherCollidingObjectsAreTheGround) return; //only do collisions between something and the ground
}


/*** P control tries to add a force that reduces the distance between
 the current joint angles and the desired value in the lookup table THETA. 
 ***/
void Pcontrol()
{
  dReal kp = 2.0;    //effects how quickly it tries to acheive the desired angle (original=2)
	//  dReal kp = 10.0;    //effects how quickly it tries to acheive the desired angle
  dReal fMax = 100.0; //fMax is the max for it can apply trying to acheive the desired angle
  
//#define ANG_VELOCITY
#ifndef ANG_VELOCITY
  for(int segment = 0; segment < BODY_SEGMENTS; ++segment) {
      for (int i = 0; i < num_legs; i++) {
	  for (int j = 0; j < num_links; j++) {
	      dReal tmp = dJointGetHingeAngle(leg[segment][i][j].joint);
	      dReal diff = THETA[segment][i][j] - tmp;
	      dReal u;
	      u = kp * diff;
	      
	      //cout << "\n\n***" << u << "***\n\n\n\n" << endl;

	      dJointSetHingeParam(leg[segment][i][j].joint,  dParamVel,
				  u);
	      dJointSetHingeParam(leg[segment][i][j].joint, dParamFMax,
				  fMax);
	  }
      }
#else //when using angular velocity
      for(int segment = 0; segment < BODY_SEGMENTS; ++segment) {
	  for (int i = 0; i < num_legs; i++) {
	      for (int j = 0; j < num_links; j++) {
		  //dReal tmp = dJointGetHingeAngle(leg[segment][i][j].joint);
		  //dReal diff = THETA[segment][i][j] - tmp;
		  //dReal u;
		  //u = kp * diff;
		  
		  dReal desiredValue = THETA[segment][i][j];
		  desiredValue *= 2.25;

		  //cout << "\n\n***" << desiredValue << "***\n\n\n\n" << endl;

		  dJointSetHingeParam(leg[segment][i][j].joint,  dParamVel,
				      desiredValue);
		  dJointSetHingeParam(leg[segment][i][j].joint, dParamFMax,
				      fMax);
	      }
	  }
	  
#endif
#ifdef USE_NLEG_ROBOT
		if(segment < BODY_SEGMENTS-1) {
			// applying torso joint force
			dReal tmp = dJointGetHingeAngle(torso[segment].joint); //joints might start in segment 2
			dReal diff = torsoJointDesiredAngle[segment] - tmp;
			
			dReal u;
			u = kp * diff;  
			dJointSetHingeParam(torso[segment].joint,  dParamVel, u);
			dJointSetHingeParam(torso[segment].joint, dParamFMax, fMax); 
		}
#endif        
	}
}

/*** Control of walking ***/
void walk()
{
  
	//  int numSheets;
	//  
	//  #ifdef HIDDEN_LAYER
	//  numSheets = 3;
	//  #else
	//  numSheets = 2;
	//  #endif
  
  timeSteps++;
	
  //jmc added to print out joint angles
	
  if(printJointAngles){
		static bool doOnce = true;
		if(doOnce){
			doOnce=false;
			//remove the file if it is there already
			ofstream jangle_file;        
			jangle_file.open ("jointAngles.txt", ios::trunc );
			jangle_file.close();      
		}
  }
	
	
	
  //clear out the network
  //LHZ  - When Recurrence is turned on, we multiply the input values by the recurrent values that were in 
  //the input nodes to begin with, if they are all 0, the network will never take a value. So we start it with
  //all 1's, since this will act as if ther was no recurrence for the first update. 
  
  substrate_pointer->reinitialize();
	
	if(experimentType != 33)  //If it is a NEAT experiment, the substrate can have an arbitrary number of layers, so we need to update 1+ExtraActivationUpdates num times
	{
	   substrate_pointer->dummyActivation();   //dummyActivation sets a flag so that the first update does not add on ExtraActivationUdates num updates
	}
	
  dReal roll, pitch, yaw;
	
	for(int segment = 0; segment < BODY_SEGMENTS; ++segment) {
		roll = pitch = yaw = 0.0;
		
		const dReal * torsoRotation = dBodyGetRotation(torso[segment].body);
		get_euler(torsoRotation, roll, pitch, yaw);
#if LEGSWING_ODE_DEBUG
		printf("roll: %f, pitch: %f, yaw: %f\n", roll, pitch, yaw); 
#endif
		
		for (int leg_no = 0; leg_no < num_legs; leg_no++) {
			for (int joint_no = 0; joint_no < num_joints; joint_no++) {
				
				
				dReal jangle = dJointGetHingeAngle(leg[segment][leg_no][joint_no].joint) - jointError[segment][leg_no][joint_no];  // hidding joint error for sensor
				
				int currDirectionPositive;
				
				if(jangle-lastJangle[segment][leg_no][joint_no] > 0.00001) currDirectionPositive =1; //we use a small positive number to ignore tinsy values
				else currDirectionPositive =0;
				
				//printf("jangle: %e, lastJangle_pneat[leg_no][joint_no]: %e, currDir: %i\n", jangle, lastJangle_pneat[leg_no][joint_no], currDirectionPositive);
				
				if(printJointAngles){
					ofstream jangle_file;        
					jangle_file.open ("jointAngles.txt", ios::app );
					jangle_file << jangle << " ";
					if(leg_no==num_legs-1 and joint_no==num_joints-1) jangle_file << endl;
					jangle_file.close();      
				}
				
				
				if(currDirectionPositive and !lastJangleDirectionPositve[segment][leg_no][joint_no]) numDirectionChanges++;
				if(!currDirectionPositive and lastJangleDirectionPositve[segment][leg_no][joint_no]) numDirectionChanges++;
				//if(visualize_pneat)  printf("numDirectionChanges_pneat %i \n", numDirectionChanges_pneat);
				
				lastJangleDirectionPositve[segment][leg_no][joint_no] =currDirectionPositive;
				lastJangle[segment][leg_no][joint_no] = jangle; 
				
				int xInt =joint_no; //which node we will be setting or getting. x & y are within the sheet, z specifies which sheet (0=input, 1=hidden, 2=output)
				int yInt =leg_no+(segment*2);     
				int zInt =0;        
				
#if LEGSWING_ODE_DEBUG				
				printf("About to set input (x: %i,y: %i,z: %i) to joint angle: %f \n", xInt, yInt, zInt, jangle);
#endif
				
				substrate_pointer->setValue(nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)],jangle);
								
				//<start> if inputting lastDesiredAngle    
				//xInt = xInt+1; //bump it one, cause the last desired angle goes into the input to the right of the current angle for that joint
				//#if LEGSWING_ODE_DEBUG
				//printf("about to set input (x: %i,y: %i,z: %i) to last desired joint angle: %f \n", xInt, yInt, zInt, newDesiredAngle[leg_no][joint_no]);
				//#endif
				
				//substrate_pointer->setValue(
				//                            nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)],
				//                            newDesiredAngle[segment][leg_no][joint_no]
				//                            );
			}
			
			//set whether the leg is touching the ground
			int xInt = 3;  //putting touch just to the right of the 4th legs lastAngle
			int yInt = leg_no+(segment*2); 
			int zInt = 0;
			
			substrate_pointer->setValue(nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)], thisLegIsTouching[segment][leg_no]);
		}
		
#ifndef LEGSWING_ODE_DEBUG  
		assert(num_legs==4); //warning: only works with four legs
		printf("Legs touching:  FrontLeft (%i), BackLeft (%i), BackRight (%i), FrontRight (%i)\n", thisLegIsTouching[segment][0],thisLegIsTouching[segment][1], thisLegIsTouching[segment][2], thisLegIsTouching[segment][3]); 
#endif
		
		
		
		//printf("timeStepDivisor_pneat: %f\n", timeStepDivisor_pneat);
		
		//  use if evolving sine period
		//  if(fabs(timeStepDivisor_pneat<1)) sineOfTimeStep =0; //prevent divide by zero errors, and allow them to silence the sine wave
		//  else sineOfTimeStep = sin(dReal(timeSteps_pneat)/timeStepDivisor_pneat)*M_PI;
		dReal sineOfTimeStep = sin(dReal(timeSteps)/50.0)*M_PI;

#ifdef USE_EXTRA_NODE
		dReal negSineOfTimeStep = (-1.0 * sineOfTimeStep);
#endif		

		if(visualize and sineOfTimeStep>.9999*M_PI) printf("****************************************************\n");
		if(visualize and sineOfTimeStep<-.9999*M_PI) printf("---------------------------------------------------\n");
		//printf("sineOfTimeStep: %f\n", sineOfTimeStep);
		
		
#ifndef USE_NLEG_ROBOT
		//<start> use to spread the PRYS horizontally
		//insert the roll, pitch and yaw to the 7th-9th columns of the 1st row
		//int yInt = 0; 
		//int zInt = 0;
		//dReal insertValue; 
		//
		//for(int q=0;q<3;q++){
		//    int xInt = 7+q;  
		//    if      (q==0) insertValue = roll;
		//    else if (q==1) insertValue = pitch;
		//    else           insertValue = yaw;
		//    substrate_pointer->setValue(
		//                                nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)],
		//                                insertValue
		//                                );
		//}
		//<ent> use to spread the PRYS horizontally
		
		//<start> to spread PRYS vertically
		//insert the roll, pitch and yaw to the 1st/0th-4th/3rd row of the 5th/4th columns 
		int xInt = 4;  
		int zInt = 0;
		dReal insertValue; 
		for(int q=0;q<3;q++){
			int yInt = q; 
			if      (q==0) insertValue = roll;
			else if (q==1) insertValue = pitch;
			else           insertValue = yaw;
			substrate_pointer->setValue(nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)], insertValue);
		}
		//<end>
		
		//insert a sine wave based on timeSteps
		xInt = 4;  
		int yInt = 3; 
		zInt = 0;

		substrate_pointer->setValue(nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)], sineOfTimeStep);
		
		
#else  // USE_NLEG_ROBOT
		//<start> to spread PRYS vertically
		//insert the roll, pitch and yaw to the 1st/0th-4th/3rd row of the 5th/4th columns 
		int xInt = 4;  
		int zInt = 0;
		dReal insertValue; 
		
		//roll	
		int yInt = segment*2; 
		substrate_pointer->setValue(nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)], roll);
		
		//pitch	
		yInt = (segment*2)+1; 
		substrate_pointer->setValue(nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)],	pitch);
		
		//yaw	
		xInt = 5;  
		yInt = segment*2; 
		substrate_pointer->setValue(nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)], yaw);
		//<end>
		if(segment < 1) 
		{
			//insert a sine wave based on timeSteps
			yInt = segment*2+1;
			
			substrate_pointer->setValue(nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)],	sineOfTimeStep);		
		} else
		{			
			dReal torsoAngle = dJointGetHingeAngle(torso[segment-1].joint);  //TODO: does torso zero have a valid joint?
			yInt = segment*2+1;
			substrate_pointer->setValue(nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)],	torsoAngle);	
		}
#endif
		
		
#if SUBSTRATE_DEBUG
		cout << "State of the nodes pre-update\n";
		for (int z=0;z<numSheets;z++)
		{
			cout << "Printing Layer " << z << "************************************************" << endl;
			for (int y1=0;y1<numNodesYglobal;y1++)
			{
				for (int x1=0;x1<numNodesXglobal;x1++)
				{
					//cout << "(x,y,z): (" << x1 << "," << y1 << "," << z << "): " ;
					cout << setw(12) << setiosflags(ios::right) << fixed << 
					double(substrate_pointer->getValue(
																						 nameLookupGlobal[HCUBE::Node(x1,y1,z)]
																						 ));
				}
				cout << endl;
			}
		}
		
		
		cout << "Updating Substrate\n";
		//CREATE_PAUSE(string("Line Number: ")+toString(__LINE__));
#endif
		
		//have to update the network once for each layer of *links* (or NumLayers -1)
#if SUBSTRATE_DEBUG
		for (int a=0;a< numSheets - 1 ;a++)
		{
			substrate_pointer->update(1);
			
			cout << "State of the nodes post-update: " << a << endl;
			for (int z=0;z<numSheets;z++)
			{
				cout << "Printing Layer " << z << endl;
				for (int y1=0;y1<numNodesYglobal;y1++)
				{
					for (int x1=0;x1<numNodesXglobal;x1++)
					{
						//cout << "(x,y,z): (" << x1 << "," << y1 << "," << z << "): " ;
						cout << double(substrate_pointer->getValue(nameLookupGlobal[HCUBE::Node(x1,y1,z)]));
						cout << " ";
					}
					cout << endl;
				}
			}
		}
#else
		substrate_pointer->update(numSheets-1);
#endif        
	}
	for(int segment = 0; segment < BODY_SEGMENTS; ++segment){
		for (int leg_no = 0; leg_no < num_legs; leg_no++) {
			for (int joint_no = 0; joint_no < num_joints; joint_no++) {
				
				int xInt =joint_no*2; //which node we will be setting or getting. x & y are within the sheet, z specifies which sheet (0=input, 1=hidden, 2=output)
				int yInt =leg_no;     
				int zInt = numSheets-1;         //2 (or numSheets - 1) to get the output
				
				newDesiredAngle[segment][leg_no][joint_no] = M_PI*double(substrate_pointer->getValue(nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)]));
				
#if LEGSWING_ODE_DEBUG
				printf("newDesiredAngle is: %f\n", newDesiredAngle[segment][leg_no][joint_no]);
#endif
				THETA[segment][leg_no][joint_no] = newDesiredAngle[segment][leg_no][joint_no] + jointError[segment][leg_no][joint_no];    // adding joint error
			}
		}
#ifdef USE_NLEG_ROBOT
		if(segment < BODY_SEGMENTS-1) {
			//get torso joint angle
			int xInt = 5;  
			int yInt = segment*2+1;
			int zInt = numSheets - 1;
			torsoJointDesiredAngle[segment] = M_PI*double(substrate_pointer->getValue(nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)]));
		}
#endif
		
		//get the timeStepDivisor
		//  xInt =10; //which node we will be setting or getting. x & y are within the sheet, z specifies which sheet (0=input, 1=hidden, 2=output)
		//  yInt =1;           
		//  zInt = 2;         //2 to get the output 
		//  timeStepDivisor = 100.0*double(substrate_pointer->getValue(
		//  nameLookupGlobal[HCUBE::Node(xInt,yInt,zInt)]));
		
		
		//if(visualize)printf("timeStepDivisor!: %f\n", timeStepDivisor);
		
		const dReal * torsoPosition = dBodyGetPosition(torso[segment].body);
				
		if(fabs(torsoPosition[0]) > maxXdistanceFrom0[segment]){
			maxXdistanceFrom0[segment] = fabs(torsoPosition[0]);
#ifndef LEGSWING_ODE_DEBUG
			printf("new maxX::::::::::::::::::::::::::::::::::::::::::::::: %f\n", maxXdistanceFrom0);
#endif
		}
		if(fabs(torsoPosition[1]) > maxYdistanceFrom0[segment]){
			maxYdistanceFrom0[segment] = fabs(torsoPosition[1]);
#ifndef LEGSWING_ODE_DEBUG
			printf("new maxY:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %f\n", maxYdistanceFrom0);
#endif
		}
		
		currentXdistanceFrom0[segment] = fabs(torsoPosition[0]);
		currentYdistanceFrom0[segment] = fabs(torsoPosition[1]);
	}
	
	Pcontrol(); 
	
	for(int segment = 0; segment < BODY_SEGMENTS; ++segment) {
		for(int z=0;z<num_legs;z++) thisLegIsTouching[segment][z]=0; //reset these flags before checking the new state of things
	}
	
	if(fitness_function == 2) {
		for(int segment = 0; segment < BODY_SEGMENTS; ++segment) {
			for (int i = 0; i < num_legs; i++) {
				for (int j = 0; j < num_links; j++) {
					totalForce[0] += dBodyGetForce(leg[segment][i][j].body)[0];
					totalForce[1] += dBodyGetForce(leg[segment][i][j].body)[1];
					totalForce[2] += dBodyGetForce(leg[segment][i][j].body)[2];
				}
			}
			totalForce[0] += dBodyGetForce(torso[segment].body)[0];
			totalForce[1] += dBodyGetForce(torso[segment].body)[1];
			totalForce[2] += dBodyGetForce(torso[segment].body)[2];
		}
	}	
}



/*** Simulation loop ***/
void simLoop(int pause)
{
	if (!pause and !pauseGlobal) {
		if(shouldIKeepRunningTheSimulation()){
			walk();                               // control of walking
			dSpaceCollide(space,0,&nearCallback); // collision detection
#ifndef USE_NLEG_ROBOT
			dWorldStep(world, 0.001);              // jmc: original was 0.01 but it was crashing
#else
			dWorldStep(world, 0.001);
			//			dWorldQuickStep(world, 0.001);              // jmc: original was 0.01 but it was crashing
#endif
			dJointGroupEmpty(contactgroup);       // Point of contact group sky
		}
#ifndef NOVIZ
		else dsStop();                            //end the simulation
#endif 
	}
#ifdef USE_NLEG_ROBOT
	drawRobot_Nleg();
#else
	drawRobot(); 
#endif
}


/*** Setting of point of view and camera gaze ***/
void start()
{
#ifndef NOVIZ
	xyz[0] = 1.0f;
	xyz[1] = -1.2f;
	xyz[2] = 0.5f;  // point of view[m]
	hpr[0] = 121.0f;
	hpr[1] = -10.0f;
	hpr[2] = 0.0f;  // gaze[°] 
	dsSetViewpoint(xyz,hpr);                // set point of view and gaze
	dsSetSphereQuality(3);
	dsSetCapsuleQuality(7);  // increase slows visualization
#endif
}


void setDrawStuff()
{
#ifndef NOVIZ
	fn.version = DS_VERSION;
	fn.start   = &start;
	fn.step    = &simLoop;
	fn.command = NULL;
	fn.path_to_textures = "../../../hyperneatlibs";//"../../drawstuff/textures";
	fn.command = &command;
#endif
	
}


double ODE_fitness() {
	switch (fitness_function) {
		case 0:  // standard distance
			return pow(2, pow(currentXdistanceFrom0[BODY_SEGMENTS/2], 2) + pow(currentYdistanceFrom0[BODY_SEGMENTS/2], 2));
		case 1:  // distance divided by numDirectionChanges
			if(numDirectionChanges == 0)
				return 0.0;
			return pow(2, pow(currentXdistanceFrom0[BODY_SEGMENTS/2], 2) + pow(currentYdistanceFrom0[BODY_SEGMENTS/2], 2)) / static_cast<double>(numDirectionChanges);
		case 2:  // distance divided by total force
			return pow(2, pow(currentXdistanceFrom0[BODY_SEGMENTS/2], 2) + pow(currentYdistanceFrom0[BODY_SEGMENTS/2], 2)) / sqrt(pow(totalForce[0], 2) + pow(totalForce[1], 2) + pow(totalForce[2], 2));
		default:
			cerr<<"\n\ninvalid fitness function\n";
			assert(false); // invalid fitness function
			return (0.00000001); //to avoid a compiler warning
			break;
	}
	
}
#endif //COMPILE_ODE
