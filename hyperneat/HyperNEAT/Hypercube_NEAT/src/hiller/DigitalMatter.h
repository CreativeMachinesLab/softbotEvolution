#ifndef CDIGITALMATTER_H
#define CDIGITALMATTER_H
#include <vector>
#include "XML_Rip.h" 
#include <math.h>

/*Written by: Jonathan Hiller (jdh74@cornell.edu) */

class CDMF_Lattice;
class CDMF_Voxel;
class CDMF_Structure;
class CDMF_Material;

class Vec3D { //a basic 3D vector container
public:
	Vec3D() {};
	Vec3D(double XIn, double YIn, double ZIn) {x = XIn; y=YIn; z=ZIn;};
	Vec3D(const Vec3D& vIn) {*this = vIn;} //copy constructor
	inline Vec3D& operator=(const Vec3D& vIn) {x=vIn.x; y=vIn.y; z=vIn.z; return *this;} //overload "=" 
	inline Vec3D operator+(Vec3D& s) {return Vec3D(x+s.x, y+s.y, z+s.z);};
	inline Vec3D operator-(Vec3D& s) {return Vec3D(x-s.x, y-s.y, z-s.z);};
	inline Vec3D operator*(double f) {return Vec3D(x*f, y*f, z*f);};
	inline Vec3D operator/(double f) {return Vec3D(x/f, y/f, z/f);};
	inline Vec3D& operator+=(Vec3D& s) {x += s.x; y += s.y; z += s.z; return *this; };
	inline double Length() {return sqrt(x*x+y*y+z*z);};
	inline double Normalize() {double l = sqrt(x*x+y*y+z*z); if (l > 0) {x /= l;y /= l;z /= l;} return l;};
	inline double Dot(Vec3D v) {return (x * v.x + y * v.y + z * v.z);};
	inline Vec3D Cross(Vec3D v) {return Vec3D(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);};
	inline Vec3D Normalized() {	double l = sqrt(x*x+y*y+z*z); return (l>0?(*this)/l:(*this));};

	double x, y, z;
};

class CDMF_Lattice //container for information about the lattice of possible voxel locations
{
public:
	CDMF_Lattice(void);
	~CDMF_Lattice(void);
	CDMF_Lattice(const CDMF_Lattice& Lattice) {*this = Lattice;} //copy constructor
	CDMF_Lattice& operator=(const CDMF_Lattice& RefLat); //overload "=" 

	//I/O functions for load/save
	void ReadXML(CXML_Rip* pXML);
	void WriteXML(CXML_Rip* pXML);

	//functions to get info about lattice
	float GetLatticeDim(void) {return Lattice_Dim;};
	float GetXDimAdj(void) {return X_Dim_Adj;};
	float GetYDimAdj(void) {return Y_Dim_Adj;};
	float GetZDimAdj(void) {return Z_Dim_Adj;};
	float GetXLiO(void) {return X_Line_Offset;};
	float GetYLiO(void) {return Y_Line_Offset;};
	float GetXLaO(void) {return X_Layer_Offset;};
	float GetYLaO(void) {return Y_Layer_Offset;};

	float GetMaxOffsetX(void); //find the maximum offsets (between 0 and 1) to get correct bounding box.
	float GetMaxOffsetY(void);

	//Funtions to modify the lattice
	void SetLattice(float LatDimIn, float XDAdjIn = 1.0f, float YDAdjIn = 1.0f, float ZDAdjIn = 1.0f, float XLiOIn = 0.0f, float YLiOIn = 0.0f, float XLaOIn = 0.0f, float YLaOIn = 0.0f){Lattice_Dim = LatDimIn; X_Dim_Adj = XDAdjIn; Y_Dim_Adj = YDAdjIn; Z_Dim_Adj = ZDAdjIn; X_Line_Offset = XLiOIn; Y_Line_Offset = YLiOIn; X_Layer_Offset = XLaOIn; Y_Layer_Offset = YLaOIn; };
	void ClearLattice(void);

protected:
	//variables from file:
	float Lattice_Dim;
	float X_Dim_Adj;
	float Y_Dim_Adj;
	float Z_Dim_Adj;
	float X_Line_Offset;
	float Y_Line_Offset;
	float X_Layer_Offset;
	float Y_Layer_Offset;
};

#define VOXFILE -1
#define SPHERE 0
#define BOX 1
#define CYLINDER 2

class CDMF_Voxel
{
public:
	CDMF_Voxel(void);
	~CDMF_Voxel(void);
	CDMF_Voxel(const CDMF_Voxel& Voxel) {*this = Voxel;} //copy constructor
	CDMF_Voxel& operator=(const CDMF_Voxel& RefVoxel); //overload "=" 

	//I/O function for save/loading
	void ReadXML(CXML_Rip* pXML);
	void WriteXML(CXML_Rip* pXML);

	//functions to modify voxel
	void SetVoxName(int VoxNameIn){Vox_Name = VoxNameIn;};
	void ClearVoxel(void);
	void DrawVoxel(Vec3D* Center, float Lat_Dim, float Z_Dim_Adj = 1.0f); //draw it (OpenGL)

