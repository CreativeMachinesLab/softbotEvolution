#pragma once
#include "tinyxml.h"
#include <vector>


//for quick run-through xml encoding
class CXML_Rip
{
public:
	CXML_Rip(void);
	~CXML_Rip(void);

	//for saving
	TiXmlDocument doc;
	std::vector <TiXmlElement*> ElStack;
	std::vector <std::string> StrStack; //used on loading to keep track of which tags were last looked for

	void SaveFile(std::string filename);
	bool LoadFile(std::string filename);
	
	void DownLevel(std::string tag);
	void UpLevel(void);
	void Element(std::string tag, std::string data, bool CDATA = false);
	void Element(std::string tag, double data);
	void Element(std::string tag, float data);
	void Element(std::string tag, int data);

	void SetAttribute(std::string Att, std::string Value) {ElStack.back()->SetAttribute(Att, Value);}; //adds an attribute to the current element in the stack
	void SetAttribute(std::string Att, double Value) {std::ostringstream tmpString; tmpString << Value; SetAttribute(Att, tmpString.str());}; //adds an attribute to the current element in the stack
	void SetAttribute(std::string Att, float Value) {std::ostringstream tmpString; tmpString << Value; SetAttribute(Att, tmpString.str());};//adds an attribute to the current element in the stack
	void SetAttribute(std::string Att, int Value) {ElStack.back()->SetAttribute(Att, Value);};  //adds an attribute to the current element in the stack

	//for loading
	std::string LastTagSearched; //keep track of which tag we last looks for. if new query same as last, look for siblings.
	//caution... this function can either go down a level or not!
	bool FindElement(std::string tag); //finds element if it exists and appends ptr to stack. if called subsequently with the same tag, looks for siblings, not children.

	//	bool FindDownLevel(std::string tag); //finds a tag at the current level of the ElStack, enters it!
//	bool FindNextSame(); //Iterates bottom element on the stack to the next tag with the same name...
	//	 Uplevel(void) //same function here...

	//these functions do not ever change the level of the stack.
	bool FindLoadElement(std::string tag, std::string* pString, bool StayDown = false); 
	bool FindLoadElement(std::string tag, double* pDouble, bool StayDown = false);
	bool FindLoadElement(std::string tag, float* pFloat, bool StayDown = false);
	bool FindLoadElement(std::string tag, int* pInt, bool StayDown = false);
	bool FindLoadElement(std::string tag, bool* pBool, bool StayDown = false);

	void GetAttribute(std::string Att, std::string* pString) {*pString = ElStack.back()->Attribute(Att.c_str());};
	void GetAttribute(std::string Att, double* pDouble) {*pDouble = atof(ElStack.back()->Attribute(Att.c_str()));};
	void GetAttribute(std::string Att, float* pFloat) {*pFloat = (float)atof(ElStack.back()->Attribute(Att.c_str()));};
	void GetAttribute(std::string Att, int* pInt) {*pInt = atoi(ElStack.back()->Attribute(Att.c_str()));};
};
