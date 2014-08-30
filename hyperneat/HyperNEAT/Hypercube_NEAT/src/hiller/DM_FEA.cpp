#include "DM_FEA.h"

//Jonathan Hiller (jdh74)
//VERSION 6

/*
#pragma comment (lib, "libpardiso_MKL_MINGW32_P.lib") //link to the Pardiso library
extern "C" int PARDISOINIT (void* pt, int* mtype, int* solver, int* iparm, double* dparm, int* error);
extern "C" int PARDISO (void* pt, int* maxfct, int* mnum, int* mtype, int* phase, int* n, double* a, int* ia, int* ja, int* perm, int* nrhs, int* iparm, int* msglvl, double* b, double* x, int* error, double* dparm);
*/



#ifdef AIX
#define F77_FUNC(func)  func     
#else
#define F77_FUNC(func)  func ## _
#endif

extern "C" {

/* PARDISO prototype. */
//extern  int F77_FUNC(PARDISOINIT)
extern  int F77_FUNC(pardisoinit)
    (void *, int *, int *, int *, double *, int *);
  
  //extern  int F77_FUNC(PARDISO)
extern  int F77_FUNC(pardiso)
    (void *, int *, int *, int *, int *, int *, 
     double *, int *, int *, int *, int *, int *, 
     int *, double *, double *, int *, double *);

}



CDM_FEA::CDM_FEA(void)
{
	Indi = NULL;
	Indj = NULL;
	BondDir = NULL;
	IndextoDOF = NULL;
	FixedList = NULL;
	F = NULL;
	e = NULL;
	MaxForces = NULL;
	MaxDisps = NULL;
	MaxReactions = NULL;
	MaxStrains = NULL;
	MaxSE = NULL;

	//Pardiso Params!
	//...that change
	a = NULL;
	ja = NULL;
	ia = NULL;
	b = NULL;
	x = NULL;
	DOF = -1;

	//...that don't change
	mtype = 2; // Real symmetric matrix 
	nrhs = 1; // Number of right hand sides.

	maxfct = 1; //Maximum number of numerical factorizations.
	mnum = 1; //Which factorization to use.
	msglvl = 1; //Print statistical information
	error = 0; //Initialize error flag

	int solver = 0; //use default (non-iterative) Pardiso solver

	//	PARDISOINIT(pt, &mtype, &solver, iparm, dparm, &error); //initialize pardiso
	//	F77_FUNC(PARDISOINIT)(pt, &mtype, &solver, iparm, dparm, &error); //initialize pardiso
	F77_FUNC(pardisoinit)(pt, &mtype, &solver, iparm, dparm, &error); //initialize pardiso

	pObj = NULL;
	
	Element_type = FRAME; //the type of element! (default)
	DOFperBlock = 0; //the dimension of each metablock
	ELperDBlock = 0; //the number of elements per metablock 
	ELperOBlock = 0; //the number of elements per metablock 

	ResetFEA();
}

CDM_FEA::~CDM_FEA(void)
{
	ResetFEA(); //clear all dynamic memory, among other things...
}

void CDM_FEA::ImportObj(CDigitalMatter* ObjIn) //links to object, or re-loads
{
	if (ObjIn != NULL)
		pObj = ObjIn; //set out object pointer!

	switch (Element_type){ //set the parameters for each element type.
	case BARSHEAR:
		DOFperBlock = 3;
		ELperDBlock = 3;
		ELperOBlock = 3;
		break;
	case FRAME:
		DOFperBlock = 6;
		ELperDBlock = 12;
		ELperOBlock = 18; //some wasted space, but oh well
		break;
	}

	int numVox = pObj->GetNumVox(); //number of voxels present
	int tmpNumDOF = numVox*DOFperBlock; //3DOF for each instantiated voxel
	if (DOF != tmpNumDOF){ //if we need to reallocate mem...
		ResetFEA(); //make sure dynamic arrays are deleted

		DOF = tmpNumDOF;
		b = new double[DOF];
		x = new double[DOF];
		if (WantForces) F = new double[DOF];
		if (WantForces) e = new double[DOF];
		MaxDisps = new float[numVox];
		if (WantForces){
			MaxForces = new float[numVox];
			MaxReactions = new float[numVox];
			MaxStrains = new float[numVox];
			MaxSE = new float[numVox];
		}
		Indi = new int[numVox*6]; //6 is for maximum number of connections per block
		Indj = new int[numVox*6];
		BondDir = new int[numVox*6];

		IndextoDOF = new int[pObj->GetStArraySize()];
		FixedList = new bool[numVox];

	}
	else { //just delete the pardiso matrices...
		if (a != NULL) delete [] a; a = NULL;
		if (ia != NULL) delete [] ia; ia = NULL;
		if (ja != NULL) delete [] ja; ja = NULL;
	}


}

