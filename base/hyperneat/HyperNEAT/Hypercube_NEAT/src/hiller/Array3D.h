#ifndef ARRAY3D_H
#define ARRAY3D_H

#include "XML_Rip.h"
//#include <fstream>


class CArray3Df
{
public:
	CArray3Df(void);
	CArray3Df(int x, int y, int z);
	~CArray3Df(void);

	CArray3Df(const CArray3Df& rArray); //copy constructor
	CArray3Df& operator=(const CArray3Df& rArray); //overload "=" 
	const float& operator [](int i) const { return pData[i]; } //overload index operator
		  float& operator [](int i)       { return pData[i]; }
	
	float& operator ()(int i, int j, int k) { return pData[GetIndex(i, j, k)]; }


	void WriteXML(CXML_Rip* pXML);
	void ReadXML(CXML_Rip* pXML);

	void IniSpace(int x, int y, int z, float IniVal = 0.0f);
	void ResetSpace(float IniVal = 0.0f);
	void DeleteSpace();
	float GetMaxValue(); //returns the maximum value anywhere in the array.
	int GetIndex(int x, int y, int z); //returns the index  or -1 if invalid
	bool GetXYZ(int* x, int* y, int* z, int Index); //returns the xyz indicies
	int GetFullSize(void) {return FullSize;};
	int GetXSize(void) {return XSize;};
	int GetYSize(void) {return YSize;};
	int GetZSize(void) {return ZSize;};



protected:
	float* pData; 
	int XSize;
	int YSize;
	int ZSize;
	int FullSize;

};

#endif
