/*
 *  HCUBE_NodeAvatar.h
 *  v2Hyperneatnick
 *
 *  Created by Admin on 7/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include "HCUBE_Defines.h"
#include "NEAT_Defines.h"

#ifndef HCUBE_NodeAvatar_H
#define HCUBE_NodeAvatar_H
namespace HCUBE
{
	class LinkAvatar;
	class NodePattern;
}

namespace HCUBE
{
class NodeAvatar
	{
	public:
		NodeAvatar(NEAT::GeneticNodeGene* n);
		
		void setCoords(const wxSize& s);
		void setCoords(int x, int y);
		int getCoordsX() {return xcenter;}
		int getCoordsY() {return ycenter;}
		
		//void setDC(wxDC* dc) {drawcontext = dc}
		
		inline void setNormalColor(const wxColour& col) { normalcolor.Set(col.GetAsString());}
		inline void setSelectedColor(const wxColour& col) {selectedcolor.Set(col.GetAsString());}
		
		
		inline string getType(){return node->getType();}
		inline string getName(){return node->getName();}
		inline int getActivationFunction(){return node->getActivationFunction();}
		inline int getSize(){return size;}
		inline int getID(){return node->getID();}
		
		inline void setActivationFunction(ActivationFunction f){node->setActivationFunction(f);}
		
		inline void addLink(LinkAvatar* l){mylinks.push_back(l);}
		
		bool isSelected(int x, int y);
		
		void draw(wxDC* dc);
		
		
	private:
		
		inline void setActiveColor(const wxColour& col) 
		{
			if(col.GetAsString() != activecolor.GetAsString())
			{
				activecolor.Set(col.GetAsString());
			}
		}
		
		NEAT::GeneticNodeGene* node;
		std::vector<LinkAvatar*> mylinks;
		wxColour activecolor, normalcolor, selectedcolor;
		int xmin,xmax,ymin,ymax,xcenter,ycenter;
		int size;
		//wxDC* drawcontext;
	};

class LinkAvatar
	{
	public:
		//LinkAvatar(NEAT::GeneticLinkGene* l);
		LinkAvatar(NEAT::GeneticLinkGene* l, NodeAvatar* s, NodeAvatar* d):link(l),nsource(s),ndest(d)
		{
			//link->setWeight(0);
			activecolor.Set(wxBLACK->GetAsString());
		}
		LinkAvatar(NEAT::GeneticLinkGene* l, NodeAvatar* s, NodeAvatar* d, double w):link(l),nsource(s),ndest(d)
		{
			//link->setWeight(w);
			activecolor.Set(wxBLACK->GetAsString());
		}
		
		int getID(){return link->getID();}
		
		double getWeight() {return link->getWeight();}
		void setWeight(double w) {link->setWeight(w);}
		
		wxColour getColour() {return activecolor;}
		void setColour(const wxColour& c) {activecolor.Set(c.GetAsString());}
		
		bool isVisible() {return visible;}
		void isVisible(bool f) {visible = f;}
		
		void draw(wxDC* dc);
		
	private:
		bool visible;
		NEAT::GeneticLinkGene* link;
		wxColour activecolor;
		NodeAvatar* nsource;
		NodeAvatar* ndest;
	};
	
	

class IndividualAvatar
	{
	public:
		
		//IndividualAvatar(NEAT::GeneticIndividual* ind, NodePattern* pat, wxDC* dc);
		IndividualAvatar(shared_ptr<NEAT::GeneticIndividual> ind, NodePattern* pat);
		IndividualAvatar(shared_ptr<NEAT::GeneticIndividual> ind);
		IndividualAvatar();
		
		NodeAvatar* getNodeByID(int passid);
		NodeAvatar* getNodeByCoords(int xcoord, int ycoord);
		
		void setPattern(NodePattern* pat) {_pat = pat;}
		void setIndividual(shared_ptr<const NEAT::GeneticIndividual> ind) {_ind = ind;}
		
		//void setDC(wxDC* dc) {_dc = dc;}
		
		void draw(NodePattern*, wxDC*);
		//void draw(NodePattern*);
		void draw(wxDC*);
		//void draw();
			
			
	protected:
		
		//std::vector<NodeAvatar*>::iterator _iternodelist;
		std::vector<NodeAvatar*> _nodelist;
		
		//std::vector<LinkAvatar*>::iterator _iterlinklist
		std::vector<LinkAvatar*>_linklist;
		//wxDC* _dc;
		shared_ptr<const NEAT::GeneticIndividual> _ind;
		NodePattern* _pat;
	
	private:
		void _construct(shared_ptr<NEAT::GeneticIndividual> ind);
			
	};
	

class NodePattern
	{
	public:
		NodePattern() {}
		
		//pass a vector of NodeAvatars, tell it the size of the draw context, and the coordinates are updated!
		virtual void getCoords(std::vector<NodeAvatar*> l, const wxSize& s) = 0;
	
	protected:
		//vector< pair<double,int> > positionHistogram;
	};

class LinearPattern: public NodePattern
	{
	public:
		void getCoords(std::vector<NodeAvatar*> l , const wxSize& s)
		{
			int sizex = s.GetWidth();
			int sizey = s.GetHeight();
			int edgepadding = sizey/l.size();
			int incount = 0;
			int hiddencount = 0;
			int outcount = 0;
			int xcoord, ycoord;
			
			for(int i = 0; i<l.size(); ++i)
			{
				if (l[i]->getType() == "NetworkSensor")
				{
					incount++;
					xcoord = edgepadding*incount;
					ycoord = sizey-edgepadding;
					
				}
				
				else if (l[i]->getType() == "HiddenNode")
				{
					hiddencount++;
					xcoord = edgepadding*hiddencount;
					ycoord = sizey/2;
				}
				
				else if (l[i]->getType() == "NetworkOutputNode")
				{
					outcount++;
					xcoord = edgepadding*outcount;
					ycoord = edgepadding;
				}
				else
				{
					throw CREATE_LOCATEDEXCEPTION_INFO("Invalid Node");
				}
				
				l[i]->setCoords(xcoord,ycoord);
			}
				
		}
	};

class CircularPattern: public NodePattern
	{
	public:
		void getCoords(std::vector<NodeAvatar*> l , const wxSize& s)
		{
			//implement getCoords for linear pattern
		}
	};
}

#endif