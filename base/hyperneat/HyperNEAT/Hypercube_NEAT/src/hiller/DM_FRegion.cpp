#include "DM_FRegion.h"


CDM_FRegion::CDM_FRegion(void)
{
	Box = NULL;
	Cylinder = NULL;
	Sphere = NULL;

	ClearRegion();
}

CDM_FRegion::CDM_FRegion(const CDM_FRegion& iFRegion) //copy constructor
{
	Box = NULL;
	Cylinder = NULL;
	Sphere = NULL;

	ClearRegion(); //clear everything out...
	Fixed = iFRegion.Fixed;
	Force = iFRegion.Force;

	if (iFRegion.Box != NULL) //if a box...
		CreateBoxRegion(Vec3D(iFRegion.Box->X, iFRegion.Box->Y, iFRegion.Box->Z), Vec3D(iFRegion.Box->dX, iFRegion.Box->dY, iFRegion.Box->dZ));
	else if (iFRegion.Cylinder != NULL) //if a box...
		CreateCylRegion(Vec3D(iFRegion.Cylinder->X, iFRegion.Cylinder->Y, iFRegion.Cylinder->Z), Vec3D(iFRegion.Cylinder->dX, iFRegion.Cylinder->dY, iFRegion.Cylinder->dZ), iFRegion.Cylinder->Radius);
	else if (iFRegion.Sphere != NULL) //if a box...
		CreateSphRegion(Vec3D(iFRegion.Sphere->X, iFRegion.Sphere->Y, iFRegion.Sphere->Z), iFRegion.Sphere->Radius);

	SetColor(iFRegion.pRegion->R, iFRegion.pRegion->G, iFRegion.pRegion->B, iFRegion.pRegion->alpha);

}

CDM_FRegion::~CDM_FRegion(void)
{
	ResetRegion();
}


CDM_FRegion& CDM_FRegion::operator=(const CDM_FRegion& iFRegion)
{
	ClearRegion(); //clear everything out...
	Fixed = iFRegion.Fixed;
	Force = iFRegion.Force;

	if (iFRegion.Box != NULL) //if a box...
		CreateBoxRegion(Vec3D(iFRegion.Box->X, iFRegion.Box->Y, iFRegion.Box->Z), Vec3D(iFRegion.Box->dX, iFRegion.Box->dY, iFRegion.Box->dZ));
	else if (iFRegion.Cylinder != NULL) //if a cylinder...
		CreateCylRegion(Vec3D(iFRegion.Cylinder->X, iFRegion.Cylinder->Y, iFRegion.Cylinder->Z), Vec3D(iFRegion.Cylinder->dX, iFRegion.Cylinder->dY, iFRegion.Cylinder->dZ), iFRegion.Cylinder->Radius);
	else if (iFRegion.Sphere != NULL) //if a sphere...
		CreateSphRegion(Vec3D(iFRegion.Sphere->X, iFRegion.Sphere->Y, iFRegion.Sphere->Z), iFRegion.Sphere->Radius);

	SetColor(iFRegion.pRegion->R, iFRegion.pRegion->G, iFRegion.pRegion->B, iFRegion.pRegion->alpha);

	return *this;

}

void CDM_FRegion::ReadXML(CXML_Rip* pXML)
{
	
}

void CDM_FRegion::WriteXML(CXML_Rip* pXML)
{
	pXML->DownLevel("FRegion");

	int PrimType;
	if (Box != NULL) PrimType = PRIM_BOX;
	if (Cylinder != NULL) PrimType = PRIM_CYLINDER;
	if (Sphere != NULL) PrimType = PRIM_SPHERE;
	pXML->Element("PrimType", PrimType);

	pXML->Element("X", pRegion->X);
	pXML->Element("Y", pRegion->Y);
	pXML->Element("Z", pRegion->Z);
	pXML->Element("dX", pRegion->dX);
	pXML->Element("dY", pRegion->dY);
	pXML->Element("dZ", pRegion->dZ);
	pXML->Element("Radius", pRegion->Radius);
	pXML->Element("R", pRegion->R);
	pXML->Element("G", pRegion->G);
	pXML->Element("B", pRegion->B);
	pXML->Element("alpha", pRegion->alpha);
	pXML->Element("Fixed", Fixed);
	pXML->Element("ForceX", Force.x);
	pXML->Element("ForceY", Force.y);
	pXML->Element("ForceZ", Force.z);

	pXML->UpLevel();
}