	//functions to get information about voxel
	int GetVoxName() {return Vox_Name;};

protected:
	//variable from file:
	int Vox_Name;
	std::string File;
	float X_Squeeze;
	float Y_Squeeze;
	float Z_Squeeze;
};

class CDMF_Structure //contains voxel location information in vast 1D array
{
public:
	CDMF_Structure();
	~CDMF_Structure();
	CDMF_Structure(const CDMF_Structure& RefStruct); //copy constructor
	CDMF_Structure& operator=(const CDMF_Structure& RefStruct); //overload "=" 
	char& operator [](int i) { return pData[i]; }

	//I/O function for save/loading
	void WriteXML(CXML_Rip* pXML);
	void ReadXML(CXML_Rip* pXML, std::string Version = ""); //version -1 means latest.

	//Get information about the structure:
	int GetArraySize(void) {return m_SizeOfArray;};
	int GetVXDim(void) {return X_Voxels;}; //get number of voxels in each dimension
	int GetVYDim(void) {return Y_Voxels;};
	int GetVZDim(void) {return Z_Voxels;};
	int GetIndex(int x, int y, int z); //returns the index of the array from xyz indices
	bool GetXYZNom(int* x, int* y, int* z, int index); //gets the physical position of the voxels from index

	//Functions to modify the lattice:
	void CreateStructure(int xV, int yV, int zV); //creates empty structure with these dimensions
	void ClearStructure(); //completly erases, frees, and destroys the voxel array
	void ResetStructure(); //erases all voxel imformation within voxel array

	void EraseMaterial(int Matindex, bool ShiftDown = false); //deletes a material, option to decrement all higher indices

protected:
	//variable from file
	std::string Compression;
	char* pData; //the main voxel array... 

	int X_Voxels;
	int Y_Voxels;
	int Z_Voxels;

	//other variables:
	int m_SizeOfArray; //keep track of the number of elements in the voxel array
};

class CDigitalMatter
{
public:
	//Constructors and operators
	CDigitalMatter(void);
	~CDigitalMatter(void);
	CDigitalMatter(const CDigitalMatter& RefObj) {*this = RefObj;} //copy constructor
	CDigitalMatter& operator=(const CDigitalMatter& RefObj); //overload "=" 
	char& operator [](int i) { return Structure[i]; }

	void GetDim(Vec3D* pVec) {GetWorkSpace(pVec);};

	//high level file I/O functions
	void New(void);
	void Save(void);
	void SaveAs(void) {Path = ""; Save();};
	bool Open(void);
	void Close(void);

protected:
	//Member classes (see DMF description document)
	CDMF_Lattice Lattice; //Lattice info (from DMF)
	CDMF_Voxel Voxel; //Voxel display info (from DMF)
	CDMF_Structure Structure; //voxel structure (from DMF)
	std::vector<CDMF_Material> Palette; //material palette (from DMF)


	//DMF file in/out
	std::string Path; //current file path

public:
	//I/O function for save/loading
	void WriteXML(CXML_Rip* pXML);
	void ReadXML(CXML_Rip* pXML);

	//creating and clearing a dmf object
	void InitializeMatter(void); //intializes object with all defaults, including dynamic arrays
	void InitializeMatter(float iLattice_Dim, int xV, int yV, int zV); //intializes object with some default params, including dynamic arrays
	void InitializeMatter(CDMF_Lattice* pLattice, int xV, int yV, int zV); //intializes object, including dynamic arrays
	
	void ClearMatter(void); //clears everything, including deleting arrays!

	void LoadDefaultPalette(void); //loads a default palette
	void ClearPalette(); //clears (erases) entire palette
	void DeleteMat(int Index); //deletes a material and shifts the rest down
//	int AddMat(std::string Name); //adds a material to the end of the palette
//	int AddMat(std::string Name, float ElasticMod, float PoissonsRatio); //adds a material to the end of the palette with specified properties
	int AddMat(std::string Name, float ElasticMod = 1.0f, float PoissonsRatio = 0.33f, float R=0.5f, float G=0.5f, float B=0.5f, float Alpha=1.0f); //adds a material to the end of the palette with specified properties


	//Basic editing of dmf object
	bool SetVoxel(int x, int y, int z, int MatIndex); //sets the material index
	int GetVoxel(int x, int y, int z); //returns the material index at this location
	void SetVoxel(int Index, int MatIndex) {Structure[Index] = MatIndex;}; //sets the material index at this location based on the index (advanced users)
	int GetVoxel(int Index) {return Structure[Index];}; //returns the material index at this location based on the index (advanced users)

	void Transform(Vec3D Trans); //shift the strucutre within the workspace (Caution! Trucates!)
	void Resize(CDMF_Structure* Structure); //resizes the DMF strucutre preserving data


	//low level file i/o functions
	void SaveFile(std::string filename, bool OnlyPal = false);
	bool LoadFile(std::string filename, bool OnlyPal = false);
	bool OpenPal(void); //open material palette
	void SavePal(void); //save material palette