void CDM_FEA::ResetFEA(bool ResetDMF) //clears everything and returns to initial state. (leaves DMF object linked)
{
	if (b != NULL) delete [] b; b = NULL;
	if (x != NULL) delete [] x; x = NULL;
	if (F != NULL) delete [] F; F = NULL;
	if (e != NULL) delete [] e; e = NULL;
	if (Indi != NULL) delete [] Indi; Indi = NULL;
	if (Indj != NULL) delete [] Indj; Indj = NULL;
	if (BondDir != NULL) delete [] BondDir; BondDir = NULL;
	if (IndextoDOF != NULL) delete [] IndextoDOF; IndextoDOF = NULL;
	if (FixedList != NULL) delete [] FixedList; FixedList = NULL;
	if (a != NULL) delete [] a; a = NULL;
	if (ia != NULL) delete [] ia; ia = NULL;
	if (ja != NULL) delete [] ja; ja = NULL;
	if (MaxForces != NULL) delete [] MaxForces; MaxForces = NULL;
	if (MaxDisps != NULL) delete [] MaxDisps; MaxDisps = NULL;
	if (MaxReactions != NULL) delete [] MaxReactions; MaxReactions = NULL;
	if (MaxStrains != NULL) delete [] MaxStrains; MaxStrains = NULL;
	if (MaxSE != NULL) delete [] MaxSE; MaxSE = NULL;

	DOF = -1;
	
	WantForces = true;

	ClearBCs();

	if(ResetDMF && pObj!= NULL) pObj->ClearMatter(); //clear the DMF if we want, but stay linked to it...
}

void CDM_FEA::SaveFile(std::string filename)
{
	CXML_Rip XML;
	WriteXML(&XML);
	XML.SaveFile(filename);
}

bool CDM_FEA::LoadFile(std::string filename)
{
	CXML_Rip XML;
	if (!XML.LoadFile(filename)) return false;

	ReadXML(&XML);
	return true;
}

void CDM_FEA::WriteXML(CXML_Rip* pXML)
{
	pXML->DownLevel("DMF_FEA");
	pXML->SetAttribute("Version", "1.0");
		pXML->DownLevel("Fixed_Regions");
		pXML->Element("NumFixed", (int)Fixed.size());
		for (int i=0; i<(int)Fixed.size(); i++){
			Fixed[i].WriteXML(pXML);
		}
		pXML->UpLevel();

		pXML->DownLevel("Forced_Regions");
		pXML->Element("NumForced", (int)Forced.size());
		for (int i=0; i<(int)Forced.size(); i++){
			Forced[i].WriteXML(pXML);
		}
		pXML->UpLevel();

		pObj->WriteXML(pXML);

	pXML->UpLevel();
}

void CDM_FEA::ReadXML(CXML_Rip* pXML)
{
	std::string ThisVersion = "0.93"; //version of the reading code... (JON WILL CHANGE THIS WHEN UPDATING SOURCE!)
	std::string Version;
	pXML->GetAttribute("Version", &Version);
	if (atof(Version.c_str()) > atof(ThisVersion.c_str())) std::cout << "Attempting to open newer version of DMF file!\n";

	/*
	std::string ThisVersion = "0.93"; //version of the reading code...
	std::string Version;
	pXML->GetAttribute("Version", &Version);
	if (atof(Version.c_str()) > atof(ThisVersion.c_str())) std::cout << "Attempting to open newer version of DMF file!\n";

	if (pXML->FindElement("Lattice")){
		Lattice.ReadXML(pXML);
		pXML->UpLevel();
	}
	if (pXML->FindElement("Voxel")){
		Voxel.ReadXML(pXML);
		pXML->UpLevel();
	}
	if (pXML->FindElement("Palette")){
		while (pXML->FindElement("Material")){
			CDMF_Material tmpMat;
			tmpMat.ReadXML(pXML, Version);
			Palette.push_back(tmpMat);
		}
		pXML->UpLevel(); //Material
	//	pXML->UpLevel(); //Palette
	}
	if (pXML->FindElement("Structure")){
		Structure.ReadXML(pXML, Version);
		pXML->UpLevel();
	}
	*/
}

void CDM_FEA::Solve() //formulates and solves system!
{
	RemoveDisconnected(); 
	CalcDOF();
	CalcBonds();
	CalcStiffness(); //jmc: think it crashes here
	ApplyForces();

	if (DOF != 0){
	    iparm[2]  = -1; //sets to defualt system value...

		double ddum = 0; //Double dummy var
		int idum = 0; //Integer dummy var

		//msglvl = 0; //don't output info!
		phase = 13;
		//		PARDISO(pt, &maxfct, &mnum, &mtype, &phase, &DOF, a, ia, ja, &idum, &nrhs, iparm, &msglvl, b, x, &error, dparm);
		//		F77_FUNC(PARDISO)(pt, &maxfct, &mnum, &mtype, &phase, &DOF, a, ia, ja, &idum, &nrhs, iparm, &msglvl, b, x, &error, dparm);
		F77_FUNC(pardiso)(pt, &maxfct, &mnum, &mtype, &phase, &DOF, a, ia, ja, &idum, &nrhs, iparm, &msglvl, b, x, &error, dparm);

		//if (error != 0) std::cout << "Pardiso error! (" << error << ") - Phase 1\n";
		if (error == -1) std::cout << "Pardiso error: Input inconsistent\n";
		else if (error == -2) std::cout << "Pardiso error: Not enough memory\n";	
		else if (error == -3) std::cout << "Pardiso error: Reodering Problem\n";	
		else if (error == -4) std::cout << "Pardiso error: Zero pivot, numerical factorization or iterative refinement problem\n";
		else if (error == -10) std::cout << "Pardiso error: No License file Pardiso.lic found\n";
		else if (error == -11) std::cout << "Pardiso error: License is expired\n";
		else if (error == -12) std::cout << "Pardiso error: Wrong username or hostname\n";

		phase = -1; /* Release internal memory. */
		//		PARDISO(pt, &maxfct, &mnum, &mtype, &phase, &DOF, &ddum, ia, ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error, dparm);
		//		F77_FUNC(PARDISO)(pt, &maxfct, &mnum, &mtype, &phase, &DOF, &ddum, ia, ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error, dparm);
		F77_FUNC(pardiso)(pt, &maxfct, &mnum, &mtype, &phase, &DOF, &ddum, ia, ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error, dparm);
	}

	//CalcMaxDisps();
	FindMaxOverall(&Disp, x, MaxDisps);

	if (WantForces)
		CalcForces();

//	OutputMatrices();
	if (a != NULL) {delete [] a; a = NULL;}
	if (ia != NULL) {delete [] ia; ia = NULL;}
	if (ja != NULL) {delete [] ja; ja = NULL;}

}

