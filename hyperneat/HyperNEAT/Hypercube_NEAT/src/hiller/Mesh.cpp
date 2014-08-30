#include "Mesh.h"
#include <cmath>
#include "Experiments/HCUBE_ShapesExperiment.h"

     
#ifdef VISUALIZESHAPES
	//#include <SFML/Graphics.hpp>
#endif


//Elements adapted from FAH code... (Thanks Evan and Hod)

CMesh::CMesh(void)
{
	DrawSmooth = true;
}

CMesh::~CMesh(void)
{
}

//copy constructure
//CMesh::CMesh(CMesh& s) { //jmc original was not const
CMesh::CMesh(const CMesh& s) {
	*this = s;
}

//overload =
CMesh& CMesh::operator=(const CMesh& s) {

	Facets.resize(s.Facets.size());
	for (int i = 0; i<(int)Facets.size(); i++)
		Facets[i] = s.Facets[i];

	Vertices.resize(s.Vertices.size());
	for (int i = 0; i<(int)Vertices.size(); i++)
		Vertices[i] = s.Vertices[i];
	
	DrawSmooth = s.DrawSmooth;

	return *this;
}

/* Windows only for now. Feel free to port and send to jdh74@cornell.edu
bool CMesh::LoadSTL(std::string filename, int maxsize) { // import stl file (binary or ascii)

typedef struct {
	char	description[80];
	long	nfacets;
} STLHEADER;

typedef struct {
	float	nx,ny,nz;
	float	x1,y1,z1;
	float	x2,y2,z2;
	float	x3,y3,z3;
	char	pad[2];
} STLFACETINFO;

	//CFacet tri;
	Vec3D tmpV1, tmpV2, tmpV3;
	int cnt;

	// read ascii

	std::string line;
	CStdioFile afile;
	if (afile.Open(filename, CFile::modeRead) == 0)
		return false;
	BOOL ok = afile.ReadString(line);
	line.TrimLeft();
	if (line.Left(5).CompareNoCase("solid")!=0) {
		afile.Close();
		goto BINARY;
	}

	// read facet loops

	do {
		//		facet normal x y z
		//		outer loop
		//		vertex x y z
		//		vertex x y z
		//		vertex x y z
		//		endloop
		//		endfacet
		//		endsolid name_of_object

		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		if (line.Left(8).CompareNoCase("endsolid")!=0 && line.Left(5).CompareNoCase("facet")!=0) {
			afile.Close();
			goto BINARY;
		}

		if (line.Left(8).CompareNoCase("endsolid")==0) {
			afile.Close();
			break;
		}
		//don't care about the normal (We'll recalculate anyway...
//		cnt = sscanf_s(line, "facet normal %lf %lf %lf", NULL, NULL, NULL);
//		if (!ok || cnt != 3) {
//			AfxMessageBox("Error during read. Expected 'facet normal nx ny nz'\n"+line);
//			break;
//		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		if (!ok || line.Left(10).CompareNoCase("outer loop")!=0) {
			AfxMessageBox("Error during read. Expected 'outer loop'\n"+line);
			break;
		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		cnt = sscanf_s(line, "vertex %lf %lf %lf", &tmpV1.x, &tmpV1.y, &tmpV1.z);
		if (!ok || cnt != 3) {
			AfxMessageBox("Error during read. Expected 'vertex nx ny nz'\n"+line);
			break;
		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		cnt = sscanf_s(line, "vertex %lf %lf %lf", &tmpV2.x, &tmpV2.y, &tmpV2.z);
		if (!ok || cnt != 3) {
			AfxMessageBox("Error during read. Expected 'vertex nx ny nz'\n"+line);
			break;
		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		cnt = sscanf_s(line, "vertex %lf %lf %lf", &tmpV3.x, &tmpV3.y, &tmpV3.z);
		if (!ok || cnt != 3) {
			AfxMessageBox("Error during read. Expected 'vertex nx ny nz'\n"+line);
			break;
		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		if (!ok || line.Left(7).CompareNoCase("endloop")!=0) {
			AfxMessageBox("Error during read. Expected 'endloop'\n"+line);
			break;
		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		if (!ok || line.Left(8).CompareNoCase("endfacet")!=0) {
			AfxMessageBox("Error during read. Expected 'endfacet'\n"+line);
			break;
		}
		
		AddFacet(tmpV1, tmpV2, tmpV3); //add the facet, calc the normal, check existing vertices...

	} while (ok);


	return true;

	// read binary

BINARY:

	CFile file;
	if (file.Open(filename, CFile::modeRead) == 0)
		return false;

	STLHEADER header;
	STLFACETINFO facetinfo;
	UINT nBytesRead = file.Read(&header, sizeof(header));

	// check

	if (nBytesRead < sizeof(header) || header.nfacets <= 0) {
		AfxMessageBox("Corrupted file.");
		return false;
	}

	if (header.nfacets > maxsize) {
		std::string text;
		text.Format("This file contains %d facets. This number of facets will result it slow graphic update.\n"
			"Please regenerate at lower resolution; meanwhile, truncating to %d\n", header.nfacets, maxsize);
		header.nfacets = maxsize;
	}

	// load

	int i;

	//mesh.SetSize(header.nfacets);
//	Facets.resize(header.nfacets);
	
//	double maxx = -1E30, maxy = -1E30, maxz = -1E30, minx = 1E30, miny = 1E30, minz = 1E30;
	for (i=0; i<min(header.nfacets,maxsize); i++) {
		nBytesRead = file.Read(&facetinfo, 50);
		if (nBytesRead < 50) {
			AfxMessageBox("Corrupted file - premature end of file.");
			return 0;
		}
		AddFacet(Vec3D(facetinfo.x1,facetinfo.y1,facetinfo.z1), Vec3D(facetinfo.x2,facetinfo.y2,facetinfo.z2), Vec3D(facetinfo.x3,facetinfo.y3,facetinfo.z3));
//		Facets[i].n.Normalize();
//		maxx = max(max(maxx,facetinfo.x1),max(facetinfo.x2,facetinfo.x3));
//		minx = min(min(minx,facetinfo.x1),min(facetinfo.x2,facetinfo.x3));
//		maxy = max(max(maxy,facetinfo.y1),max(facetinfo.y2,facetinfo.y3));
//		miny = min(min(miny,facetinfo.y1),min(facetinfo.y2,facetinfo.y3));
//		maxz = max(max(maxz,facetinfo.z1),max(facetinfo.z2,facetinfo.z3));
//		minz = min(min(minz,facetinfo.z1),min(facetinfo.z2,facetinfo.z3));
	}
	file.Close();

//	double size = max(max(maxx-minx,maxy-miny),maxz-minz);

	CalcVertNormals();

	return true;

}
*/

