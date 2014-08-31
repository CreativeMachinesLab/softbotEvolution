#include "DigitalMatter.h"

CDigitalMatter::CDigitalMatter(void)
{
	Path = ""; //initialize path to nothing
	ClearPalette();
}

CDigitalMatter::~CDigitalMatter(void)
{
	ClearMatter();
}

CDigitalMatter& CDigitalMatter::operator=(const CDigitalMatter& RefObj)
{
	Lattice = RefObj.Lattice;
	Voxel = RefObj.Voxel; 
	Structure = RefObj.Structure;
	Palette = RefObj.Palette;
	Path = RefObj.Path;

	return *this;
}

void CDigitalMatter::ClearMatter(void) //clears out everything...
{
	Lattice.ClearLattice();
	Voxel.ClearVoxel();
	Structure.ClearStructure();
	ClearPalette();
}

void CDigitalMatter::InitializeMatter(CDMF_Lattice* pLattice, int xV, int yV, int zV)
{
	ClearMatter();
	
	LoadDefaultPalette();
	Lattice = *pLattice;
	Voxel.SetVoxName(BOX);
	Structure.CreateStructure(xV, yV, zV);


}

void CDigitalMatter::InitializeMatter(void) //loads default 
{
	if(!LoadFile("Default.dmf")){ //try loading default file (must be in working directory)... if not default to 10x 1mm cubes

		ClearMatter();
		AddMat("Flexible");
		AddMat("Stiff");
		AddMat("Light");
		Palette[1].SetColor(1, 0, 0);
		Palette[1].SetElasticMod(1.0f);
		Palette[1].SetDensity(10.0f);

		Palette[2].SetColor(0, 0, 1);
		Palette[2].SetElasticMod(100.0f);
		Palette[2].SetDensity(10.0f);

		Palette[3].SetColor(1, 1, 0);
		Palette[3].SetElasticMod(10.0f);
		Palette[3].SetDensity(1.0f);

		Lattice.SetLattice(0.001f);

		Voxel.SetVoxName(BOX); 
		Structure.CreateStructure(10, 10, 10);
	}
}

void CDigitalMatter::InitializeMatter(float iLattice_Dim, int xV, int yV, int zV) //intializes object with some default params, including dynamic arrays
{
	ClearMatter();
	
	LoadDefaultPalette();
	Lattice.SetLattice(iLattice_Dim);
	Voxel.SetVoxName(BOX); ///XXX Temporary!!
	Structure.CreateStructure(xV, yV, zV);
}

void CDigitalMatter::LoadDefaultPalette(void)
{
	if (!LoadFile("Default.dmf", true)){ //try to load the default palette from default dmf object
		AddMat("Default"); //otherwise, add a single, default material
	}
}



void CDigitalMatter::ClearPalette() //clears the material palette, leaving only empty (vacant) material
{
	int VecSize = (int)Palette.size();
	if (VecSize != 0)
		Palette.erase(Palette.begin(), Palette.begin()+VecSize); //erase the materials array

	//add back the default (empty) first material
	CDMF_Material Empty = CDMF_Material(NULL, NULL, NULL, NULL, "Erase");

	Palette.push_back(Empty);
}

void CDigitalMatter::DeleteMat(int Index) //deletes a material (in palette and object) and moves the rest of the material indexes down
{
	//checks
	if (Index >= (int)Palette.size()){
		std::cout << "invalid material to delete\n";
		return;
	}

	if (Index == 0){
		std::cout << "Cannot delete Null material\n";
		return;
	}

	Structure.EraseMaterial(Index, true); //erase and shift down....
	Palette.erase(Palette.begin()+Index);

}

int CDigitalMatter::AddMat(std::string Name, float ElasticMod, float PoissonsRatio, float R, float G, float B, float Alpha) //adds a material to the end of the palette with specified properties
{
	//checks
	if (Name == "" || Name == " "){
		std::cout << "Invalid material Name\n";
		return -1;
	}

	for (int i=0; i<(int)Palette.size(); i++)
	{
		if (Name == Palette[i].GetName()){
			std::cout << "Name already in use\n";
			return -1;
		}
	}

	int NextIndex = (int)Palette.size(); //not plus one!
	if (NextIndex<=256){ //if we have a material slot left...
		CDMF_Material tmp;
		Palette.push_back(tmp);
		Palette[NextIndex].SetName(Name);
		Palette[NextIndex].SetColor(R, G, B, Alpha);
		Palette[NextIndex].SetElasticMod(ElasticMod);
		Palette[NextIndex].SetPoissonsRatio(PoissonsRatio);
		Palette[NextIndex].SetDensity(1.0f);
		return NextIndex;
	}
	else {
		std::cout << "Maximum number of materials reached!\n";
		return -1;
	}
}

void CDigitalMatter::New(void) //new dmf
{
	Close();
}

void CDigitalMatter::Save(void) //saves the file, or prompts if not yet been saved
{
	if (Path == ""){ //if file path is empty string (IE not been saved yet)
		std::cout <<"Enter Name to save DMF as:\n";
		std::cin >> Path;
	}

	if (Path.size() != 0) SaveFile(Path);
	else Path = "";
}

bool CDigitalMatter::Open(void) //Brings up file dialog to open dmf file
{
	Close();

	std::string tmpPath;
	std::cout << "Enter File name to open:\n";
	std::cin >> tmpPath;
	if (Path.size()!= 0){
		Path = tmpPath;
		LoadFile(Path);
		return true;
	}
	return false;

}

void CDigitalMatter::Close(void) //closes (and clears) the dmf object
{
	ClearMatter();
	Path = "";

}

