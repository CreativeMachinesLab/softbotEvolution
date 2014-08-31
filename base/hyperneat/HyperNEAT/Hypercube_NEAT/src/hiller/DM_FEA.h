#ifndef CDM_FEA_H
#define CDM_FEA_H
#include "DigitalMatter.h"
#include "DM_FRegion.h"

//Jonathan Hiller (jdh74@cornell.edu)
//VERSION 5

//different element types
#define BARSHEAR 0
#define FRAME 1

#define MAXDIR -1 //maximum... for viewing only!
#define XDIR 0
#define YDIR 1
#define ZDIR 2

struct INFO3D { float Max; float MaxX; float MaxY; float MaxZ;};  //info about maximum properties for the 3D stucture

class CDM_FEA
{
public: //variables and flags to be interacted with

	CDM_FEA(void);
	~CDM_FEA(void);

	void ImportObj(CDigitalMatter* ObjIn = NULL); //links to object, or re-loads
	void ResetFEA(bool ResetDMF = false); //clears everything and returns to initial state. (keeps linked DMF object, but can clear it, too.)
	
	void SaveFile(std::string filename);
	bool LoadFile(std::string filename);

	//I/O function for save/loading
	void WriteXML(CXML_Rip* pXML);
	void ReadXML(CXML_Rip* pXML);

	CDigitalMatter* pObj; //DMF object we are analyzing

	//boundary conditions
	void AddFixedRegion(Vec3D Location, Vec3D Size);
	void AddForcedRegion(Vec3D Location, Vec3D Size, Vec3D Force);
	void ClearBCs(void);

	int Element_type; //the type of element!
	bool WantForces; //calculate forces in post-processing step? (recomended: minimal overhead)
	void SetPardisoConsoleOutput(bool Display) {msglvl = Display;};
	void Solve(); //formulates and solves system!

	//Methods to get data back from the solution
	//get properties of a specific voxel (index px, py, pz) in a given direction (or maximum for that voxel)
	double GetMagForce(int px, int py, int pz, int dir = MAXDIR) {return GetMagProperty(px, py, pz, dir, F, MaxForces); } //returns force from a voxel in DMF x, y, z coords (or -1 if no voxel present)
	double GetMagDisp(int px, int py, int pz, int dir = MAXDIR) {return GetMagProperty(px, py, pz, dir, x, MaxDisps); } //returns displacement from a voxel in DMF x, y, z coords (or -1 if no voxel present)
	double GetMagReaction(int px, int py, int pz, int dir = MAXDIR) {return GetMagProperty(px, py, pz, dir, b, MaxReactions); } //returns external reaction force from a voxel in DMF x, y, z coords (or -1 if no voxel present)
	double GetMagStrain(int px, int py, int pz, int dir = MAXDIR) {return GetMagProperty(px, py, pz, dir, e, MaxStrains); } //returns internal strain from a voxel in DMF x, y, z coords (or -1 if no voxel present)
	
	//get properties of a specific voxel (index px, py, pz) in ALL directions (or maximum for that voxel)
	Vec3D GetForce(int x, int y, int z) {return Vec3D(GetMagForce(x, y, z, XDIR), GetMagForce(x, y, z, YDIR), GetMagForce(x, y, z, ZDIR));}; //returns displacement from a voxel in DMF x, y, z coords
	Vec3D GetDisp(int x, int y, int z) {return Vec3D(GetMagDisp(x, y, z, XDIR), GetMagDisp(x, y, z, YDIR), GetMagDisp(x, y, z, ZDIR));}; //returns displacement from a voxel in DMF x, y, z coords
	Vec3D GetReaction(int x, int y, int z) {return Vec3D(GetMagReaction(x, y, z, XDIR), GetMagReaction(x, y, z, YDIR), GetMagReaction(x, y, z, ZDIR));}; //returns displacement from a voxel in DMF x, y, z coords
	Vec3D GetStrain(int x, int y, int z) {return Vec3D(GetMagStrain(x, y, z, XDIR), GetMagStrain(x, y, z, YDIR), GetMagStrain(x, y, z, ZDIR));}; //returns displacement from a voxel in DMF x, y, z coords