bool CMesh::Save(const char* filename) const { //writes ascii stl file...
	FILE* file;
	//	fopen_s(&file, filename, "w");
	file = fopen(filename, "w");

	if (!file) return false;
	
	fprintf(file, "solid STL1\n");
	for (int i=0; i<(int)Facets.size(); ++i) //WHY ++i??? ISNT VECTOR 0-BASED?
	{
		fprintf(file, "facet normal %g %g %g\n", Facets[i].n.x, Facets[i].n.y, Facets[i].n.z);
		fprintf(file, "outer loop\n");
		fprintf(file, "vertex %g %g %g\n", Vertices[Facets[i].vi[0]].v.x, Vertices[Facets[i].vi[0]].v.y, Vertices[Facets[i].vi[0]].v.z);
		fprintf(file, "vertex %g %g %g\n", Vertices[Facets[i].vi[1]].v.x, Vertices[Facets[i].vi[1]].v.y, Vertices[Facets[i].vi[1]].v.z);
		fprintf(file, "vertex %g %g %g\n", Vertices[Facets[i].vi[2]].v.x, Vertices[Facets[i].vi[2]].v.y, Vertices[Facets[i].vi[2]].v.z);
		fprintf(file, "endloop\n");
		fprintf(file, "endfacet\n");
	}
	fprintf(file, "endsolid STL1\n");
	
	fclose(file);
	return true;
}