bool CDigitalMatter::OpenPal(void) //Open a palette
{
	std::string TmpPath;
	std::cout << "Enter name of Palette to open:\n";
	std::cin >> TmpPath;
	
	if(TmpPath.size() != 0){
		ClearPalette(); //clear all materials
		LoadFile(TmpPath, true);
		return true;
	}
	return false;
}


void CDigitalMatter::SavePal(void) //save a palette
{
	std::string TmpPath;
	std::cout << "Enter name of file to save Palette to:\n";
	std::cin >> TmpPath;

	if(TmpPath.size() != 0){
		SaveFile(TmpPath);
	}
}

void CDigitalMatter::SaveFile(std::string filename, bool OnlyPal)
{
	std::cout << "jmc: here:\n";
	Path = filename;
	CXML_Rip XML;
	WriteXML(&XML);
	XML.SaveFile(filename);
}

bool CDigitalMatter::LoadFile(std::string filename, bool OnlyPal)
{
	std::string tmpS;
	ClearMatter(); //clears everything!

	CXML_Rip XML;
	if (XML.LoadFile(filename)){
		Path = filename;
		ReadXML(&XML);
	}

	return true;
}

void CDigitalMatter::WriteXML(CXML_Rip* pXML)
{
	pXML->DownLevel("DMF");
	pXML->SetAttribute("Version", "0.93");
		Lattice.WriteXML(pXML);
		Voxel.WriteXML(pXML);
		pXML->DownLevel("Palette");
		for (int i=1; i<(int)Palette.size(); i++){ //Can skip the "empty" material (0)
			pXML->DownLevel("Material");
			pXML->SetAttribute("ID", i);
			Palette[i].WriteXML(pXML);
			pXML->UpLevel();
		}
		pXML->UpLevel();
		Structure.WriteXML(pXML);

	pXML->UpLevel();
}

void CDigitalMatter::ReadXML(CXML_Rip* pXML)
{
	std::string ThisVersion = "0.93"; //version of the reading code... (JON WILL CHANGE THIS WHEN UPDATING SOURCE!)
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
}

void CDigitalMatter::GetWorkSpace(Vec3D* Dim) //calculate actual workspace dimensions
{
	float LatDim = Lattice.GetLatticeDim();
	//calculate the dims
	Dim->x = LatDim*((Structure.GetVXDim()-1)*Lattice.GetXDimAdj()+1 + Lattice.GetMaxOffsetX());
	Dim->y = LatDim*((Structure.GetVYDim()-1)*Lattice.GetYDimAdj()+1 + Lattice.GetMaxOffsetY());
	if (Voxel.GetVoxName() == SPHERE)
		Dim->z = LatDim*((Structure.GetVZDim()-1)*Lattice.GetZDimAdj()+1);
	else
		Dim->z = LatDim*Structure.GetVZDim()*Lattice.GetZDimAdj();

}

Vec3D CDigitalMatter::GetWorkSpace() //calculate actual workspace dimensions (acounting for dif offsets!)
{
	Vec3D ToReturn;
	GetWorkSpace(&ToReturn);
	return ToReturn;
}

bool CDigitalMatter::GetXYZ(Vec3D* Point, int index) //gets the XYZ coordinates of an index in the master array, returns true if a valid location
{
	float Eps = 0.000001f; //hack, offset to make sure its all on the right side of zero
	float TotalXOffP = 0.0f; //running variables to add the offsets to. (in percentage of width
	float TotalYOffP = 0.0f;

	int iz = 0;
	int iy = 0;
	int ix = 0;
	GetXYZNom(&ix, &iy, &iz, index); //get nominal x, y, z indicies
	
	TotalXOffP = Lattice.GetXLiO()*iy + Lattice.GetXLaO()*iz + Eps; //total offset
	TotalYOffP = Lattice.GetYLiO()*ix + Lattice.GetYLaO()*iz + Eps; 
	
	//THESE ARE PSEUDO-HACKED!!! (don't work with arbitrary...)
	if (Lattice.GetXLaO() > Lattice.GetYLaO())
		if (((int)(TotalYOffP/Lattice.GetYDimAdj()))%2 == 1)
			TotalXOffP += 0.5;
	else if (Lattice.GetYLaO() > Lattice.GetXLaO())
		if (((int)(TotalXOffP/Lattice.GetXDimAdj()))%2 == 1)
			TotalYOffP += 0.5;

	TotalXOffP = TotalXOffP - ((int)(TotalXOffP/Lattice.GetXDimAdj()))*Lattice.GetXDimAdj();
	TotalYOffP = TotalYOffP - ((int)(TotalYOffP/Lattice.GetYDimAdj()))*Lattice.GetYDimAdj();

	Point->x = Lattice.GetLatticeDim()*(0.5 + TotalXOffP + ix*Lattice.GetXDimAdj());
	Point->y =Lattice.GetLatticeDim()*(0.5 + TotalYOffP + iy*Lattice.GetYDimAdj());
	if (Voxel.GetVoxName() != SPHERE)
		Point->z = Lattice.GetLatticeDim()*((0.5 + iz)*Lattice.GetZDimAdj());
	else
		Point->z = Lattice.GetLatticeDim()*(0.5 + iz*Lattice.GetZDimAdj());

	return true;
}

bool CDigitalMatter::IsAdjacent(int Index1, int Index2, Vec3D* RelativeLoc) //true if these voxels are touching!
{
	int tmp = (int)Structure[Index2];
	(void) tmp; //to get rid of unused variable warning
	if (Index1 < 0 || Index2 >= Structure.GetArraySize() || Structure[Index2] == 0)
		return false;

	//optomize later! XXX
	Vec3D P1;
	Vec3D P2;
	Vec3D Rel;

	GetXYZ(&P1, Index1);
	GetXYZ(&P2, Index2);

	Rel = P2-P1;
	if (RelativeLoc != NULL) //set the relativeness if wanted
		*RelativeLoc = Rel;

	if (Rel.Length()<1.4*Lattice.GetLatticeDim())
		return true;
	else 
		return false;
}