void CDM_FEA::CalcDOF()
{
	//pre compute DOFtoIndex
	int DOFInd = 0;
	for (int i=0; i<pObj->GetStArraySize(); i++){ //for all posible voxels:
		if (pObj->GetVoxel(i) != 0){ //if there's a voxel here...
			IndextoDOF[i] = DOFInd++;
		}
		else IndextoDOF[i] = -1;
	}
}


void CDM_FEA::CalcBonds() //creates list of connecting voxel indicies!
{
	NumBonds = 0;
	int tmpIndex = 0;

	for (int i=0; i<pObj->GetStArraySize(); i++){ //for every possible voxel
		if (pObj->GetVoxel(i) != 0){ //if there's a voxel here...
			for (int Dim = 0; Dim < 3; Dim++){ //check each possible connection (x, y, z). Only in positive direction so we only get each bond once
				switch (Dim) {
					case XDIR: //X direction
						tmpIndex = i+1; //index of voxel to check
						if (tmpIndex % pObj->GetVXDim() == 0) //account for edge condition
							tmpIndex = pObj->GetStArraySize(); //set to high!
						
						break;
					case YDIR: //Y direction
						tmpIndex = i + pObj->GetVXDim(); //index of voxel to check
						if ((int)(tmpIndex/pObj->GetVXDim()) % pObj->GetVYDim() == 0) //account for edge condition
							tmpIndex = pObj->GetStArraySize(); //set to high!

						break;
					case ZDIR: //Z direction
						tmpIndex = i + pObj->GetVXDim()*pObj->GetVYDim(); //index of voxel to check
				}
				if (tmpIndex < pObj->GetStArraySize() && pObj->GetVoxel(tmpIndex) != 0){ //if the voxel to check is instantiated...
					BondDir[NumBonds] = Dim;
					Indi[NumBonds] = i; //this is voxel one of the bond
					Indj[NumBonds++] = tmpIndex; //this is voxel 2 of the bond
				}
			}
		}
	}
}

void CDM_FEA::CalcStiffness() //calculates the big stiffness matrix!
{
	int NumEl = DOF/DOFperBlock*ELperDBlock + NumBonds*ELperOBlock; // - NumFixed; //approximate number of metablocks in our K matrix (overestimates slightly)

	if (a != NULL) {delete [] a; a = NULL;}
	a = new double[NumEl]; //values of sparse matrix
	if (ja != NULL) {delete [] ja; ja = NULL;}
	ja = new int[NumEl]; //columns each value is in
	if (ia != NULL) {delete [] ia; ia = NULL;}
	ia = new int[DOF+1]; //row index

	for (int i=0; i<NumEl; i++){ a[i] = 0; } //initialize A;

	CalciA();
	CalcjA();
	CalcA();
	ConsolidateA(); //removes zeros, help, but only marginally. (maybe pardiso does this internally?)

}

void CDM_FEA::CalciA() //populate ia!
{
	int IndIndex = 0;
	int iaIndex = 0;
	ia[iaIndex] = 1; //first element is always 1!

	int NumLocBonds; //number of bonds in a given row 
	int NumToAdd; //number to add

	for (int i=0; i<DOF/DOFperBlock; i++){ //iterate through each meta-row
		NumLocBonds = 0; //number of bonds in this row 

		while (IndIndex<NumBonds && IndextoDOF[Indi[IndIndex]] <= i){ //calculate how many bonds involve this block
			NumLocBonds++;
			IndIndex++;
		}

		for (int j=0; j<DOFperBlock; j++){ //now go through each row of the metarow
			if (Element_type == BARSHEAR)
				ia[++iaIndex] = ia[iaIndex-1] + 1 + NumLocBonds; //add number of elements in this row!
			else { //if Element_type == FRAME
				NumToAdd = 1 + 3*(NumLocBonds);; //1 for diagonal elements, 3*(NumLocBonds) = 3 more elements for every bond metablock
				if (j<3) NumToAdd +=2; //the off-diag terms of diagonal metablocks
				ia[++iaIndex] = ia[iaIndex-1] + NumToAdd; //add number of elements in this row!
			}
		}
	}
}