bool CMesh::SaveTrimmed(const char* filename) const { //writes ascii stl file...
	FILE* file;
	//	fopen_s(&file, filename, "w");
	file = fopen(filename, "w");
	
	if (!file) return false;
	
	for (int i=0; i<(int)Facets.size(); ++i) //WHY ++i??? ISNT VECTOR 0-BASED?
	{
		fprintf(file, "v (%g, %g, %g);\n", Vertices[Facets[i].vi[0]].v.x, Vertices[Facets[i].vi[0]].v.y, Vertices[Facets[i].vi[0]].v.z);
		fprintf(file, "v (%g, %g, %g);\n", Vertices[Facets[i].vi[1]].v.x, Vertices[Facets[i].vi[1]].v.y, Vertices[Facets[i].vi[1]].v.z);
		fprintf(file, "v (%g, %g, %g);\n", Vertices[Facets[i].vi[2]].v.x, Vertices[Facets[i].vi[2]].v.y, Vertices[Facets[i].vi[2]].v.z);
	}
	
	
	fclose(file);
	return true;
}


// Writes a file compatible with our Three.js format
bool CMesh::SaveThreeJS(const char* filename) const {
	FILE* file;
	file = fopen(filename, "w");
	
	if (!file) return false;

    // Add format line
    /*
    fprintf(file, "v0.3\n");
	for (int i=0; i<(int)Facets.size(); ++i)
	{
        // vf format 0.2
		//fprintf(file, "v(%g, %g, %g);\n", Vertices[Facets[i].vi[0]].v.x, Vertices[Facets[i].vi[0]].v.y, Vertices[Facets[i].vi[0]].v.z);
		//fprintf(file, "v(%g, %g, %g);\n", Vertices[Facets[i].vi[1]].v.x, Vertices[Facets[i].vi[1]].v.y, Vertices[Facets[i].vi[1]].v.z);
		//fprintf(file, "v(%g, %g, %g);\n", Vertices[Facets[i].vi[2]].v.x, Vertices[Facets[i].vi[2]].v.y, Vertices[Facets[i].vi[2]].v.z);
		//fprintf(file, "f3(%d, %d, %d);\n", 3*i, 3*i+1, 3*i+2);

        // vf format 0.3
		fprintf(file, "%.3g %.3g %.3g\n", Vertices[Facets[i].vi[0]].v.x, Vertices[Facets[i].vi[0]].v.y, Vertices[Facets[i].vi[0]].v.z);
		fprintf(file, "%.3g %.3g %.3g\n", Vertices[Facets[i].vi[1]].v.x, Vertices[Facets[i].vi[1]].v.y, Vertices[Facets[i].vi[1]].v.z);
		fprintf(file, "%.3g %.3g %.3g\n", Vertices[Facets[i].vi[2]].v.x, Vertices[Facets[i].vi[2]].v.y, Vertices[Facets[i].vi[2]].v.z);
	}
    */

    // vf format 0.4
    fprintf(file, "v0.4\n");
	//    fprintf(file, "v0.5\n"); //JMC: uncomment when ready for color (and uncomment line just below with the word color in its comment)
    fprintf(file, "%i\n", Vertices.size());
	for (int ii=0; ii<(int)Vertices.size(); ++ii)
	{
		fprintf(file, "%.3g %.3g %.3g %.3g %.3g %.3g\n", Vertices[ii].v.x, Vertices[ii].v.y, Vertices[ii].v.z, Vertices[ii].n.x, Vertices[ii].n.y, Vertices[ii].n.z);

		//delete previous line, and uncomment following line, to add color
//		fprintf(file, "%.3g %.3g %.3g %.3g %.3g %.3g %.3g %.3g %.3g\n", 
//				Vertices[ii].v.x, Vertices[ii].v.y, Vertices[ii].v.z, 
//				Vertices[ii].n.x, Vertices[ii].n.y, Vertices[ii].n.z,
//				Vertices[ii].Color.r,Vertices[ii].Color.g,Vertices[ii].Color.b);
	
	}
    fprintf(file, "%i\n", Facets.size());
	for (int ii=0; ii<(int)Facets.size(); ++ii)
	{
		fprintf(file, "%i %i %i\n", Facets[ii].vi[0], Facets[ii].vi[1], Facets[ii].vi[2]);
	}
	
	fclose(file);
	return true;
}


