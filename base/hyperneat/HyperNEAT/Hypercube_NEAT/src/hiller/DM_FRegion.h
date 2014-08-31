#ifndef CDM_FREGION_H
#define CDM_FREGION_H
#include "XML_Rip.h" 
#include "DigitalMatter.h" //to get Vec3D


class CPrimitive;
class CP_Box;
class CP_Cylinder;
class CP_Sphere;


#define PRIM_BOX 0 //pasted.. just something to keep track of different primitives
#define PRIM_CYLINDER 1
#define PRIM_SPHERE 2

class CDM_FRegion //holds a region and directed force...
{
public:
	CDM_FRegion(void);
	~CDM_FRegion(void);
	CDM_FRegion(const CDM_FRegion& iFRegion); //copy constructor
	CDM_FRegion& operator=(const CDM_FRegion& iFRegion); //overload "=" 

	void ReadXML(CXML_Rip* pXML);
	void WriteXML(CXML_Rip* pXML);

//	void Write(std::ofstream* pFile);
//	void Read(CStdioFile* pFile);

	//variables
	CPrimitive* pRegion;

	//possible types...
	CP_Box* Box;
	CP_Cylinder* Cylinder;
	CP_Sphere* Sphere;

	bool Fixed;
	Vec3D Force;

	//functions
	void CreateBoxRegion(void);
	void CreateBoxRegion(Vec3D Pos, Vec3D Dim);
	void CreateCylRegion(void);
	void CreateCylRegion(Vec3D Pos, Vec3D Axis, float Rad);
	void CreateSphRegion(void);
	void CreateSphRegion(Vec3D Pos, float Rad);
	void SetColor(float r, float g, float b, float alpha);
	void ScaleTo(Vec3D OldDims, Vec3D NewDims);

	void ClearRegion();
	void ResetRegion(); //doesn't reset fixed or force...
};


class CPrimitive
{
public:
	CPrimitive(void) {};
	~CPrimitive(void) {};

	virtual bool IsIn(Vec3D* P) = 0;
	virtual bool IsTouching(Vec3D* P, float Dist) = 0;
	virtual bool IsTouching(Vec3D* P, Vec3D* Dist) = 0; //(for non-1 aspect ratios

	float X;
	float Y;
	float Z;
	float Radius;
	float dX;
	float dY;
	float dZ;

	float R;
	float G;
	float B;
	float alpha;

};

class CP_Box : public CPrimitive
{
public:
	CP_Box(void);
	~CP_Box(void);

	virtual bool IsIn(Vec3D* P);
	virtual bool IsTouching(Vec3D* P, float Dist);
	virtual bool IsTouching(Vec3D* P, Vec3D* Dist); //(for non-1 aspect ratios
};

class CP_Cylinder : public CPrimitive
{
public:
	CP_Cylinder(void);
	~CP_Cylinder(void);

	virtual bool IsIn(Vec3D* P);
	virtual bool IsTouching(Vec3D* P, float Dist);
	virtual bool IsTouching(Vec3D* P, Vec3D* Dist); //(for non-1 aspect ratios
};

class CP_Sphere : public CPrimitive
{
public:
	CP_Sphere(void);
	~CP_Sphere(void);

	virtual bool IsIn(Vec3D* P);
	virtual bool IsTouching(Vec3D* P, float Dist);
	virtual bool IsTouching(Vec3D* P, Vec3D* Dist); //(for non-1 aspect ratios
};

#endif