void CDM_FEA::CalcjA() //calculate ja!
{
	int IndIndex = 0;
	int CurInd;
	int NumRowBonds; //number of bonds in this row

	int jaIndex; //tmp ja index
	int ElNum; //tmp number of elements

	for (int i=0; i<DOF/DOFperBlock; i++){ //iterate through each meta-row
		NumRowBonds = 0; //number of bonds in this row

		//diagonal elements
		for (int j=0; j<DOFperBlock; j++){ //now go through each row of the metarow
			//all diagonal elements are set no matter what...+
			CurInd = i*DOFperBlock+j;
			jaIndex = ia[CurInd]-1;
			ja[jaIndex] = CurInd +1; //this will get redone for more complicated elements...
			
			if (Element_type == FRAME){
				switch (j){
					case 0:
						ja[++jaIndex] = CurInd + 5; //+4 for offset to next term... + 1 for silly offset (1-based index)
						ja[++jaIndex] = CurInd + 6; //+5
						break;
					case 1:
						ja[++jaIndex] = CurInd + 3; //+2
						ja[++jaIndex] = CurInd + 5; //+4
						break;
					case 2:
						ja[++jaIndex] = CurInd + 2; //+1
						ja[++jaIndex] = CurInd + 3; //+2
						break;
				}
			}
		}

		//for each bond
		while (IndIndex<NumBonds && IndextoDOF[Indi[IndIndex]] <= i){ //calculate how many bonds involve this 
			for (int j=0; j<DOFperBlock; j++){ //now go through each row of the metarow
				if (Element_type == BARSHEAR){
					jaIndex = ia[i*DOFperBlock+j] + NumRowBonds;
					ja[jaIndex] = IndextoDOF[Indj[IndIndex]]*DOFperBlock+j+1; //this will get overwritten with more complicated types...
				}
				else if (Element_type == FRAME){
					if (j<3) ElNum = 3; //number of elements taken in the diagonal block...
					else ElNum = 1;
					ElNum += 3*NumRowBonds;

					jaIndex = ia[i*DOFperBlock+j]-1 + ElNum;
					CurInd = IndextoDOF[Indj[IndIndex]]*DOFperBlock+j;

					switch (j){
						case 0:
							ja[jaIndex] = CurInd+1; //+0 (after accounting for 1-based index)
							ja[++jaIndex] = CurInd+5; //+4
							ja[++jaIndex] = CurInd+6; //+5
							break;
						case 1:
							ja[jaIndex] = CurInd+1; //+0
							ja[++jaIndex] = CurInd+3; //+2
							ja[++jaIndex] = CurInd+5; //+4
							break;
						case 2:
							ja[jaIndex] = CurInd+1; //+0
							ja[++jaIndex] = CurInd+2; //+1
							ja[++jaIndex] = CurInd+3; //+2
							break;
						case 3:
							ja[jaIndex] = CurInd-1; //-2
							ja[++jaIndex] = CurInd; //-1
							ja[++jaIndex] = CurInd+1; //+0
							break;
						case 4:
							ja[jaIndex] = CurInd-3; //-4
							ja[++jaIndex] = CurInd-1; //-2
							ja[++jaIndex] = CurInd+1; //+0
							break;
						case 5:
							ja[jaIndex] = CurInd-4; //-5
							ja[++jaIndex] = CurInd-3; //-4
							ja[++jaIndex] = CurInd+1; //+0
							break;
					}
				}
			}

			IndIndex++;
			NumRowBonds++;
		}
	}
}

void CDM_FEA::CalcA() //populate "a" matrix!
{
	for(int b=0; b<NumBonds; b++){ //for each bond!
		MakeBond(a, b);
	}
}