void CDigitalMatter::Resize(CDMF_Structure* pStructure) //resizes a structure, preserving voxels in correct locations
{
	CDMF_Structure LStruct;
	LStruct.CreateStructure(pStructure->GetVXDim(), pStructure->GetVYDim(), pStructure->GetVZDim());

	int tX=0;
	int tY=0;
	int tZ=0;
	int NewIndex = 0;
	Vec3D UselessPoint;

	//Populate the Temporary new array
	for (int i=0; i<Structure.GetArraySize(); i++){ //go through the existing array
		if (Structure[i] != 0){ //If there is a voxel present
			GetXYZNom(&tX, &tY, &tZ, i); //sets the current XYZ indices 
			if (tX < LStruct.GetVXDim() && tY < LStruct.GetVYDim() && tZ < LStruct.GetVZDim()){ //if its within the new area... (X, Y, Z are totals- based from 1)
				if (GetXYZ(&UselessPoint, i)){
					NewIndex = tX + LStruct.GetVXDim()*tY + LStruct.GetVXDim()*LStruct.GetVYDim()*tZ;
					if (NewIndex <= LStruct.GetArraySize()) {
						LStruct[NewIndex] = Structure[i];
					}
					else {
						std::cout << "Index Out-of-range\n";
					}
				}
			}
		}
	}
	Structure = LStruct;
}

void CDigitalMatter::Transform(Vec3D Trans) //shift the strucutre within the workspace (Caution! Trucates!)
{
	//create a copy of current object...
	char* pTmpVoxArray = NULL;
	pTmpVoxArray = new char[Structure.GetArraySize()];
	for (int i=0; i<Structure.GetArraySize(); i++){
		*(pTmpVoxArray+i) = Structure[i];
	}

	int tX = 0, tY = 0, tZ = 0;
	//overwrite with transformed values:
	for (int i=0; i<Structure.GetArraySize(); i++){
		GetXYZNom(&tX, &tY, &tZ, i);
		int NewIndex = GetIndex(tX-(int)Trans.x, tY-(int)Trans.y, tZ-(int)Trans.z);
		if(NewIndex != -1){ //if it was within bounds
			Structure[i] = *(pTmpVoxArray+NewIndex);
		}
		else {
			Structure[i] = 0;
		}
	}

	delete [] pTmpVoxArray;
	pTmpVoxArray = NULL;
}

int CDigitalMatter::GetNumVox(void)
{
	int NumVox = 0;
	for (int i=0; i<Structure.GetArraySize(); i++){ //for each voxel in the array
		if(Structure[i]!=0){ //if this is stuff we want to replace
			NumVox++;
		}
	}
	return NumVox;
}

int CDigitalMatter::GetNumVox(int MatIndex)
{
	int NumVox = 0;
	for (int i=0; i<Structure.GetArraySize(); i++){ //for each voxel in the array
		if(Structure[i] == MatIndex){ //if this is instantiated
			NumVox++;
		}
	}
	return NumVox;
}

float CDigitalMatter::GetWeight(void) //returns weight based on densities stored in material.
{
	float SumWeight = 0;

	//crude volume (works for cubes...)
	float VoxVol = Lattice.GetLatticeDim()*Lattice.GetLatticeDim()*Lattice.GetLatticeDim();

	for (int i=0; i<Structure.GetArraySize(); i++){ //for each voxel in the array
		if (Structure[i]!= 0)
			SumWeight += Palette[(int)Structure[i]].GetDens()*VoxVol;
	}
	return SumWeight;
}

CDMF_Material* CDigitalMatter::GetLeafMat(int Index) //gets the lowest level of material
{
	return GetLeafMat(Index, &Structure); //need this seperate to recurse properly
}

CDMF_Material* CDigitalMatter::GetLeafMat(int Index, CDMF_Structure* pStructure) //returns the material for a given substrucutre
{
	int MatIndex = abs((int)(*pStructure)[Index]);

	if (MatIndex <= 0 || MatIndex > (int)Palette.size()){
		return NULL;
	}

	switch (Palette[MatIndex].GetMatType()){
		case SINGLE:
			return &Palette[MatIndex];
			break;
		case INTERNAL: 
			if (Palette[MatIndex].HasLocalStructure()){
				int x, y, z;
				GetXYZNom(&x, &y, &z, Index);
				x = (x - Palette[MatIndex].GetSubXOffset()) % Palette[MatIndex].GetSubXSize();
				while (x<0) x += Palette[MatIndex].GetSubXSize();
				y = (y - Palette[MatIndex].GetSubYOffset()) % Palette[MatIndex].GetSubYSize();
				while (y<0) y += Palette[MatIndex].GetSubYSize();
				z = (z - Palette[MatIndex].GetSubZOffset()) % Palette[MatIndex].GetSubZSize();
				while (z<0) z += Palette[MatIndex].GetSubZSize();

				int NewIndex = Palette[MatIndex].pStructure->GetIndex(x, y, z);
				return GetLeafMat(NewIndex, Palette[MatIndex].pStructure);
			}
			else
				return &Palette[MatIndex]; //default to default material values...

			break;

		case EXTERNAL:
			if (Palette[MatIndex].pDMF){
				int x, y, z;
				GetXYZNom(&x, &y, &z, Index);
				x = (x - Palette[MatIndex].GetSubXOffset()) % Palette[MatIndex].GetSubXSize();
				while (x<0) x += Palette[MatIndex].GetSubXSize();
				y = (y - Palette[MatIndex].GetSubYOffset()) % Palette[MatIndex].GetSubYSize();
				while (y<0) y += Palette[MatIndex].GetSubYSize();
				z = (z - Palette[MatIndex].GetSubZOffset()) % Palette[MatIndex].GetSubZSize();
				while (z<0) z += Palette[MatIndex].GetSubZSize();

				return Palette[MatIndex].pDMF->GetLeafMat(Palette[MatIndex].pDMF->Structure.GetIndex(x, y, z));
			}
			else
				return &Palette[MatIndex]; //defualt to default material values...
			break;
	}
	return NULL;
}