void CDM_FRegion::ClearRegion() //clears everything here...
{
	ResetRegion();
	
	Fixed = false;
	Force = Vec3D(0,0,0);
}

void CDM_FRegion::ResetRegion(void) //clears primitive info
{
	pRegion = NULL;
	
	if (Box != NULL) delete Box;
	Box = NULL;
	if (Cylinder != NULL) delete Cylinder;
	Cylinder = NULL;
	if (Sphere != NULL) delete Sphere;
	Sphere = NULL;
}

void CDM_FRegion::CreateBoxRegion(Vec3D Pos, Vec3D Dim)
{
	ResetRegion();

	Box = new CP_Box;
	Box->X = (float)Pos.x;
	Box->Y = (float)Pos.y;
	Box->Z = (float)Pos.z;
	Box->dX = (float)Dim.x;
	Box->dY = (float)Dim.y;
	Box->dZ = (float)Dim.z;
	pRegion = Box;
}

void CDM_FRegion::CreateBoxRegion(void) //use defaults
{
	ResetRegion();
	Box = new CP_Box;
	pRegion = Box;
}

void CDM_FRegion::CreateCylRegion(Vec3D Pos, Vec3D Axis, float Rad)
{
	ResetRegion();

	Cylinder = new CP_Cylinder;
	Cylinder->X = (float)Pos.x;
	Cylinder->Y = (float)Pos.y;
	Cylinder->Z = (float)Pos.z;
	Cylinder->dX = (float)Axis.x;
	Cylinder->dY = (float)Axis.y;
	Cylinder->dZ = (float)Axis.z;
	Cylinder->Radius = Rad;
	pRegion = Cylinder;
}

void CDM_FRegion::CreateCylRegion() //use defaults
{
	ResetRegion();
	Cylinder = new CP_Cylinder;
	pRegion = Cylinder;
}

void CDM_FRegion::CreateSphRegion(Vec3D Pos, float Rad)
{
	ResetRegion();

	Sphere = new CP_Sphere;
	Sphere->X = (float)Pos.x;
	Sphere->Y = (float)Pos.y;
	Sphere->Z = (float)Pos.z;
	Sphere->Radius = Rad;
	pRegion = Sphere;
}

void CDM_FRegion::CreateSphRegion(void)
{
	ResetRegion();
	Sphere = new CP_Sphere;
	pRegion = Sphere;
}

void CDM_FRegion::SetColor(float r, float g, float b, float alpha)
{
	pRegion->R = r;
	pRegion->G = g;
	pRegion->B = b;
	pRegion->alpha = alpha;
}

void CDM_FRegion::ScaleTo(Vec3D OldDims, Vec3D NewDims) //scales to fit new envelope provided
{
	pRegion->X *= (float)(NewDims.x/OldDims.x);
	pRegion->Y *= (float)(NewDims.y/OldDims.y);
	pRegion->Z *= (float)(NewDims.z/OldDims.z);
	pRegion->dX *= (float)(NewDims.x/OldDims.x);
	pRegion->dY *= (float)(NewDims.y/OldDims.y);
	pRegion->dZ *= (float)(NewDims.z/OldDims.z);
	pRegion->Radius *= (float)(NewDims.x/OldDims.x);
}



//BOX
CP_Box::CP_Box(void)
{
	X = 0;
	Y = 0;
	Z = 0;
	dX = 0.001f;
	dY = 0.001f;
	dZ = 0.001f;
	R = 0.5f;
	G = 0.5f;
	B = 0.5f;
	Radius = 0;
	alpha = 1.0f;
}

CP_Box::~CP_Box(void)
{
}