void CDM_FEA::MakeBond(double* Ain, int BondIndex)
{
	bool FullOnlyFlag;

	int El1 = IndextoDOF[Indi[BondIndex]];
	int El2 = IndextoDOF[Indj[BondIndex]];
	if (Ain != a) { //tmp
		El1 = 0;
		El2 = 1;
	}

	int BondDirec = BondDir[BondIndex]; //which direction is this?

	float E1 = (pObj->GetLeafMat(Indi[BondIndex]))->GetEMod(); //elastic modulus
	float v1 = 0.33f; //(pObj->GetLeafMat(Indi[i]))->Poissons_Ratio; //poissons ratio
	float G1 = E1/(2*(1+v1)); //shear modulus [E = 2G(1+v)) -> G = E/(2*(1+v))]
	float E2 = (pObj->GetLeafMat(Indj[BondIndex]))->GetEMod();
	float v2 = 0.33f; //(pObj->GetLeafMat(Indj[i]))->Poissons_Ratio;
	float G2 = E2/(2*(1+v2));

	float E = (E1*E2/(E1+E2))*2; //composite elastic modulus: series spring equation... from 2 materials
	float G = (G1*G2/(G1+G2)) *2;	//composite shear modulus: series spring equation... from 2 materials
	float L = (float)pObj->GetLatticeDim(); //length between points (lattice_Dim

	if (Element_type == BARSHEAR){
		float NormalStiff = E*L; //k=EA/l
		float ShearStiff = G*L; //series spring equation... from 2 materials, G=F*I/dX*A

		//diagonal of element 1:
		for (int i=0; i<DOFperBlock; i++){ //for each individual direction of freedom
			float ToAdd = ShearStiff; //add the shear...
			if (i==BondDirec) ToAdd = NormalStiff; //unless this is normal direction
			ImposeValA(El1, El1, i, i, ToAdd, Ain);
			ImposeValA(El2, El2, i, i, ToAdd, Ain);

			FullOnlyFlag = true;
			if (!FixedList[El1] && !FixedList[El2]) //if neither element is fixed
				FullOnlyFlag = false;
			
			ImposeValA(El1, El2, i, i, -ToAdd, Ain, FullOnlyFlag);

		}
	}
	else if (Element_type == FRAME){

		float a1 = E*L; //EA/L
		float a2 = G*L*L*L/6; //GJ/L, J=BH/12*(BB+HH) = L^4/6
		float b1 = E*L; //12EI/L^3, I=bh^3/12 = L^4/12
		float b2 = E*L*L/2; //6EI/L^2
		float b3 = E*L*L*L/6; //2EI/L

		if (BondDirec == XDIR){
			//upper diagonal
			ImposeValA(El1, El1, 0, 0, a1, Ain);
			ImposeValA(El1, El1, 1, 1, b1, Ain);
			ImposeValA(El1, El1, 5, 1, b2, Ain);
			ImposeValA(El1, El1, 2, 2, b1, Ain);
			ImposeValA(El1, El1, 4, 2, -b2, Ain);
			ImposeValA(El1, El1, 3, 3, a2, Ain);
			ImposeValA(El1, El1, 4, 4, 2*b3, Ain);
			ImposeValA(El1, El1, 5, 5, 2*b3, Ain);

			//lower diagonal
			ImposeValA(El2, El2, 0, 0, a1, Ain);
			ImposeValA(El2, El2, 1, 1, b1, Ain);
			ImposeValA(El2, El2, 5, 1, -b2, Ain);
			ImposeValA(El2, El2, 2, 2, b1, Ain);
			ImposeValA(El2, El2, 4, 2, b2, Ain);
			ImposeValA(El2, El2, 3, 3, a2, Ain);
			ImposeValA(El2, El2, 4, 4, 2*b3, Ain);
			ImposeValA(El2, El2, 5, 5, 2*b3, Ain);

			//off diagonal
			FullOnlyFlag = true;
			if (!FixedList[El1] && !FixedList[El2])//{ //don't add if fixed! (if both are not fixed...)
				FullOnlyFlag = false;

			ImposeValA(El1, El2, 0, 0, -a1, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 1, 1, -b1, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 5, 1, b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 2, 2, -b1, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 4, 2, -b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 3, 3, -a2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 2, 4, b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 4, 4, b3, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 1, 5, -b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 5, 5, b3, Ain, FullOnlyFlag);
		}

		else if (BondDirec == YDIR){
			//upper diagonal
			ImposeValA(El1, El1, 0, 0, b1, Ain);
			ImposeValA(El1, El1, 5, 0, -b2, Ain);
			ImposeValA(El1, El1, 1, 1, a1, Ain);
			ImposeValA(El1, El1, 2, 2, b1, Ain);
			ImposeValA(El1, El1, 3, 2, b2, Ain);
			ImposeValA(El1, El1, 3, 3, 2*b3, Ain);
			ImposeValA(El1, El1, 4, 4, a2, Ain);
			ImposeValA(El1, El1, 5, 5, 2*b3, Ain);

			//lower diagonal
			ImposeValA(El2, El2, 0, 0, b1, Ain);
			ImposeValA(El2, El2, 5, 0, b2, Ain);
			ImposeValA(El2, El2, 1, 1, a1, Ain);
			ImposeValA(El2, El2, 2, 2, b1, Ain);
			ImposeValA(El2, El2, 3, 2, -b2, Ain);
			ImposeValA(El2, El2, 3, 3, 2*b3, Ain);
			ImposeValA(El2, El2, 4, 4, a2, Ain);
			ImposeValA(El2, El2, 5, 5, 2*b3, Ain);

			//off diagonal
			FullOnlyFlag = true;
			if (!FixedList[El1] && !FixedList[El2])//{ //don't add if fixed! (if both are not fixed...)
				FullOnlyFlag = false;

			ImposeValA(El1, El2, 0, 0, -b1, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 5, 0, -b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 1, 1, -a1, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 2, 2, -b1, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 3, 2, b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 2, 3, -b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 3, 3, b3, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 4, 4, -a2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 0, 5, b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 5, 5, b3, Ain, FullOnlyFlag);
		}

		else if (BondDirec == ZDIR){
			//upper diagonal
			ImposeValA(El1, El1, 0, 0, b1, Ain);
			ImposeValA(El1, El1, 4, 0, b2, Ain);
			ImposeValA(El1, El1, 1, 1, b1, Ain);
			ImposeValA(El1, El1, 3, 1, -b2, Ain);
			ImposeValA(El1, El1, 2, 2, a1, Ain);
			ImposeValA(El1, El1, 3, 3, 2*b3, Ain);
			ImposeValA(El1, El1, 4, 4, 2*b3, Ain);
			ImposeValA(El1, El1, 5, 5, a2, Ain);

			//lower diagonal
			ImposeValA(El2, El2, 0, 0, b1, Ain);
			ImposeValA(El2, El2, 4, 0, -b2, Ain);
			ImposeValA(El2, El2, 1, 1, b1, Ain);
			ImposeValA(El2, El2, 3, 1, b2, Ain);
			ImposeValA(El2, El2, 2, 2, a1, Ain);
			ImposeValA(El2, El2, 3, 3, 2*b3, Ain);
			ImposeValA(El2, El2, 4, 4, 2*b3, Ain);
			ImposeValA(El2, El2, 5, 5, a2, Ain);

			//off diagonal
			FullOnlyFlag = true;
			if (!FixedList[El1] && !FixedList[El2])//{ //don't add if fixed! (if both are not fixed...)
				FullOnlyFlag = false;

			ImposeValA(El1, El2, 0, 0, -b1, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 4, 0, b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 1, 1, -b1, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 3, 1, -b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 2, 2, -a1, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 1, 3, b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 3, 3, b3, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 0, 4, -b2, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 4, 4, b3, Ain, FullOnlyFlag);
			ImposeValA(El1, El2, 5, 5, -a2, Ain, FullOnlyFlag);
		}
	}
}

