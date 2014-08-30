/*
 *  HCUBE_ODE.h
 *  v2Hyperneat
 *
 *  Created by Benjamin Beckmann on 10/27/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HCUBE_ODE_H
#define HCUBE_ODE_H

//#define NOVIZ
//#define USE_NLEG_ROBOT  //torso is 1/2 the mass
#ifdef USE_NLEG_ROBOT
    #define BODY_SEGMENTS 4
#else
    #define BODY_SEGMENTS 1   
#endif
#define DISTANCE_BETWEEN_SEGMENTS 0.0
#define MIDDLE_BODY_SEGMENT BODY_SEGMENTS/2


#define HIDDEN_LAYER
//#define RECURRENT
//#define RECURRENT_SINE


#include "HCUBE_Defines.h"

#define dDOUBLE //using ode double precision  



#define LEGSWING_ODE_DEBUG (0)

//ODE CODE
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <ode/ode.h>

#ifndef NOVIZ
#include <drawstuff/drawstuff.h>
#endif

#include <math.h>

#ifdef dDOUBLE
#define dsDrawCapsule dsDrawCapsuleD
#define dsDrawBox     dsDrawBoxD
#define dsDrawLine    dsDrawLineD
#endif

extern dWorldID      world;        // world for kinetics calculation
extern dSpaceID      space;        // space for collision detection
extern dGeomID       ground;       // ground
extern dJointGroupID contactgroup; // Contact group
#ifndef NOVIZ
extern dsFunctions   fn;           // Drawing function of draw stuff
#endif

//globals to help HyperNEAT and ODE talk to each other 
#if LEGSWING_EXPERIMENT_ENABLE_BIASES
extern NEAT::FastBiasNetwork<double> * substrate_pointer;
#else 
extern NEAT::FastNetwork<double> * substrate_pointer;
#endif
#ifdef USE_NLEG_ROBOT
const int numNodesXglobal = 6;
const int numNodesYglobal = BODY_SEGMENTS*2;
#else
const int numNodesXglobal = 5;
const int numNodesYglobal = 4;
#endif
extern map<HCUBE::Node,string> nameLookupGlobal;

extern int experimentType;
extern int dimensionality;
extern int simulation_steps;
extern int fitness_function;
const int num_joints =3;
#ifdef USE_NLEG_ROBOT
extern int hingeType;
const int num_legs = 2;
#else
const int num_legs = 4;
#endif
const int num_links =3;  //num segments per leg
extern bool someNonFootIsTouchingTheGround;// = false;
extern bool userSaysQuit;// = false;
const dReal minHeightPercentage =.5;
extern int rewardOrg;// = 0;
extern int thisLegIsTouching [BODY_SEGMENTS][num_legs]; 
extern bool pauseGlobal;// = false;
extern int numDirectionChanges;// = 0;
extern int maxnumDirectionChanges;// = .16*simulation_steps;
extern dReal maxXdistanceFrom0[BODY_SEGMENTS];// = 0;
extern dReal maxYdistanceFrom0[BODY_SEGMENTS];// = 0;
extern dReal currentXdistanceFrom0[BODY_SEGMENTS];// = 0;
extern dReal currentYdistanceFrom0[BODY_SEGMENTS];// = 0;
extern bool visualize;// =false;
extern bool postHoc;// = false;
extern int timeSteps;
extern dReal timeStepDivisor;// = 200; //controls the sine period

extern dReal lastJangle                 [BODY_SEGMENTS][num_legs][num_joints];// = { {0,0,0},{0,0,0},{0,0,0},{0,0,0} };
extern int lastJangleDirectionPositve   [BODY_SEGMENTS][num_legs][num_joints];// = { {0,0,0},{0,0,0},{0,0,0},{0,0,0} };
extern double newDesiredAngle           [BODY_SEGMENTS][num_legs][num_joints];// = { {0,0,0},{0,0,0},{0,0,0},{0,0,0} };
#ifdef USE_NLEG_ROBOT
extern double torsoJointDesiredAngle    [BODY_SEGMENTS];
#endif

extern double jointError[BODY_SEGMENTS][num_legs][num_joints];
//extern int numJointsWithError;
extern int currentJointsWithError;
extern double widthOfErrorRange;

extern int numSheets;

typedef struct {
  dBodyID  body;
  dGeomID  geom;
  dJointID joint;
  dReal    m,r,x,y,z; // Mass and radius, position(x,y,z)
} MyLink;

extern MyLink leg[BODY_SEGMENTS][num_legs][num_links]; //Legs
extern MyLink torso[BODY_SEGMENTS]; //  and body
// The 'front' of the robot is the right side at the start, so forward walking would be to the right
// North is into the screen, South is down toward the keyboard and the typer
// leg[0] is front left (north east)
// leg[1] is back left
// leg[2] is back right
// leg[3] is front right 

extern dReal  THETA[BODY_SEGMENTS][num_legs][num_links];// = {{0},{0},{0},{0}}; // Target angle

//const dReal percentOfLegs =.25;
//
//const dReal  SX = 0, SY = 0, SZ = .2;           // Initial position of body center of gravity
////const dReal  l1 = 1.5*0.05, l2 = 0.3, l3  = 0.3;  // Leg segment lengths, l1=hip, l2=thigh, l3=calf
//const dReal  l1 = 1.5*0.05, l2 = 0.3*percentOfLegs, l3  = 0.3*percentOfLegs;  // Leg segment lengths, l1=hip, l2=thigh, l3=calf
//
//const dReal  lx = 0.5, ly= 0.3, lz = 0.05;         // body sides
//const dReal  r1 = 0.02, r2 = 0.02, r3 = 0.02 ;     // leg radius
//const dReal  cx1 = (lx-r1)/2, cy1 = (ly+l1)/2;     // Temporary variable
//
//
//
//// for four legs
//const dReal  c_x[num_legs][num_links] = {{ cx1, cx1, cx1},{-cx1,-cx1,-cx1}, // Joint central x coordinate
//{-cx1,-cx1,-cx1},{ cx1, cx1, cx1}};
//const dReal  c_y[num_legs][num_links] = {{ cy1, cy1, cy1},{ cy1, cy1, cy1}, // Joint central y coordinate
//{-cy1,-cy1,-cy1},{-cy1,-cy1,-cy1}};
//const dReal  c_z[num_legs][num_links] =  {{0, 0, -l2},                      // Joint central z coordinate
//{0, 0, -l2},{0, 0, -l2},{0, 0, -l2}};



void resetRelevantGlobals ();

void get_euler(const dReal * matrix,dReal &kx,dReal &ky,dReal &kz);      


bool shouldIKeepRunningTheSimulation ();

static void command (int cmd)
{
	if(cmd == 'r'){
		printf("Rewarded!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		rewardOrg += 1;        
	}
	if(cmd == 'q'){
		userSaysQuit = true;
	}
	if(cmd == 'p'){     
		if(pauseGlobal) pauseGlobal = false;
		else pauseGlobal = true;
	}
	
}

/*** Formation of robot ***/
void  makeRobot();
void  makeRobot_Nleg();

/*** Drawing of robot ***/
void drawRobot();
void drawRobot_Nleg();

static void nearCallback(void *data, dGeomID o1, dGeomID o2);

/*** P control tries to add a force that reduces the distance between
 the current joint angles and the desired value in the lookup table THETA. 
 ***/
void Pcontrol();

/*** Control of walking ***/
void walk();

/*** Simulation loop ***/
void simLoop(int pause);


/*** Setting of point of view and camera gaze ***/
void start();


void setDrawStuff();

double ODE_fitness();

#endif //HCUBE_ODE_H