bool CDigitalMatter::SetVoxel(int x, int y, int z, int MatIndex) //sets the material index at a lattice location
{
	if (MatIndex >=0 && MatIndex < (int)Palette.size()){
		int Index = GetIndex(x, y, z);
		if (Index != -1){ //if it was a valid index!
			SetVoxel(Index, MatIndex);
			return true; //successful
		}
	}
	return false; //error out
}


int CDigitalMatter::GetVoxel(int x, int y, int z) //returns the material index (at a lattice location)
{
	int index = GetIndex(x, y, z);

	if (index == -1) return -1;
	else return GetVoxel(index);
}



////////////////////////////LATTICE///////////////////////////////////////
CDMF_Lattice::CDMF_Lattice(void)
{
	ClearLattice();
}

CDMF_Lattice::~CDMF_Lattice(void)
{
}

CDMF_Lattice& CDMF_Lattice::operator=(const CDMF_Lattice& RefLat)
{
	Lattice_Dim = RefLat.Lattice_Dim;
	X_Dim_Adj = RefLat.X_Dim_Adj;
	Y_Dim_Adj = RefLat.Y_Dim_Adj;
	Z_Dim_Adj = RefLat.Z_Dim_Adj;
	X_Line_Offset = RefLat.X_Line_Offset;
	Y_Line_Offset = RefLat.Y_Line_Offset;
	X_Layer_Offset = RefLat.X_Layer_Offset;
	Y_Layer_Offset = RefLat.Y_Layer_Offset;
	
	return *this;
}

void CDMF_Lattice::ClearLattice()
{
	Lattice_Dim = 0.0f;
	X_Dim_Adj = 1.0f;
	Y_Dim_Adj = 1.0f;
	Z_Dim_Adj = 1.0f;
	X_Line_Offset = 0.0f;
	Y_Line_Offset = 0.0f;
	X_Layer_Offset = 0.0f;
	Y_Layer_Offset = 0.0f;
}

void CDMF_Lattice::ReadXML(CXML_Rip* pXML)
{
	if (!pXML->FindLoadElement("Lattice_Dim", &Lattice_Dim)) Lattice_Dim = 0.001f;
	if (!pXML->FindLoadElement("X_Dim_Adj", &X_Dim_Adj)) X_Dim_Adj = 1.0f;
	if (!pXML->FindLoadElement("Y_Dim_Adj", &Y_Dim_Adj)) Y_Dim_Adj = 1.0f;
	if (!pXML->FindLoadElement("Z_Dim_Adj", &Z_Dim_Adj)) Z_Dim_Adj = 1.0f;
	if (!pXML->FindLoadElement("X_Line_Offset", &X_Line_Offset)) X_Line_Offset = 0.0f;
	if (!pXML->FindLoadElement("Y_Line_Offset", &Y_Line_Offset)) Y_Line_Offset = 0.0f;
	if (!pXML->FindLoadElement("X_Layer_Offset", &X_Layer_Offset)) X_Layer_Offset = 0.0f;
	if (!pXML->FindLoadElement("Y_Layer_Offset", &Y_Layer_Offset)) Y_Layer_Offset = 0.0f;
}

void CDMF_Lattice::WriteXML(CXML_Rip* pXML)
{
	pXML->DownLevel("Lattice");
		pXML->Element("Lattice_Dim", Lattice_Dim);
		pXML->Element("X_Dim_Adj", X_Dim_Adj);
		pXML->Element("Y_Dim_Adj", Y_Dim_Adj);
		pXML->Element("Z_Dim_Adj", Z_Dim_Adj);
		pXML->Element("X_Line_Offset", X_Line_Offset);
		pXML->Element("Y_Line_Offset", Y_Line_Offset);
		pXML->Element("X_Layer_Offset", X_Layer_Offset);
		pXML->Element("Y_Layer_Offset", Y_Layer_Offset);
	pXML->UpLevel();
}

float CDMF_Lattice::GetMaxOffsetX(void)
{
	float XMaxOffset = 0;
	if (X_Line_Offset > XMaxOffset)
		XMaxOffset = X_Line_Offset;
	if (X_Layer_Offset > XMaxOffset)
		XMaxOffset = X_Layer_Offset;
	if (2*X_Layer_Offset > XMaxOffset && 2*X_Layer_Offset < X_Dim_Adj)
		XMaxOffset = 2*X_Layer_Offset;

	return XMaxOffset;
}

float CDMF_Lattice::GetMaxOffsetY(void)
{
	float YMaxOffset = 0;
	if (Y_Line_Offset > YMaxOffset)
		YMaxOffset = Y_Line_Offset;
	if (Y_Layer_Offset > YMaxOffset)
		YMaxOffset = Y_Layer_Offset;
	if (2*Y_Layer_Offset > YMaxOffset && 2*Y_Layer_Offset < Y_Dim_Adj)
		YMaxOffset = 2*Y_Layer_Offset;

	return YMaxOffset;
}





//////////////////////////MATERIAL/////////////////////////////

CDMF_Material::CDMF_Material(void)
{
	pStructure = NULL;
	pDMF = NULL;
	ClearMaterial();
}

CDMF_Material::CDMF_Material(float r, float g, float b, float a, std::string newName) //allow us to create easily
{
	pStructure = NULL;
	pDMF = NULL;
	ClearMaterial();

	MatType = SINGLE; //inferred from list of params!

	Name = newName;
	SetColor(r, g, b, a);
}
	