void CDM_FEA::ImposeValA(int El1, int El2, int i, int j, float val, double* Ain, bool FullOnly) //adds a bond to the A matrix
{
	if (Ain == a){ //if we're making the main matrix
		//assumes IA, JA, set up to include this position!!!!!
		//El1, El2 are element indices of meta-block, i, j are sub-indicies, val is value to impose, FullOnly if we don't want in a matrix to solve... 
		//get the index of a:
		int ThisIndex = ia[El1*DOFperBlock+j]-1; //the row we want!
		while (ja[ThisIndex] != El2*DOFperBlock+i + 1) //go through jA until we find the right index!
			ThisIndex++;

		if (!FullOnly)
			a[ThisIndex] += val;

	}
	else { //if making little post-processing matrices
		Ain[(El2*DOFperBlock+i) + 2*DOFperBlock*(El1*DOFperBlock+j)] += val;
	}

}

void CDM_FEA::ConsolidateA() //gets rid of all the zeros for quicker solving! (maybe...)
{
	int index = 0; //master index of longer arrays
	int Shift = 0;

	for (int i=0; i<DOF; i++){ //for each element of ia;
		ia[i+1] -= Shift;
		while (index < ia[i+1]-1){ //for everything on this row...
			while (a[index + Shift] == 0){ //if this is an element to remove
				Shift++;
				ia[i+1] --;
			}
			a[index] = a[index+Shift];
			ja[index] = ja[index+Shift];

			index++;
		}
	}
}

void CDM_FEA::ApplyForces()
{
	int* Sizes = new int[(int)Forced.size()]; //holds how many voxels are within each forcing region
	for (int i=0; i<(int)Forced.size(); i++)
		Sizes[i] = 0;
	
	Vec3D point;

	for (int i=0; i<DOF; i++)
		b[i] = 0;

	//first, we have to count the number of voxels in the area to decide how much force each gets.
	for (int i=0; i<pObj->GetStArraySize(); i++){
		if (pObj->GetVoxel(i) != 0){ //if there's a voxel here...
			pObj->GetXYZ(&point, i);
			for (int j=0; j<(int)Forced.size(); j++){ //for each forcing region
				if (Forced[j].pRegion->IsTouching(&point, (float)(pObj->GetLatticeDim()/2))){
					Sizes[j]++;
				}
			}
		}
	}

	for (int i=0; i<pObj->GetStArraySize(); i++){
		if (pObj->GetVoxel(i) != 0){ //if there's a voxel here...
			pObj->GetXYZ(&point, i);

			for (int j=0; j<(int)Forced.size(); j++){
				int size = Sizes[j];
				(void) size; //to get rid of unused variable warning
				if (Forced[j].pRegion->IsTouching(&point, (float)(pObj->GetLatticeDim()/2))){
					b[IndextoDOF[i]*DOFperBlock]+=Forced[j].Force.x/Sizes[j];
					b[IndextoDOF[i]*DOFperBlock+1]+=Forced[j].Force.y/Sizes[j];
					b[IndextoDOF[i]*DOFperBlock+2]+=Forced[j].Force.z/Sizes[j];
				}
			}
		}
	}

	delete[] Sizes;
	Sizes = NULL;

}

void CDM_FEA::CalcForces() //fills in forces, reaction forces, and strains!!!
{
	for (int i=0; i<DOF; i++){ //set all to zero force
		F[i] = 0;
		e[i] = 0; //internal strains
		b[i] = 0; //do reactions here.... (if we want)
	}
	for (int i=0; i<pObj->GetNumVox(); i++){ 
		MaxSE[i] = 0; 
	}
	
	//set up a smaller F = Kx matrix involving just the two blocks involved in each bond (to calculate "F")
	double* TmpK = new double[4*DOFperBlock*DOFperBlock]; //technically only need the upper diagonal
	double* TmpX = new double[2*DOFperBlock]; 
	double* TmpF = new double[2*DOFperBlock];

	float* FBig = new float [DOF*3]; //one for each dimension, so we can keep track of multiple bonds getting added up & average
	for (int i=0; i<3*DOF; i++){FBig[i] = 0;} //initialize to zeros

	for(int bond=0; bond<NumBonds; bond++){ //for each bond!
		int BondDirec = BondDir[bond]; //which direction is this?
		int El1 = IndextoDOF[Indi[bond]];
		int El2 = IndextoDOF[Indj[bond]];

		for (int i=0; i<4*DOFperBlock*DOFperBlock; i++) //zero out them all
			TmpK[i] = 0; 
		for (int i=0; i<2*DOFperBlock; i++){
			TmpX[i] = 0; 
			TmpF[i] = 0; 
		}

		MakeBond(TmpK, bond);

		for (int i=0; i<DOFperBlock; i++){ //set up our little displacement vector
			TmpX[i] = x[El1*DOFperBlock+i];
			TmpX[DOFperBlock + i] = x[El2*DOFperBlock+i];
		}

		//multiply out!
		for (int i=0; i<2*DOFperBlock; i++){ //cycle through each row
			for (int j=i; j<2*DOFperBlock; j++){ //cycle through each column (only for upper diag)
				TmpF[i] += (TmpK[i*2*DOFperBlock+j]*TmpX[j]);
				if (i != j)
					TmpF[j] += (TmpK[i*2*DOFperBlock+j]*TmpX[i]);
			}
		}

		float StrainEnergy = 0;
		for (int i=0; i<2*DOFperBlock; i++){ //cycle through each row
			StrainEnergy += (float)(TmpX[i]*TmpF[i]); //abs value already
		}

		//StrainEnergy = abs(StrainEnergy); //only care about magnitude!
		//check against both bonds!
		if (StrainEnergy > MaxSE[El1])
			MaxSE[El1] = StrainEnergy;
		if (StrainEnergy > MaxSE[El2])
			MaxSE[El2] = StrainEnergy;

		for (int i=0; i<DOFperBlock; i++) { //local forces to universal coordinates
			int CurIndex = DOF*BondDirec + El1*DOFperBlock+i; //first element
			if (FBig[CurIndex] == 0)
				FBig[CurIndex] = (float)abs(TmpF[i]);
			else
				FBig[CurIndex] = (float)(FBig[CurIndex] + abs(TmpF[i]))/2;

			CurIndex = DOF*BondDirec + El2*DOFperBlock+i;

			if (FBig[CurIndex] == 0)
				FBig[CurIndex] = (float)abs(TmpF[DOFperBlock + i]);
			else
				FBig[CurIndex] = (float)(FBig[CurIndex] + abs(TmpF[DOFperBlock + i]))/2;


			b[El1*DOFperBlock+i] += TmpF[i];
			b[El2*DOFperBlock+i] += TmpF[DOFperBlock + i];
		}

		double LinearStrain = abs(x[El1*DOFperBlock + BondDirec] - x[El2*DOFperBlock + BondDirec])/pObj->GetLatticeDim(); //simple volumetric strain (no shear...)
		if (LinearStrain > e[El1*DOFperBlock + BondDirec]) e[El1*DOFperBlock + BondDirec] = LinearStrain;
		if (LinearStrain > e[El2*DOFperBlock + BondDirec]) e[El2*DOFperBlock + BondDirec] = LinearStrain;

	}

	//render FBig to F!
	for (int i=0; i<DOF; i++){ //each element
		for (int j=0; j<3; j++){ //each direction of possible bond
			F[i] += FBig[DOF*j + i];
		}
	}

	delete[]TmpK;
	TmpK = NULL;
	delete[]TmpX;
	TmpX = NULL;
	delete[]TmpF;
	TmpF = NULL;
	delete[]FBig;
	FBig = NULL;

	FindMaxOverall(&Reaction, b, MaxReactions);
	FindMaxOverall(&Force, F, MaxForces);
	FindMaxOverall(&Strain, e, MaxStrains);


}

