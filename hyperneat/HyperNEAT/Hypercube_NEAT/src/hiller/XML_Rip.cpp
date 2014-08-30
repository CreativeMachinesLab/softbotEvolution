#include "XML_Rip.h"


CXML_Rip::CXML_Rip(void)
{
	TiXmlDeclaration* declaration = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild(declaration);

	LastTagSearched = "";
}

CXML_Rip::~CXML_Rip(void)
{
}

void CXML_Rip::SaveFile(std::string filename)
{
	doc.SaveFile(filename);
}

bool CXML_Rip::LoadFile(std::string filename) 
{
	if (!doc.LoadFile(filename)) return false;

	ElStack.clear();
	ElStack.push_back(doc.RootElement()); //start with the root element!
	StrStack.clear();
	StrStack.push_back("Root");
	return true;
}

void CXML_Rip::DownLevel(std::string tag)
{
	int ElStackSize = (int)ElStack.size();
	
	TiXmlElement * tmpEl = new TiXmlElement(tag);
	if (ElStackSize == 0) //if root level, add to doc
		doc.LinkEndChild(tmpEl);
	else //if not root level, add to current level of stack
		ElStack[ElStackSize-1]->LinkEndChild(tmpEl);

	ElStack.push_back(tmpEl);
	StrStack.push_back(tag); //not relly necessary, but keep track of it anyway...
}

void CXML_Rip::UpLevel(void)
{
	ElStack.pop_back();
	StrStack.pop_back();
}

void CXML_Rip::Element(std::string tag, std::string data, bool CDATA)
{
	DownLevel(tag);
	
	TiXmlText * tmpText = new TiXmlText(data.c_str());
	if (CDATA) tmpText->SetCDATA(true);
	ElStack[ElStack.size()-1]->LinkEndChild(tmpText);

	UpLevel();
}

void CXML_Rip::Element(std::string tag, float data)
{
	std::ostringstream tmp;
	tmp << data;
	Element(tag, tmp.str());
}

void CXML_Rip::Element(std::string tag, double data)
{
	std::ostringstream tmp;
	tmp << data;
	Element(tag, tmp.str());
}

void CXML_Rip::Element(std::string tag, int data)
{
	std::ostringstream tmp;
	tmp << data;
	Element(tag, tmp.str());
}


bool CXML_Rip::FindElement(std::string tag) //finds element if it exists and appends ptr to stack. if called subsequently with the same tag, looks for siblings, not children.
{
	TiXmlElement* StartElement;
	TiXmlElement* IterElement;
	bool IsSameTag = (tag == StrStack.back());
	//LastTagSearched = tag; //keep track of this query


	if (IsSameTag) //if this IS the last tag we searched for...
		StartElement = (TiXmlElement*) ElStack.back()->NextSibling();
	else //if this is the first time we've searched for the tag
		StartElement = (TiXmlElement*) ElStack.back()->FirstChild();

	for (IterElement = StartElement; IterElement != 0; IterElement = (TiXmlElement*)IterElement->NextSibling()){
		if (tag == IterElement->Value()){
			if (IsSameTag) ElStack.back() = IterElement; //move the bottom element of the stack to next sibling
			else {
				ElStack.push_back(IterElement); //if first element of this type
				StrStack.push_back(tag);
			}
			return true; 
		}
	}

	if (IsSameTag) UpLevel(); //idea: if've we've been searching for multiple tags and we reach the end, automatically go back up a level

	return false;
}

bool CXML_Rip::FindLoadElement(std::string tag, std::string* pString, bool StayDown)
{
	if (!FindElement(tag)) return false;
	TiXmlText* pText = ElStack.back()->FirstChild()->ToText();
	if (pText == 0) return false;
	
	*pString = pText->Value();
	
	if (!StayDown) UpLevel(); //if we're not expecting to load more tags of the same name...

	return true;
}

bool CXML_Rip::FindLoadElement(std::string tag, double* pDouble, bool StayDown)
{
	std::string tmp;
	if (FindLoadElement(tag, &tmp, StayDown)){
		*pDouble = atof(tmp.c_str()); //returns a double
		return true;
	}
	return false;
}

bool CXML_Rip::FindLoadElement(std::string tag, float* pFloat, bool StayDown)
{
	std::string tmp;
	if (FindLoadElement(tag, &tmp, StayDown)){
		*pFloat = (float)atof(tmp.c_str());
		return true;
	}
	return false;
}

bool CXML_Rip::FindLoadElement(std::string tag, int* pInt, bool StayDown)
{
	std::string tmp;
	if (FindLoadElement(tag, &tmp, StayDown)){
		*pInt = atoi(tmp.c_str());
		return true;
	}
	return false;
}

bool CXML_Rip::FindLoadElement(std::string tag, bool* pBool, bool StayDown)
{
	int tmpInt;
	if (!FindLoadElement(tag, &tmpInt, StayDown)) return false;
	
	if (tmpInt == 0) *pBool = false;
	else *pBool = true;

	return true;
}