//---------------------------------------------------------------------------
void CMesh::Draw(bool bModelhNormals, bool bShaded)
//---------------------------------------------------------------------------
{
	
#ifdef VISUALIZESHAPES


// jmc hand coded example triangle
//	glBegin(GL_TRIANGLES);	
//    glVertex3f(0.45f, 0.45f, 0.45f);    // lower left vertex
//    glVertex3f(0.35f, 0.35f, 0.35f);    // lower right vertex
//    glVertex3f(1.45f, 0.45f, 0.45f);    // upper vertex	
//    glEnd();
/*
	if (bShaded) {
		glBegin(GL_TRIANGLES);
		for (int i=0; i<(int)Facets.size(); i++) {
			for (int j=0; j<3; j++) {
				CVertex& CurVert = Vertices[Facets[i].vi[j]]; //just a local reference for readability

				if (DrawSmooth){ //if we want to draw smoothed normals
					glNormal3d(CurVert.n.x, CurVert.n.y, CurVert.n.z);
				}
				else
					glNormal3d(Facets[i].n.x, Facets[i].n.y, Facets[i].n.z);

				glColor3d(CurVert.Color.r, CurVert.Color.g, CurVert.Color.b);

				double xv = (CurVert.v.x + CurVert.DrawOffset.x);
//				double xv = (CurVert.v.x + CurVert.DrawOffset.x-10); //original
//				double yv = (CurVert.v.y + CurVert.DrawOffset.y-20); //original
				double yv = (CurVert.v.y + CurVert.DrawOffset.y-10);
				double zv = (CurVert.v.z + CurVert.DrawOffset.z);
				
				glVertex3d(xv, yv, zv); //jmc: more readable
				
//				printf("x: %f", xv);
//				printf(" y: %f", yv);
//				printf(" z: %f\n", zv);
			}
		}
		glEnd();
	}
	else { // wireframe
		glDisable(GL_LIGHTING);

		glColor4f(1.0, 1.0, 1.0, 1.0);
		for (int i=0; i<(int)Facets.size(); i++) {
			glBegin(GL_LINE_LOOP);
			glNormal3d(Facets[i].n.x, Facets[i].n.y, Facets[i].n.z);
			for (int j=0; j<3; j++) {
				CVertex& CurVert = Vertices[Facets[i].vi[j]]; //just a local reference for readability
				//glColor3d(CurVert.Color.r, CurVert.Color.g, CurVert.Color.b);
				glVertex3d(CurVert.v.x + CurVert.DrawOffset.x , CurVert.v.y + CurVert.DrawOffset.y, CurVert.v.z + CurVert.DrawOffset.z);
			}
			glEnd();
		}
		glEnable(GL_LIGHTING);

	}

	if (bModelhNormals) {
		glColor3d(1,1,0);
		glBegin(GL_LINES);
		for (int i=0; i<(int)Facets.size(); i++) {
			Vec3D c = (Vertices[Facets[i].vi[0]].v + Vertices[Facets[i].vi[1]].v + Vertices[Facets[i].vi[2]].v)/3;
			//			Vec3D c2 = c - Facets[i].n*3; //jmc not compiling
			Vec3D tempVec3d = Facets[i].n*3;
			Vec3D c2 = c - tempVec3d; 
			
			glVertex3d(c.x, c.y, c.z);
			glVertex3d(c2.x, c2.y, c2.z);
		}
		glEnd();
	}
*/
#endif //end #ifdef VISUALIZESHAPES



}