void CDM_FEA::FindMaxOverall(INFO3D* info3D, double* Ar, float* EachMax) //calculate maximums overall and for each voxel
{
	info3D->Max = 0;
	info3D->MaxX = 0;
	info3D->MaxY = 0;
	info3D->MaxZ = 0;

	for(int i=0; i<DOF; i++){
		switch (i%DOFperBlock){
		case 0: //X
			if (abs(Ar[i]) > info3D->MaxX) info3D->MaxX = (float)abs(Ar[i]); 

			//maximum (could do anywhere, but here for convenience)
			EachMax[i/DOFperBlock] = (float)sqrt(Ar[i]*Ar[i] + Ar[i+1]*Ar[i+1] + Ar[i+2]*Ar[i+2]);
			if (EachMax[i/DOFperBlock] > info3D->Max) info3D->Max = EachMax[i/DOFperBlock];

			break;
		case 1: //Y
			if (abs(Ar[i]) > info3D->MaxY) info3D->MaxY = (float)abs(Ar[i]); break;
		case 2: //Z
			if (abs(Ar[i]) > info3D->MaxZ) info3D->MaxZ = (float)abs(Ar[i]); break;
		}
	}
}

double CDM_FEA::GetMagProperty(int px, int py, int pz, int dir, double* Ar, float* Maxs)
{
	if (Ar != x && (!WantForces || Ar == NULL)) //if looking for a force-derived output, but doesn't exist
		std::cout << "Set \"WantForces\" flag to true & restart to calculate internal forces!\n";

	int index = pObj->GetIndex(px, py, pz);
	if (index != -1 && pObj->GetVoxel(index) != 0){ //if a valid index...
		if (dir == XDIR) //return X force
			return Ar[(int)(IndextoDOF[index]*DOFperBlock+XDIR)];
		else if (dir == YDIR) //return Y force
			return Ar[IndextoDOF[index]*DOFperBlock+YDIR];
		else if (dir == ZDIR) //return Z force
			return Ar[IndextoDOF[index]*DOFperBlock+ZDIR];
		else //return magnitude
			return Maxs[IndextoDOF[index]];
	}
	else
		return -1;
}

double CDM_FEA::GetMaxProperty(INFO3D* info3D, int dir)
{
	switch (dir){
		case XDIR: return info3D->MaxX; break;
		case YDIR: return info3D->MaxY; break;
		case ZDIR: return info3D->MaxZ; break;
		case MAXDIR: return info3D->Max; break;
	}
	return -1; //if error...
}

Vec3D CDM_FEA::GetDisp(CDM_FRegion* pRegion) //average displacement of a region! 
{
	CDM_FRegion Region = *pRegion;

	Vec3D WS = pObj->GetWorkSpace();
	Region.ScaleTo(Vec3D(1,1,1), pObj->GetWorkSpace());
	Vec3D Pos;
	Vec3D Dist = Vec3D(0,0,0);
	int NumAdded = 0;
	for (int i=0; i< pObj->GetStArraySize(); i++){
		if (pObj->GetVoxel(i) != 0){ //if there's a voxel here...
			pObj->GetXYZ(&Pos, i);
			
			if(Region.pRegion->IsTouching(&Pos, (float)(pObj->GetLatticeDim()/2))){ //if touching
				Vec3D tmp(x[IndextoDOF[i]*DOFperBlock+XDIR], x[IndextoDOF[i]*DOFperBlock+YDIR], x[IndextoDOF[i]*DOFperBlock+ZDIR]);
				Dist += tmp;
				NumAdded++;
			}
		}
	}
	if (NumAdded == 0)
		return Vec3D(0,0,0);
	else 
		return Dist/NumAdded;
}