	//gets the maximums for the entire structure
	double GetMaxForce(int dir = MAXDIR) {return GetMaxProperty(&Force, dir);}
	double GetMaxDisp(int dir = MAXDIR) {return GetMaxProperty(&Disp, dir);};
	double GetMaxReaction(int dir = MAXDIR) {return GetMaxProperty(&Reaction, dir);};
	double GetMaxStrain(int dir = MAXDIR) {return GetMaxProperty(&Strain, dir);};

	//double GetExtForce(int x, int y, int z, int dir = MAXDIR); //LEGACY!!! replaced with GetMagReaction

	Vec3D GetDisp(CDM_FRegion* pRegion); //average displacement of a region! 

private: //off limits variable and functions (internal)
	int DOFperBlock; //the dimension of each metablock
	int ELperDBlock; //the number of elements per metablock on diagonal
	int ELperOBlock; //the number of elements per metablock off diagonal

	int* Indi; //information about bonds: indicis
	int* Indj;
	int* BondDir; //the direction of each bond
	int* IndextoDOF; //maps each voxel to its degree of freedom...
	bool* FixedList; //keep track of which are fixed
	int NumFixed; //number of fixed
	int NumBonds;

	double* F; //to store forces in! (dimension = Num DOF)
	double* e; //to store strains in (dimension = Num DOF)

	//arrays to hold the euclidian maximum value for each voxel (from x, y, z)
	double GetMagProperty(int x, int y, int z, int dir, double* Ar, float* Maxs);
	float* MaxForces; //(dimension = number of voxels)
	float* MaxDisps; //(dimension = number of voxels)
	float* MaxReactions; //(dimension = number of voxels)
	float* MaxStrains;
	float* MaxSE; //maximum strain energy of any bond in this voxel (dimension = number of voxels)

	//output stuff:
	INFO3D Disp; //max displacements
	INFO3D Force; //max forces
	INFO3D Reaction; //max reactions
	INFO3D Strain; //max internal strains

	double GetMaxProperty(INFO3D* info3D, int dir);
	void FindMaxOverall(INFO3D* info3D, double* Ar, float* EachMax);

	std::vector <CDM_FRegion> Fixed;
	std::vector <CDM_FRegion> Forced;

	void CalcBonds(); //creates list of connecting voxel indicies!
	void CalcDOF(); //does some pre-processing to figure out DOF stuff

	void CalcStiffness(); //calculates the a (stiffness) matrix!
	void CalciA(); //calculates the ia matrix
	void CalcjA(); //calculates the ja matrix
	void CalcA(); //calculates the a matrix!
	void MakeBond(double* Ain, int BondIndex); //adds values for specified bond to the specified matrix
	void ImposeValA(int El1, int El2, int i, int j, float val, double* Ain, bool FullOnly = false); //adds a bond to the A matrix
	void ConsolidateA(); //gets rid of all the zeros for solving!
	void CalcForces();
	void RemoveDisconnected(void); //removes regions not connected to ground...

	void ApplyForces();

	//Pardiso variables:
	double* a; //values of sparse matrix for solving
	int* ja; //columns each value is in
	int* ia; //row index
	double* b;
	double* x;
	int DOF; //degrees of freedom

	int mtype; //defines matrix type
	int nrhs; //number of right-hand side vectors
	void *pt[64]; 
	int iparm[64];
	double dparm[64];
	int maxfct, mnum, phase, error, msglvl;

};

//types:
//BARSHEAR    FRAME X
//|# * *| x   |# * * * * *| x
//|* # *| y   |* # * * * #| y
//|* * #| z   |* * # * # *| z
//            |* * * # * *| Tx
//            |* * # * # *| Ty
//            |* # * * * #| Tz
//

#endif
