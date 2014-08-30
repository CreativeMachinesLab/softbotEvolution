#ifndef MARCHCUBE_H
#define MARCHCUBE_H

#include "Mesh.h"

//takes a vector of 3D arrays of floats and turns them into an iso-surface...
class CArray3Df;
//class CMesh;
//struct CColor;
//struct CVertex;

typedef struct {
   Vec3D p[3];
} TRIANGLE;

typedef struct {
   CVertex p[8];
   double val[8];
} GRIDCELL;

class CMarchCube
{
public:
	CMarchCube(void);
	~CMarchCube(void);

	static void SingleMaterial(CMesh* pMeshOut, CArray3Df* pArray, float Thresh = 0.0f, float Scale = 1.0f);
	
	static void SingleMaterialMultiColor(CMesh* pMeshOut, CArray3Df* pArray, CArray3Df* rColorArray, CArray3Df* gColorArray, CArray3Df* bColorArray, float Thresh = 0.0f, float Scale = 1.0f);
	
	static void MultiMaterial(CMesh* pMeshOut, void* pArrays, bool SumMat, CColor* pColors = NULL, float Thresh = 0.0f, float Scale = 1.0f);

//	MultiMaterial(...);

	static int Polygonise(GRIDCELL grid, double iso, CMesh* pMeshOut);
	static int PolygoniseCube(GRIDCELL grid, double iso, CMesh* pMeshOut);
	static int PolygoniseTet(GRIDCELL g, double iso, CMesh* pMeshOut, int v0,int v1,int v2,int v3);
	static CVertex VertexInterp(double iso, CVertex p1, CVertex p2, double valp1, double valp2);
};

#endif