CDMF_Material::CDMF_Material(const CDMF_Material& RefMat) //copy constructor
{
	Name = RefMat.Name;
	MatType = RefMat.MatType;

	Red = RefMat.Red;
	Green = RefMat.Green;
	Blue = RefMat.Blue;
	Alpha = RefMat.Alpha;

	Elastic_Mod = RefMat.Elastic_Mod;
	Density = RefMat.Density;
	Poissons_Ratio = RefMat.Poissons_Ratio;
	CTE = RefMat.CTE;

	X_Offset = RefMat.X_Offset;
	Y_Offset = RefMat.Y_Offset;
	Z_Offset = RefMat.Z_Offset;

	if (RefMat.pStructure != NULL){
		pStructure = new CDMF_Structure;
		*pStructure = *RefMat.pStructure;
	}
	else
		pStructure = NULL;

	File = RefMat.File;

	if (RefMat.pDMF != NULL){ //delete the old if it exist
		pDMF = new CDigitalMatter;
		*pDMF = *RefMat.pDMF;
	}
	else
		pDMF = NULL;

}


CDMF_Material::~CDMF_Material(void)
{
	if (pStructure != NULL){
		delete pStructure;
		pStructure = NULL;
	}
	if (pDMF != NULL){
		delete pDMF;
		pDMF = NULL;
	}
}

CDMF_Material& CDMF_Material::operator=(const CDMF_Material& RefMat)
{

	Name = RefMat.Name;
	MatType = RefMat.MatType;

	Red = RefMat.Red;
	Green = RefMat.Green;
	Blue = RefMat.Blue;
	Alpha = RefMat.Alpha;

	Elastic_Mod = RefMat.Elastic_Mod;
	Density = RefMat.Density;
	Poissons_Ratio = RefMat.Poissons_Ratio;
	CTE = RefMat.CTE;

	X_Offset = RefMat.X_Offset;
	Y_Offset = RefMat.Y_Offset;
	Z_Offset = RefMat.Z_Offset;

	if (pStructure != NULL){ //delete the old if it exist
		delete pStructure;
		pStructure = NULL;
	}

	if (RefMat.pStructure){ //null materi
		pStructure = new CDMF_Structure();
		*pStructure = *RefMat.pStructure;
	}

	File = RefMat.File;

	if (pDMF != NULL){ //delete the old if it exist
		delete pDMF;
		pDMF = NULL;
	}

	if (RefMat.pDMF){ //null 
		pDMF = new CDigitalMatter;
		*pDMF = *RefMat.pDMF;
	}

	return *this;

}

void CDMF_Material::ClearMaterial(void)
{
	Name = "";
	MatType = SINGLE;

	Red = 0.5f;
	Green = 0.5f;
	Blue = 0.5f;
	Alpha = 1.0f;

	Elastic_Mod = 0.0f;
	Density = 0.0f;
	Poissons_Ratio = 0.0f;
	CTE = 0.0f;

	if (pStructure != NULL){
		delete [] pStructure;
		pStructure = NULL;
	}

	X_Offset = 0;
	Y_Offset = 0;
	Z_Offset = 0;

	//for external material definition
	File = "";

	if (pDMF != NULL){
		delete [] pDMF;
		pDMF = NULL;
	}

}

void CDMF_Material::SetColor(float r, float g, float b, float a)
{
	Red = r;
	Green = g;
	Blue = b;
	Alpha = a;
}

void CDMF_Material::SetColor(float r, float g, float b)
{
	SetColor(r, g, b, 1.0f);
}

void CDMF_Material::WriteXML(CXML_Rip* pXML)
{
//	pXML->DownLevel("Material");
	pXML->Element("MatType", MatType);

	switch (MatType){
		case SINGLE:
			pXML->Element("Name", Name);
			pXML->DownLevel("Display");
				pXML->Element("Red", Red);
				pXML->Element("Green", Green);
				pXML->Element("Blue", Blue);
				pXML->Element("Alpha", Alpha);
			pXML->UpLevel();
			pXML->DownLevel("Mechanical");
				pXML->Element("Elastic_Mod", Elastic_Mod);
				pXML->Element("Density", Density);
				pXML->Element("Poissons_Ratio", Poissons_Ratio);
				pXML->Element("CTE", CTE);
			pXML->UpLevel();
			break;
		case INTERNAL:
			pXML->Element("Name", Name);
			pStructure->WriteXML(pXML);
			pXML->Element("X_Offset", X_Offset);
			pXML->Element("Y_Offset", Y_Offset);
			pXML->Element("Z_Offset", Z_Offset);
			break;
		case EXTERNAL:
			pXML->Element("File", File);
			break;
	}
//	pXML->UpLevel();
}