//---------------------------------------------------------------------------
void CMesh::CalcVertNormals()
//---------------------------------------------------------------------------
{ //called once for each new geometry
	//fills in Vertices.n
	for (int i=0; i<(int)Vertices.size(); i++){
		Vertices[i].n = Vec3D(0,0,0);
	}

	for (int i=0; i<(int)Facets.size(); i++){

		for (int j=0; j<3; j++){
			Vertices[Facets[i].vi[j]].n += Facets[i].n;
		}
	}

	for (int i=0; i<(int)Vertices.size(); i++){
		Vertices[i].n.Normalize();
	}
}

void CMesh::AddFacet(const Vec3D& v1, const Vec3D& v2, const Vec3D& v3, bool QuickAdd) //adds a facet, checks vertex list for existing vertices...
{
	AddFacet(v1, v2, v3, CColor(0.5, 0.5, 0.5), CColor(0.5, 0.5, 0.5), CColor(0.5, 0.5, 0.5), QuickAdd);
}

//---------------------------------------------------------------------------
void CMesh::AddFacet(const Vec3D& v1, const Vec3D& v2, const Vec3D& v3, const CColor& Col1, const CColor& Col2, const CColor& Col3, bool QuickAdd) //adds a facet... with color info
//---------------------------------------------------------------------------
{
	double WeldThresh = 1e-10; //This needs to be around the precision of a float.

	Vec3D Points[3]; //make a local array for easy referencing
	Points[0] = v1;
	Points[1] = v2;
	Points[2] = v3;
	CColor Colors[3];
	Colors[0] = Col1;
	Colors[1] = Col2;
	Colors[2] = Col3;


	int FoundIndex[3]; //each index for a triangle...

	for (int j=0; j<3; j++){ //each point in this facet
		FoundIndex[j] = -1;
		
		if(!QuickAdd){
			for (int k=Vertices.size()-1; k>=0; k--){ //DO THIS BACKWARDS!!!! (more likely to have just added one next to us...)
				if (fabs(Points[j].x - Vertices[k].v.x) < WeldThresh  &&  fabs(Points[j].y - Vertices[k].v.y) < WeldThresh  &&  fabs(Points[j].z - Vertices[k].v.z) < WeldThresh){ //if points are identical...
					FoundIndex[j] = k;
					break; //kicks out of for loop, because we've found!
				}
			}
		}
		if (FoundIndex[j] == -1){ //if we didn't find one...
			CVertex ThisPoint;
			ThisPoint.v.x = Points[j].x;
			ThisPoint.v.y = Points[j].y;
			ThisPoint.v.z = Points[j].z;
			ThisPoint.Color = Colors[j];

			Vertices.push_back(ThisPoint);
			FoundIndex[j] = (int)Vertices.size() - 1; //-1 because zero-index based.
		}

	}

	CFacet ThisFacet;
	for (int m=0; m<3; m++) ThisFacet.vi[m] = FoundIndex[m];

	//get the normal:
	ThisFacet.n = ((Vertices[ThisFacet.vi[1]].v-Vertices[ThisFacet.vi[0]].v).Cross(Vertices[ThisFacet.vi[2]].v-Vertices[ThisFacet.vi[0]].v)).Normalized();

	Facets.push_back(ThisFacet); //TODO... selct whether to create new object or add to existing...


}

//---------------------------------------------------------------------------
void CMesh::ComputeBoundingBox(Vec3D &pmin, Vec3D &pmax)
//---------------------------------------------------------------------------
{

	if (Vertices.size() == 0)
		return;

	pmin = pmax = Vertices[0].v;
	
	for (int i=0; i<(int)Vertices.size(); i++) {
		pmin.x = std::min(pmin.x, Vertices[i].v.x);
		pmin.y = std::min(pmin.y, Vertices[i].v.y);
		pmin.z = std::min(pmin.z, Vertices[i].v.z);
		pmax.x = std::max(pmax.x, Vertices[i].v.x);
		pmax.y = std::max(pmax.y, Vertices[i].v.y);
		pmax.z = std::max(pmax.z, Vertices[i].v.z);
	}

}