	//status functions: call to get information about the strucutre or voxels.
	double GetLatticeDim(void) {return Lattice.GetLatticeDim();} //returns the main lattice dimension (voxel size)
	void GetVDim(int* X, int* Y, int* Z) {*X = GetVXDim(); *Y = GetVYDim(); *Z = GetVZDim();} //returns the number of voxels in all dimensions
	int GetVXDim(void) {return Structure.GetVXDim();} //returns the number of voxels in the X direction
	int GetVYDim(void) {return Structure.GetVYDim();} //returns the number of voxels in the Y direction
	int GetVZDim(void) {return Structure.GetVZDim();} //returns the number of voxels in the Z direction
	int GetStArraySize(void) {return Structure.GetArraySize();};

	void GetWorkSpace(Vec3D* Dim); //calculate actual workspace dimensions (acounting for dif offsets!)
	Vec3D GetWorkSpace(); //calculate actual workspace dimensions (acounting for dif offsets!)
	bool GetXYZ(Vec3D* Point, int index); //gets the XYZ coordinates of an index in the master array, returns true if a valid location
	bool GetXYZ(Vec3D* Point, int x, int y, int z) {return GetXYZ(Point, GetIndex(x, y, z));}
	Vec3D GetXYZ(int index) {Vec3D toReturn; if (GetXYZ(&toReturn, index)) return toReturn; else return Vec3D (-1, -1, -1);}
	Vec3D GetXYZ(int x, int y, int z) {return GetXYZ(GetIndex(x, y, z));}
	int GetIndex(int x, int y, int z) {return Structure.GetIndex(x, y, z);} //get index from xyz indices
	bool GetXYZNom(int* x, int* y, int* z, int index) {return Structure.GetXYZNom(x, y, z, index);} //get xyz indices from index
	bool IsAdjacent(int Index1, int Index2, Vec3D* RelativeLoc = NULL); //true if these voxels are touching!
	CDMF_Material* GetLeafMat(int Index); //returns the material at this location
	CDMF_Material* GetLeafMat(int Index, CDMF_Structure* pStructure); //returns the material for a given substrucutre

	//Statistics of structure
	int GetNumVox(void);
	int GetNumVox(int MatIndex);
	float GetWeight(void); //based on densities
};

#define SINGLE 0
#define INTERNAL 1
#define EXTERNAL 2

class CDMF_Material
{
public:
	CDMF_Material(void);
	~CDMF_Material(void);
	CDMF_Material(const CDMF_Material& Mat); //copy constructor
	CDMF_Material& operator=(const CDMF_Material& RefMat); //overload "=" 
	CDMF_Material(float r, float g, float b, float a, std::string Name); //allow us to create easily

	//I/O function for save/loading
	void WriteXML(CXML_Rip* pXML);
	void ReadXML(CXML_Rip* pXML, std::string Version = "");

	//Functions to modify material
	void ClearMaterial(void);
	void SetColor(float r, float g, float b, float a);
	void SetColor(float r, float g, float b);
	void SetGLColor(void);
	void SetName(std::string NameIn){Name = NameIn;};
	void SetDensity(float DensIn){Density = DensIn;};
	void SetElasticMod(float EMIn){Elastic_Mod = EMIn;};
	void SetPoissonsRatio(float PIn){Poissons_Ratio = PIn;};
	void SetCTE(float CTEIn){CTE = CTEIn;};

	//functions to get info about material
	std::string& GetName(void) {return Name;};
	float GetEMod(void) {return Elastic_Mod;};
	float GetDens(void) {return Density;};
	int GetMatType(void) {return MatType;};
	bool HasLocalStructure(void) {if (pStructure) return true; else return false;};
	int GetSubXOffset(void) {return X_Offset;};
	int GetSubYOffset(void) {return Y_Offset;};
	int GetSubZOffset(void) {return Z_Offset;};
	int GetSubXSize(void){if (MatType == INTERNAL) return pStructure->GetVXDim(); else if (MatType == EXTERNAL) return pDMF->GetVXDim(); else return 1;};
	int GetSubYSize(void){if (MatType == INTERNAL) return pStructure->GetVYDim(); else if (MatType == EXTERNAL) return pDMF->GetVYDim(); else return 1;};
	int GetSubZSize(void){if (MatType == INTERNAL) return pStructure->GetVZDim(); else if (MatType == EXTERNAL) return pDMF->GetVZDim(); else return 1;};

	//ok fine, I left these public to make recursion easier...
	CDMF_Structure* pStructure; //create dynamically to keep things small...
	CDigitalMatter* pDMF; //to hold loaded digital material in for external dmf...

protected:
	std::string Name;
	int MatType; //keep track of which material this is!

	//for low-level material definition
	//<display>
	float Red;
	float Green;
	float Blue;
	float Alpha;
	//<Mechanical>
	float Density;
	float Elastic_Mod;
	float Poissons_Ratio;
	float CTE;
	//...

	//for internal material definition


	int X_Offset;
	int Y_Offset;
	int Z_Offset;

	//for external material definition
	std::string File;
};

#endif