void CDMF_Material::ReadXML(CXML_Rip* pXML, std::string Version)
{
	if (!pXML->FindLoadElement("MatType", &MatType)){ //if there's not a MatType tag, we need some detective work...
		if (pXML->FindElement("Structure")) {pXML->UpLevel(); MatType = INTERNAL;}
		else if (pXML->FindElement("DMF")) {pXML->UpLevel(); MatType = EXTERNAL;}
		else MatType = SINGLE;
	}
	if (!pXML->FindLoadElement("Name", &Name)) Name = "Default";

	switch (MatType){
		case SINGLE:
			if (pXML->FindElement("Display")){
				if (!pXML->FindLoadElement("Red", &Red)) Red = 0.5;
				if (!pXML->FindLoadElement("Green", &Green)) Green = 0.5;
				if (!pXML->FindLoadElement("Blue", &Blue)) Blue = 0.5;
				if (!pXML->FindLoadElement("Alpha", &Alpha)) Alpha = 1.0;
				pXML->UpLevel();
			}
			if (pXML->FindElement("Mechanical")){
				if (!pXML->FindLoadElement("Elastic_Mod", &Elastic_Mod)) Elastic_Mod = 0;
				if (!pXML->FindLoadElement("Density", &Density)) Density = 0;
				if (!pXML->FindLoadElement("Poissons_Ratio", &Poissons_Ratio)) Poissons_Ratio = 0;
				if (!pXML->FindLoadElement("CTE", &CTE)) CTE = 0;
				pXML->UpLevel();
			}
			break;
		case INTERNAL:
			if (!pXML->FindLoadElement("Name", &Name)) Name = "";
			if (!pXML->FindLoadElement("X_Offset", &X_Offset)) X_Offset = 0;
			if (!pXML->FindLoadElement("Y_Offset", &Y_Offset)) Y_Offset = 0;
			if (!pXML->FindLoadElement("Z_Offset", &Z_Offset)) Z_Offset = 0;

			if (pXML->FindElement("Structure")){
				if (pStructure == NULL)
					pStructure = new CDMF_Structure;
				pStructure->ReadXML(pXML, Version);
				pXML->UpLevel();
			}
			break;
		case EXTERNAL:
			if (!pXML->FindLoadElement("File", &File)) File = "";
			if (pDMF == NULL) pDMF = new CDigitalMatter;
			if (File != "") pDMF->LoadFile(File);
			break;
	}
}

////////////////////////////////STRUCTURE////////////////////////////////

CDMF_Structure::CDMF_Structure()
{
	pData = NULL; //initialize to void
	ClearStructure(); //clear all model parameters
}

CDMF_Structure::~CDMF_Structure()
{
	if (pData != NULL){
		delete [] pData;
		pData = NULL;
	}
}

CDMF_Structure::CDMF_Structure(const CDMF_Structure& RefStruct) //copy constructor
{
	Compression = RefStruct.Compression;
	X_Voxels = RefStruct.X_Voxels;
	Y_Voxels = RefStruct.Y_Voxels;
	Z_Voxels = RefStruct.Z_Voxels;

	m_SizeOfArray = RefStruct.m_SizeOfArray;

	if (pData != NULL){ //clear current array
		delete [] pData;
		pData = NULL;
	}

	if (RefStruct.pData != NULL){ 
		pData = new char[RefStruct.m_SizeOfArray];
		for (int i=0; i<RefStruct.m_SizeOfArray; i++)
			*(pData+i) = *(RefStruct.pData+i);
	}
}

CDMF_Structure& CDMF_Structure::operator=(const CDMF_Structure& RefStruct)
{

	Compression = RefStruct.Compression;
	X_Voxels = RefStruct.X_Voxels;
	Y_Voxels = RefStruct.Y_Voxels;
	Z_Voxels = RefStruct.Z_Voxels;

	m_SizeOfArray = RefStruct.m_SizeOfArray;

	if (pData != NULL) //delete the old if it exist
		delete [] pData;
	pData = new char[RefStruct.m_SizeOfArray];
	for (int i=0; i<RefStruct.m_SizeOfArray; i++)
		*(pData+i) = *(RefStruct.pData+i);

	return *this;
}

void CDMF_Structure::WriteXML(CXML_Rip* pXML)
{
	pXML->DownLevel("Structure");
	pXML->SetAttribute("Compression", "ASCII_READABLE");

	pXML->Element("X_Voxels", X_Voxels);
	pXML->Element("Y_Voxels", Y_Voxels);
	pXML->Element("Z_Voxels", Z_Voxels);

	pXML->DownLevel("Data");

	for (int i=0; i<Z_Voxels; i++){
		std::string Data;
		for (int j=0; j<X_Voxels*Y_Voxels; j++){Data += pData[i*X_Voxels*Y_Voxels + j]+48;} //store in ascii numbers
		int size = Data.size();
		(void) size; //to get rid of unused variable warning
		pXML->Element("Layer", Data, true);
	}
	pXML->UpLevel();
	pXML->UpLevel();
}

void CDMF_Structure::ReadXML(CXML_Rip* pXML, std::string Version)
{
	std::string Compression;
	pXML->GetAttribute("Compression", &Compression);

	if (!pXML->FindLoadElement("X_Voxels", &X_Voxels)) X_Voxels = 1;
	if (!pXML->FindLoadElement("Y_Voxels", &Y_Voxels)) Y_Voxels = 1;
	if (!pXML->FindLoadElement("Z_Voxels", &Z_Voxels)) Z_Voxels = 1;

	CreateStructure(X_Voxels, Y_Voxels, Z_Voxels); //create the space for the structure

	if (Version == "" || Version == "0.93"){ //newest version
		pXML->FindElement("Data");
		for (int i=0; i<Z_Voxels; i++){
			std::string DataInString;
			pXML->FindLoadElement("Layer", &DataInString, true);
			for(int k=0; k<X_Voxels*Y_Voxels; k++){
				pData[X_Voxels*Y_Voxels*i + k] = *(DataInString.data() + k);
			}
		}
	}
	else if (Version == "0.92") { //legacy version
		std::string Data;
		pXML->FindLoadElement("Data", &Data);

		//QByteArray RawData = Data.toAscii();


		//char *pDataIn = RawData.data(); //get pointer to characters

		//int IteratorIn = 0; //iterator for input file
		//int Iterator = 0; //iterator for output file

		//for (int k=0; k<X_Voxels*Y_Voxels*Z_Voxels + 2*Z_Voxels; k++){ //specific to old system where each ascii line was a layer...
		//	if (pDataIn[IteratorIn] != 0x0A && pDataIn[IteratorIn] != 0x0D){ //ignore carraige returns and line feeds
		//		char tmp = pDataIn[IteratorIn];
		//		pData[Iterator++] = pDataIn[IteratorIn];
		//	}
		//	IteratorIn++;
		//}
	}
}