//---------------------------------------------------------------------------
void CMesh::Translate(Vec3D d)
//---------------------------------------------------------------------------
{// translate geometry
	for (int i=0; i<(int)Vertices.size(); i++) {
		Vertices[i].v += d;
	}
}

//---------------------------------------------------------------------------
void CMesh::Scale(Vec3D s)
//---------------------------------------------------------------------------
{// scale geometry

	//check for zero scale factor
	if(s.x==0 || s.y==0 || s.z==0) return;
	for (int i=0; i<(int)Vertices.size(); i++) {
		Vertices[i].v.x *= s.x;
		Vertices[i].v.y *= s.y;
		Vertices[i].v.z *= s.z;
//		Vertices[i].n.x *= s.x; //do we really want to scale these?
//		Vertices[i].n.y *= s.y;
//		Vertices[i].n.z *= s.z;
///		Facets[i].n.Normalize();
	}

}

//---------------------------------------------------------------------------
int CMesh::GetXIntersections(double z, double y, double* pIntersections, int NumtoCheck, int* pToCheck)
//---------------------------------------------------------------------------
{ //returns the number of intersections, stored in pIntersections. pToCheck is a vector of facet indices that are in this Z plane...
	Vec3D p;
	double pu, pv, V1y, V2y, V3y;
	int NumFound = 0;

	for (int i=0; i<NumtoCheck; i++){ //for each facet we wish to check...
		V1y = Vertices[Facets[pToCheck[i]].vi[0]].v.y;
		V2y = Vertices[Facets[pToCheck[i]].vi[1]].v.y;
		V3y = Vertices[Facets[pToCheck[i]].vi[2]].v.y;
		//trivial checks (should get most of them...)
		if (V1y < y && V2y < y && V3y < y)
			continue;
		if (V1y > y && V2y > y && V3y > y)
			continue;

		if(IntersectXRay(&Facets[pToCheck[i]], y, z, p, pu, pv)) { //if it intersects
			if (InsideTri(p, Vertices[Facets[pToCheck[i]].vi[0]].v, Vertices[Facets[pToCheck[i]].vi[1]].v, Vertices[Facets[pToCheck[i]].vi[2]].v)){
				pIntersections[NumFound++] = p.x; //(1.0 - pu - pv)*Vertices[Facets[pToCheck[i]].vi[0]].v.x + pu*Vertices[Facets[pToCheck[i]].vi[1]].v.x + pv*Vertices[Facets[pToCheck[i]].vi[2]].v.x;
			}
		}
	}
	
	if (NumFound%2 != 0) std::cout << "Uh-oh! Found an odd number of intersections!";
	
	//sort intersections... (bubble sort = slow, but these should be super small...
	double tmp;
	for (int i=0; i<NumFound; i++){
		for (int j=0; j<NumFound - i - 1; j++){ //each iteration gets the largest element to the end...
			if(pIntersections[j] > pIntersections[j+1]){
				tmp = pIntersections[j+1];
				pIntersections[j+1] = pIntersections[j];
				pIntersections[j] = tmp;
			}
		}
	}

	return NumFound;
}

//---------------------------------------------------------------------------
bool CMesh::InsideTri(Vec3D& p, Vec3D& v0, Vec3D& v1, Vec3D& v2)
//---------------------------------------------------------------------------
{// True if point p projects to within triangle (v0;v1;v2)


	Vec3D xax = (v1-v0).Normalized();
	Vec3D zax = ((v2-v0).Cross(xax)).Normalized();
	Vec3D yax = zax.Cross(xax).Normalized();

	Vec3D p0(0,0,1);
	Vec3D p1((v1-v0).Dot(xax),(v1-v0).Dot(yax),1);
	Vec3D p2((v2-v0).Dot(xax),(v2-v0).Dot(yax),1);
	Vec3D pt((p-v0).Dot(xax),(p-v0).Dot(yax),1);

	double d0 = Det(p0,p1,pt);
	double d1 = Det(p1,p2,pt);
	double d2 = Det(p2,p0,pt);

	if (d0<=0 && d1<=0 && d2<=0)
		return true;
	if (d0>=0 && d1>=0 && d2>=0)
		return true;

	return false;

}

