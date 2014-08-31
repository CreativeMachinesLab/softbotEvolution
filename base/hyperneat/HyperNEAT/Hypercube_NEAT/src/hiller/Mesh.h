#ifndef MESH_H
#define MESH_H

#include <vector>
#include "DigitalMatter.h"

//basic RGB color container
struct CColor {
	float r, g, b;
	CColor () {r = 0.5f; g = 0.5f; b = 0.6f;}
	CColor (const CColor& C) {r = C.r; g = C.g; b = C.b;}
	CColor (float rn, float gn, float bn) {r=rn; b=bn; g=gn;}
};

//structure to hold each vertex
struct CVertex {
	CVertex() {DrawOffset = Vec3D(0,0,0);}
	CVertex(const Vec3D& an, const Vec3D& av) {n = an; v = av; }
	CVertex(const Vec3D& av, const CColor& ac) {v = av; Color = ac; }
	CVertex(const CVertex& V) {n=V.n; v=V.v; Color = V.Color; DrawOffset = V.DrawOffset;} // DrawAxis = V.DrawAxis; DrawAngle = V.DrawAngle;}
	Vec3D n; //normal
	Vec3D v; //Vertex location
	CColor Color;
	Vec3D DrawOffset; //apply offset to each point
};

//structure to hold each facet
struct CFacet {
	CFacet() {}
//	CFacet(const Vec3D& an, const Vec3D& av1, const Vec3D& av2, const Vec3D& av3) : n(an), v1(av1), v2(av2), v3(av3) {}
	CFacet(const Vec3D& an, const int& av1, const int& av2, const int& av3) { n = an; vi[0] = av1; vi[1] = av2; vi[2] = av3;}
	CFacet(const CFacet& p) { vi[0]=p.vi[0]; vi[1]=p.vi[1]; vi[2]=p.vi[2]; n=p.n;}
	Vec3D n; //normal
	int vi[3]; //vertex indices
};


class CMesh
{
public:
	CMesh(void);
	~CMesh(void);

//	CMesh(CMesh& s); //jmc original was not const
	CMesh(const CMesh& s);
	CMesh& operator=(const CMesh& s);

	bool Exists(void) {if (Facets.size() != 0) return true; else return false;}
	std::vector<CFacet> Facets;
	std::vector<CVertex> Vertices;

	// file i/o
//	bool LoadSTL(std::string filename, int maxsize = 1000000); //Windows only for now
	bool Save(const char* filename) const;
	bool SaveThreeJS(const char* filename) const;
	bool SaveTrimmed(const char* filename) const;

	bool DrawSmooth;
	void CalcVertNormals(); //called once for each new geometry
	void Draw(bool bModelhNormals = false, bool bShaded = true); //requires OpenGL libs

	//add a facet
	void AddFacet(const Vec3D& v1, const Vec3D& v2, const Vec3D& v3, bool QuickAdd=false); //adds a facet, checks vertex list for existing vertices...
	void AddFacet(const Vec3D& v1, const Vec3D& v2, const Vec3D& v3, const CColor& Col1, const CColor& Col2, const CColor& Col3, bool QuickAdd=false); //adds a facet... with color info

	// clear/reset the list of trianges
	void Clear() { Facets.clear(); Vertices.clear();}
	//int Size() const { return (int)Facets.size(); }
	
	void ComputeBoundingBox(Vec3D& pmin, Vec3D& pmax);

	//manipulation...
	void Scale(Vec3D d);
	void Translate(Vec3D d);

	int GetXIntersections(double z, double y, double* pIntersections, int NumtoCheck, int* pToCheck); //returns the number of intersections, stored in pIntersections

	static bool InsideTri(Vec3D& p, Vec3D& v0, Vec3D& v1, Vec3D& v2);
	static double Det(Vec3D& v0, Vec3D& v1, Vec3D& v2);
	bool IntersectXRay(CFacet* pFacet, double y, double z, Vec3D& p, double& pu, double& pv);

	void WeldClose(float Distance); //welds vertices that are nearby (within Distance). Removes deleted triangles...


};
#endif