#ifdef MFC
void CDMF_Structure::WriteStructure(ofstream* pFile, std::string Padding)
{
	*pFile << Padding << "<Structure Compression=\"" << Compression << "\">\n";

	*pFile << Padding << "  <X_Voxels>" << X_Voxels << "</X_Voxels>\n";
	*pFile << Padding << "  <Y_Voxels>" << Y_Voxels << "</Y_Voxels>\n";
	*pFile << Padding << "  <Z_Voxels>" << Z_Voxels << "</Z_Voxels>\n";

	*pFile << Padding << "  <Data>\n";

	*pFile << "<![CDATA[\n";
	for (int k=0; k<Z_Voxels; k++){ //store each layers' data on a seperate line
		for (int ij=0; ij<X_Voxels*Y_Voxels; ij++){
			BYTE PutValue = (BYTE)abs(pData[k*X_Voxels*Y_Voxels + ij]);
			(*pFile).put(PutValue);
		}
		*pFile << "\n";
	}
	*pFile << "]]>\n";

	*pFile << Padding << "  </Data>\n";
	*pFile << Padding << "</Structure>\n";
}

void CDMF_Structure::ReadStructure(CStdioFile* pFile)
{
	std::string line = "";
	int count = 0;

	while(line.Left(12).CompareNoCase("</Structure>") != 0){ //Go until the </Structure> tag
		pFile->ReadString(line);
		line.Trim(' ');
		if (count++ > 1000){ //watch for infinite loops...
			AfxMessageBox("Could not find closing Lattice tag");
			return;
		}

		if(line.Left(10).CompareNoCase("<X_Voxels>") == 0 && sscanf_s(line, "<X_Voxels>%d</X_Voxels>", &X_Voxels)!=1) 
			AfxMessageBox("Error Reading Structure Parameters: X_Voxels");
		if(line.Left(10).CompareNoCase("<Y_Voxels>") == 0 && sscanf_s(line, "<Y_Voxels>%d</Y_Voxels>", &Y_Voxels)!=1) 
			AfxMessageBox("Error Reading Structure Parameters: Y_Voxels");
		if(line.Left(10).CompareNoCase("<Z_Voxels>") == 0 && sscanf_s(line, "<Z_Voxels>%d</Z_Voxels>", &Z_Voxels)!=1) 
			AfxMessageBox("Error Reading Structure Parameters: Z_Voxels");

		if(line.Left(6).CompareNoCase("<Data>") == 0){
			pFile->ReadString(line);
			line.Trim(' ');
			if (line.Left(10).CompareNoCase("<![CDATA[") != 0){
				AfxMessageBox("did not find CDATA tag in expected place!");
				return;
			}

			if (X_Voxels == 0 || Y_Voxels == 0 || Z_Voxels == 0){ //check to make sure we have decent parameters!
				AfxMessageBox("Invalid Structure dimensions when data loading. (reorder file?)");
				return;
			}
			CreateStructure(X_Voxels, Y_Voxels, Z_Voxels);

			for (int k=0; k<Z_Voxels; k++){
				int numTrans = pFile->Read(pData+k*X_Voxels*Y_Voxels, X_Voxels*Y_Voxels);
				pFile->Seek(2, CFile::current); //move past the endline character!
			}

			pFile->ReadString(line);
			line.Trim(' ');
			if (line.Left(10).CompareNoCase("]]>") != 0){
				AfxMessageBox("did not find ending CDATA tag in expected place!");
				//return; //(can probably recover from this...)
			}
		}
	}
}
#endif

void CDMF_Structure::ClearStructure() //completey erases, frees, and destroys the voxel array
{
	m_SizeOfArray = 0;
	if (pData != NULL){
		delete [] pData;
		pData = NULL;
	}

	Compression = "";

	X_Voxels = 0;
	Y_Voxels = 0;
	Z_Voxels = 0;
}

void CDMF_Structure::CreateStructure(int xV, int yV, int zV) //creates empty structure with these dimensions
{
	m_SizeOfArray = xV*yV*zV;

	X_Voxels = xV;
	Y_Voxels = yV;
	Z_Voxels = zV;

	if (pData != NULL) //delete the old if it exist
		delete [] pData;
	pData = new char[m_SizeOfArray];

	ResetStructure(); //sets to zeros!
}

void CDMF_Structure::ResetStructure() //erases all voxel imformation within voxel array
{
	for (int i=0; i<m_SizeOfArray; i++)
		pData[i] = 0;
}

void CDMF_Structure::EraseMaterial(int Matindex, bool ShiftDown)
{
	//Delete all voxels in the matrix made of this material and (optionally) shift down the rest...
	for (int i=0; i<m_SizeOfArray; i++){
		if (*(pData+i) == Matindex)
			*(pData+i) = 0;

		if (ShiftDown && (int)*(pData+i) > Matindex)
			*(pData+i) = abs(*(pData+i))-1;

	}
}


int CDMF_Structure::GetIndex(int x, int y, int z) { //returns the index of the array from xyz indices
	if (x<0 || x >= X_Voxels || y<0 || y >= Y_Voxels || z<0 || z >= Z_Voxels) { //if this XYZ is out of the area
		return -1;
	}
	else {
		return x + X_Voxels*y + X_Voxels*Y_Voxels*z;
	}
}

bool CDMF_Structure::GetXYZNom(int* x, int* y, int* z, int index) //gets the physical position of the voxels from index
{
	if (index<0 || index > X_Voxels*Y_Voxels*Z_Voxels){
		*x = -1;
		*y = -1;
		*z = -1;
		return false;
	}
	else {
		*z = (int)((float)index / (X_Voxels*Y_Voxels)); //calculate the indices in x, y, z directions
		*y = (int)(((float)index - *z*X_Voxels*Y_Voxels)/X_Voxels);
		*x = index - *z*X_Voxels*Y_Voxels - *y*X_Voxels;
		return true;
	}
}