//---------------------------------------------------------------------------
double CMesh::Det(Vec3D& v0, Vec3D& v1, Vec3D& v2)
//---------------------------------------------------------------------------
{ // Compute determinant of 3x3 matrix v0,v1,v2

	return 

		v0.x*(v1.y*v2.z-v1.z*v2.y) +
		v0.y*(v1.z*v2.x-v1.x*v2.z) +
		v0.z*(v1.x*v2.y-v1.y*v2.x);

}

//---------------------------------------------------------------------------
bool CMesh::IntersectXRay(CFacet* pFacet, double y, double z, Vec3D& p, double& pu, double& pv)
//---------------------------------------------------------------------------
{// compute intersection point P of triangle plane with ray from origin O in direction D
	// D assumed to be normalized
	// if no interstion, return false
	// u and v are barycentric coordinates of the intersection point P = (1 - u - v)A + uB + vC 
	// see http://www.devmaster.net/wiki/Ray-triangle_intersection
	Vec3D d = Vec3D(1,0,0);
	Vec3D o = Vec3D(-1e9, y, z);

	Vec3D a = Vertices[pFacet->vi[0]].v;
	Vec3D b = Vertices[pFacet->vi[1]].v;
	Vec3D c = Vertices[pFacet->vi[2]].v;
	
	//Vec3D n = pFacet->n; //((b-a).Cross(c-a)).Normalized();
	Vec3D n = ((b-a).Cross(c-a)).Normalized();
	//if (n.x > 0){ //flip vertices...
	//	Vec3D tmp = a;
	//	a = b;
	//	b = tmp;
	//	n = ((b-a).Cross(c-a)).Normalized();
	//}

	double dn = d.Dot(n);
	if (fabs(dn)<1E-5)
		return false; //parallel

	double dist = -(o-a).Dot(n)/dn;
	Vec3D tmp = d * dist;
	p = o + tmp;
	//	p = o+d*dist;

	double V1, V2, V3;
	V1 = (b-a).Cross(p-a).Dot(n);
	V2 = (c-b).Cross(p-b).Dot(n);
	V3 = (a-c).Cross(p-c).Dot(n);
	
	if (V1 >=0 && V2 >=0 && V2 >=0) return true;
	//if (V1 <=0 && V2 <=0 && V2 <=0) return true;
	else return false;

//	a = a.Dot(n); //project a in direction of normal...
//	b = b.Dot(n); //project a in direction of normal...
//	c = c.Dot(n); //project a in direction of normal...
//	p = p.Dot(n); //project a in direction of normal...
	
//	pu = (p.y*c.x-p.x*c.y)/(b.y*c.x-b.x*c.y);
//	pv = (p.y*b.x-p.x*b.y)/(c.y*b.x-c.x*b.y);

//	if (pu >= 0.0 && pv >= 0.0 && pu+pv <= 1.0) return true;
//	else return false;

	////copied from InTriangel()
	//Vec3D xax = (b-a).Normalized();
	//Vec3D zax = ((c-a).Cross(xax)).Normalized();
	//Vec3D yax = zax.Cross(xax).Normalized();

	//Vec3D p0(0,0,1);
	//Vec3D p1((b-a).Dot(xax),(b-a).Dot(yax),1);
	//Vec3D p2((c-a).Dot(xax),(c-a).Dot(yax),1);
	//Vec3D pt((p-a).Dot(xax),(p-a).Dot(yax),1);

	//double d0 = Det(p0,p1,pt);
	//double d1 = Det(p1,p2,pt);
	//double d2 = Det(p2,p0,pt);

	//if (d0<=0 && d1<=0 && d2<=0)
	//	return true;
	//if (d0>=0 && d1>=0 && d2>=0)
	//	return true;

	//return false;
}