bool CP_Box::IsIn(Vec3D* P)
{
	if (P->x > X && P->x < X+dX && P->y > Y && P->y < Y+dY && P->z > Z && P->z < Z+dZ) //if contained...
		return true;
	else
		return false;
}

bool CP_Box::IsTouching(Vec3D* P, float Dist)
{
	if (P->x+Dist > X  &&  P->x-Dist < X+dX  &&  P->y+Dist > Y  &&  P->y-Dist < Y+dY  &&  P->z+Dist > Z  &&  P->z-Dist < Z+dZ) //if contained including offsets...
		return true;
	else
		return false;
}


bool CP_Box::IsTouching(Vec3D* P, Vec3D* Dist)
{
	if (P->x+Dist->x > X  &&  P->x-Dist->x < X+dX  &&  P->y+Dist->y > Y  &&  P->y-Dist->y < Y+dY  &&  P->z+Dist->z > Z  &&  P->z-Dist->z < Z+dZ) //if contained including offsets...
		return true;
	else
		return false;
}




//Cylinder:

CP_Cylinder::CP_Cylinder(void)
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
	Radius = 0.001f;
	dX = 0.0f;
	dY = 0.0f;
	dZ = 0.001f;
	R = 0.5f;
	G = 0.5f;
	B = 0.5f;
	alpha = 1.0f;
}

CP_Cylinder::~CP_Cylinder(void)
{
}

bool CP_Cylinder::IsIn(Vec3D* P)
{
	//only 1 axis should be non-zero!
	Vec3D ThruOr = Vec3D (P->x-X, P->y-Y, P->z-Z);
	//Vec3D dDim = Vec3D(dX, dY, dZ);
	//double NormDist = ThruOr.Dot(dDim);
	//Vec3D ClosePoint = ThruOr/ThruOr.Length()*NormDist;
	//Vec3D vRadius = Vec3D(ThruOr.x - ClosePoint.x, ThruOr.y - ClosePoint.y, ThruOr.z - ClosePoint.z);
	//float thisRad = vRadius.Length();

	if (dX != 0){
		if (P->x > X && P->x < X+dX && sqrt((P->y-Y)*(P->y-Y) + (P->z-Z)*(P->z-Z)) < Radius)
			return true;
	}
	else if (dY != 0){
		if (P->y > Y && P->y < Y+dY && sqrt((P->x-X)*(P->x-X) + (P->z-Z)*(P->z-Z)) < Radius)
			return true;
	}
	else if (dZ != 0){
		if (P->z > Z && P->z < Z+dZ && sqrt((P->x-X)*(P->x-X) + (P->y-Y)*(P->y-Y)) < Radius)
			return true;
	}

	return false;
}

bool CP_Cylinder::IsTouching(Vec3D* P, float Dist)
{
	return (IsIn(P)); //TODO: fix this when needed
}

bool CP_Cylinder::IsTouching(Vec3D* P, Vec3D* Dist)
{
	return (IsIn(P)); //TODO: fix this when needed
}


//Sphere:

CP_Sphere::CP_Sphere(void)
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
	Radius = 0.001f;
	R = 0.5f;
	G = 0.5f;
	B = 0.5f;
	alpha = 1.0f;
}

CP_Sphere::~CP_Sphere(void)
{
}

bool CP_Sphere::IsIn(Vec3D* P)
{
	if (sqrt((P->x-X)*(P->x-X)+(P->y-Y)*(P->y-Y)+(P->z-Z)*(P->z-Z)) < Radius)
		return true;
	else
		return false;
}

bool CP_Sphere::IsTouching(Vec3D* P, float Dist)
{
	if (sqrt((P->x-X)*(P->x-X)+(P->y-Y)*(P->y-Y)+(P->z-Z)*(P->z-Z)) < Radius+Dist) //test against spherical region around point
		return true;
	else
		return false;
}

bool CP_Sphere::IsTouching(Vec3D* P, Vec3D* Dist)
{
	return IsTouching(P, (float)Dist->Length());
}