////////////////////////////////VOXEL////////////////////////////


CDMF_Voxel::CDMF_Voxel(void)
{
	ClearVoxel();
}

CDMF_Voxel::~CDMF_Voxel(void)
{
}

CDMF_Voxel& CDMF_Voxel::operator=(const CDMF_Voxel& RefVox)
{

	Vox_Name = RefVox.Vox_Name;
	File = RefVox.File;
	X_Squeeze = RefVox.X_Squeeze;
	Y_Squeeze = RefVox.Y_Squeeze;
	Z_Squeeze = RefVox.Z_Squeeze;

	return *this;

}

void CDMF_Voxel::ReadXML(CXML_Rip* pXML)
{
	std::string Name;
	if (pXML->FindLoadElement("File", &File)) {Vox_Name = VOXFILE; }
	else if (pXML->FindLoadElement("Vox_Name", &Name)){
		if (Name == "SPHERE") Vox_Name = SPHERE;
		else if (Name == "BOX") Vox_Name = BOX;
		else if (Name == "CYLINDER") Vox_Name = CYLINDER;
		else Vox_Name = BOX; //if not these three...
		File = "";
	}
	else { //default
		Vox_Name = BOX; //default
		File = "";
	}

	if (!pXML->FindLoadElement("X_Squeeze", &X_Squeeze)) X_Squeeze = 1.0;
	if (!pXML->FindLoadElement("Y_Squeeze", &Y_Squeeze)) Y_Squeeze = 1.0;
	if (!pXML->FindLoadElement("Z_Squeeze", &Z_Squeeze)) Z_Squeeze = 1.0;
}

void CDMF_Voxel::WriteXML(CXML_Rip* pXML)
{
	pXML->DownLevel("Voxel");
		switch (Vox_Name){
			case VOXFILE: pXML->Element("File", File); break;
			case SPHERE: pXML->Element("Vox_Name", "SPHERE"); break;
			case BOX: pXML->Element("Vox_Name", "BOX"); break;
			case CYLINDER: pXML->Element("Vox_Name", "CYLINDER"); break;
		}
		pXML->Element("X_Squeeze", X_Squeeze);
		pXML->Element("Y_Squeeze", Y_Squeeze);
		pXML->Element("Z_Squeeze", Z_Squeeze);
	pXML->UpLevel();
}

#ifdef MFC
void CDMF_Voxel::WriteVoxel(ofstream* pFile)
{
	*pFile << "<Voxel>\n";

	switch (Vox_Name){
		case VOXFILE:
			*pFile << "  <File>" << File << "</File>\n";
			break;
		case SPHERE:
			*pFile << "  <Vox_Name>SPHERE</Vox_Name>\n";
			break;
		case BOX:
			*pFile << "  <Vox_Name>BOX</Vox_Name>\n";
			break;
		case CYLINDER:
			*pFile << "  <Vox_Name>CYLINDER</Vox_Name>\n";
			break;
	}
		*pFile << "  <X_Squeeze>" << X_Squeeze << "</X_Squeeze>\n";
		*pFile << "  <Y_Squeeze>" << Y_Squeeze << "</Y_Squeeze>\n";
		*pFile << "  <Z_Squeeze>" << Z_Squeeze << "</Z_Squeeze>\n";

	*pFile << "</Voxel>\n";

}

void CDMF_Voxel::ReadVoxel(CStdioFile* pFile)
{
	std::string line = "";
	int count = 0;

	while(line.Left(8).CompareNoCase("</Voxel>") != 0){ //Go until the </Lattice> tag
		(*pFile).ReadString(line);
		line.Trim(' ');
		if (count++ > 1000){ //watch for infinite loops...
			AfxMessageBox("Could not find closing Voxel tag");
			return;
		}

		if(line.Left(6).CompareNoCase("<File>") == 0){ //if we find this tag, its an external file!
			File = line.Mid(6, line.GetLength()-13);
			Vox_Name = VOXFILE;
			//LOAD THE FILE HERE!!!
		}

		if(line.Left(10).CompareNoCase("<Vox_Name>") == 0){
			std::string Name = line.Mid(10, line.GetLength()-21);
			if (Name.CompareNoCase("SPHERE") == 0) Vox_Name = SPHERE;
			else if (Name.CompareNoCase("BOX") == 0) Vox_Name = BOX;
			else if (Name.CompareNoCase("CYLINDER") == 0) Vox_Name = CYLINDER;
		}

		if(line.Left(11).CompareNoCase("<X_Squeeze>") == 0 && sscanf_s(line, "<X_Squeeze>%f</X_Squeeze>", &X_Squeeze)!=1) 
			AfxMessageBox("Error Reading Material Parameters: X_Squeeze");
		if(line.Left(11).CompareNoCase("<Y_Squeeze>") == 0 && sscanf_s(line, "<Y_Squeeze>%f</Y_Squeeze>", &Y_Squeeze)!=1) 
			AfxMessageBox("Error Reading Material Parameters: Y_Squeeze");
		if(line.Left(11).CompareNoCase("<Z_Squeeze>") == 0 && sscanf_s(line, "<Z_Squeeze>%f</Z_Squeeze>", &Z_Squeeze)!=1) 
			AfxMessageBox("Error Reading Material Parameters: Z_Squeeze");
	}
}
#endif

void CDMF_Voxel::ClearVoxel(void)
{
	Vox_Name = SPHERE; //default to sphere!
	File = "";
	X_Squeeze = 1.0f;
	Y_Squeeze = 1.0f;
	Z_Squeeze = 1.0f;
}