void CMesh::WeldClose(float Distance)
{

	int* NumVertHere = new int[Vertices.size()]; //keeps track of how many vertices have been averaged to get here...
	int* ConsolidateMap = new int[Vertices.size()]; //maps the whole vertex list to the welded vertex list (IE has holes)
	int* OldNewMap = new int [Vertices.size()]; //maps the old, larger vertex list to the new, smaller one.
	for (int i=0; i<(int)Vertices.size(); i++){
		NumVertHere[i] = 1;
		ConsolidateMap[i] = i;
		OldNewMap[i] = -1;
	}

	for (int i=0; i<(int)Facets.size(); i++){ //look through facets so we don't have to do exhaustive On2 search of all vertex combos
		for (int j=0; j<3; j++){ //look at all three combinations of vertices...
			int Vi1 = Facets[i].vi[j];
			int np = -1; while (np != Vi1){ np = Vi1; Vi1 = ConsolidateMap[Vi1]; } //iterates NewMap to get the final value...

			int Vi2 = Facets[i].vi[(j+1)%3];
			np = -1; while (np != Vi2){ np = Vi2; Vi2 = ConsolidateMap[Vi2]; } //iterates NewMap to get the final value...

			if (Vi1 != Vi2 && (Vertices[Vi1].v-Vertices[Vi2].v).Length() < Distance){ //if they are close enough but not already the same...
			  Vec3D v1 = Vertices[Vi1].v*NumVertHere[Vi1];
			  Vec3D v2 = Vertices[Vi2].v*NumVertHere[Vi2];
			  Vertices[Vi1].v = (v1 + v2)/(NumVertHere[Vi1]+NumVertHere[Vi2]); //Vertex 1 is the weighted average
			//	Vertices[Vi1].v = (Vertices[Vi1].v*NumVertHere[Vi1] + Vertices[Vi2].v*NumVertHere[Vi2])/(NumVertHere[Vi1]+NumVertHere[Vi2]); //Vertex 1 is the weighted average
				NumVertHere[Vi1] = NumVertHere[Vi1] + NumVertHere[Vi2]; //count how many vertices make up this point now...
				
				ConsolidateMap[Vi2] = Vi1; //effectively deletes Vi2... (points to Vi1)
			}
		}
	}

	std::vector<CFacet> NewFacets;
	std::vector<CVertex> NewVertices;

	for (int i=0; i<(int)Vertices.size(); i++){
		if (ConsolidateMap[i] == i) { //if this vertex ended up being part of the welded part
			NewVertices.push_back(Vertices[i]); //add to the new vertex list
			OldNewMap[i] = NewVertices.size()-1;
		}
	}

	//update the vertex indices
	for (int i=0; i<(int)Facets.size(); i++){ //look through facets so we don't have to do exhaustive On2 search of all vertex combos
		for (int j=0; j<3; j++){ //look at all three combinations of vertices...
			int n = Facets[i].vi[j];
			int np = -1; while (np != n){ np = n; n = ConsolidateMap[n]; } //iterates NewMap to get the final value...

			Facets[i].vi[j] = OldNewMap[n];
		}
		if (!(Facets[i].vi[0] == Facets[i].vi[1] || Facets[i].vi[0] == Facets[i].vi[2] || Facets[i].vi[2] == Facets[i].vi[1])) //if there aren't any the same...
			NewFacets.push_back(Facets[i]);
	}

	Facets = NewFacets;
	Vertices = NewVertices;

	delete [] NumVertHere;
	NumVertHere = NULL;
	delete [] ConsolidateMap;
	ConsolidateMap = NULL;
	delete [] OldNewMap;
	OldNewMap = NULL;

	CalcVertNormals(); //re-calculate normals!

}