void CDM_FEA::AddFixedRegion(Vec3D Location, Vec3D Size) //fixes a region of voxels
{
	CDM_FRegion MyRegion;
	MyRegion.CreateBoxRegion(Location, Size);
	MyRegion.SetColor(1.0f, 0.3f, 0.3f, 0.5f);
	MyRegion.Fixed = true;
	Fixed.push_back(MyRegion);
}

void CDM_FEA::AddForcedRegion(Vec3D Location, Vec3D Size, Vec3D Force) //applies a force to a region of voxels
{
	CDM_FRegion MyRegion;
	MyRegion.CreateBoxRegion(Location, Size);
	MyRegion.SetColor(0.3f, 0.3f, 1.0f, 0.5f);
	MyRegion.Force = Force;
	Forced.push_back(MyRegion);
}

void CDM_FEA::ClearBCs(void) //clears all fixed and forced regions
{
	Fixed.clear();
	Forced.clear();
}

void CDM_FEA::RemoveDisconnected(void) //removes regions not connected to ground... (and sets fixed list...) ALSO removes lone voxels (no bonds...)
{
	NumFixed = 0;

	int NumVox = pObj->GetNumVox(); //number of voxels

	int* List = NULL; //list of voxels to expand
	List = new int[NumVox];
	int ListEnd = 0; //list iterators
	int ListBegin = 0;

	bool* Visited = NULL; //size of full voxel array, set to true if connected!
	Visited = new bool [pObj->GetStArraySize()];

	//Pupoluate fixed...
	int DOFInd = 0;
	Vec3D point;

	for (int i=0; i<pObj->GetStArraySize(); i++){ //for all posible voxels:
		Visited[i] = false;

		if (pObj->GetVoxel(i) != 0){ //if there's a voxel here...
			pObj->GetXYZ(&point, i);

			//Do fixed constraints
			for (int j = 0; j<(int)Fixed.size(); j++){ //go through each primitive defined as a constraint!
				if (!Visited[i] && Fixed[j].pRegion->IsTouching(&point, (float)(pObj->GetLatticeDim()/2))){ //if this point is within
					NumFixed++;
					List[ListEnd++] = i; //add to our list to check later...
					Visited[i] = true;
				}
			}
			DOFInd++;
		}
		
	}

	//go through the list, adding neighbors to the list until we run out...
	int ThisIndex, ThatIndex;
	int x, y, z;
	bool FixedAlone;

	while (ListBegin != ListEnd){
		ThisIndex = List[ListBegin++];

		pObj->GetXYZNom(&x, &y, &z, ThisIndex);
		
		FixedAlone = true; //flag to see if this is a lone grounded one with no bonds... (true unless find a bond...)

		for (int i=0; i<6; i++){ //6 possible directions to find cube
			switch (i){ //go through each dimension
				case 0: ThatIndex = pObj->GetIndex(x+1, y, z); break; //+X direction
				case 1: ThatIndex = pObj->GetIndex(x-1, y, z); break; //-X direction
				case 2: ThatIndex = pObj->GetIndex(x, y+1, z); break; //+Y direction
				case 3: ThatIndex = pObj->GetIndex(x, y-1, z); break; //-Y direction
				case 4: ThatIndex = pObj->GetIndex(x, y, z+1); break; //+Z direction
				case 5: ThatIndex = pObj->GetIndex(x, y, z-1); break; //-Z direction
			}

			if (ThatIndex != -1 && pObj->GetVoxel(ThatIndex) != 0){ //if its there
				if (Visited[ThatIndex] == false){ //if we want to add it to the list
					Visited[ThatIndex] = true; //we've hit it now!
					List[ListEnd++] = ThatIndex;
				}
				FixedAlone = false; //we found a bond!
			}

			
		}

		if (FixedAlone) Visited[ThisIndex] = false; //flag for removal...
	}


	for (int i=0; i<pObj->GetStArraySize(); i++){ //for all posible voxels:
		if (Visited[i] == false) //if we never got there
			pObj->SetVoxel(i, 0); //erase non-connected material!
	}

	DOF = pObj->GetNumVox()*DOFperBlock; //set this to our new DOF value

	//fill in fixed list... ((would be quicker to do this earlier when we checked everything against fixed regions
	DOFInd = 0;
	for (int i=0; i<pObj->GetStArraySize(); i++){ //for all posible voxels:
		if (pObj->GetVoxel(i) != 0){ //if there's a voxel here...
			FixedList[DOFInd] = false; //assume not fixed
			pObj->GetXYZ(&point, i);

			//Do fixed constraints
			for (int j = 0; j<(int)Fixed.size(); j++){ //go through each primitive defined as a constraint!
				if (Fixed[j].pRegion->IsTouching(&point, (float)(pObj->GetLatticeDim()/2))){ //if this point is within
					FixedList[DOFInd] = true; //set this one as fixed

				}
			}
			DOFInd++;
		}
		
	}

	delete[] List;
	List = NULL;
	delete[] Visited;
	Visited = NULL;

}
