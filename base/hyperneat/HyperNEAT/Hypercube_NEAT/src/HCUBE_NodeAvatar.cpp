/*
 *  HCUBE_NodeAvatar.cpp
 *  v2Hyperneatnick
 *
 *  Created by Admin on 7/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "HCUBE_NodeAvatar.h"

namespace HCUBE

{
//NodeAvatar Definitions
	
	NodeAvatar::NodeAvatar(NEAT::GeneticNodeGene* n):node(n)
	{
		size = 10;
		normalcolor.Set(wxBLACK->GetAsString());
		selectedcolor.Set(wxRED->GetAsString());
		activecolor = normalcolor;
	}
	
	void NodeAvatar::setCoords(const wxSize& s)
	{
		xcenter = s.GetWidth();
		ycenter = s.GetHeight();
		xmin = xcenter-(size/2);
		xmax = xcenter+(size/2);
		ymin = ycenter-(size/2);
		ymax = ycenter+(size/2);
	}
	
	void NodeAvatar::setCoords(int x, int y)
	{
		xcenter = x;
		ycenter = y;
		xmin = x-(size/2);
		xmax = x+(size/2);
		ymin = y-(size/2);
		ymax = y+(size/2);
	}
	
	bool NodeAvatar::isSelected(int x, int y)
	{
		if (x < xmin) return false;
		if (x > xmax) return false;
		if (y < ymin) return false;
		if (y > ymax) return false;
		
		activecolor = selectedcolor;
		
		
		return true;
	}
	
	
	void NodeAvatar::draw(wxDC* dc)
	{
		if(dc->GetPen().GetColour().GetAsString() != activecolor.GetAsString())
		{
			wxPen oldpen = dc->GetPen();
			wxPen newpen(activecolor);
			dc->SetPen(newpen);
			
			dc->DrawRectangle(xmin,ymin,size,size);
			
			dc->SetPen(oldpen);
			//delete newpen;
		}
		else
		{
			dc->DrawRectangle(xmin,ymin,size,size);
		}
	}

//LinkAvatar Definitions
	void LinkAvatar::draw(wxDC* dc)
	{
		if(dc->GetPen().GetColour().GetAsString() != activecolor.GetAsString())
		{
			wxPen oldpen = dc->GetPen();
			wxPen newpen(activecolor);
			dc->SetPen(newpen);
			
			dc->DrawLine(nsource->getCoordsX(),nsource->getCoordsY(),ndest->getCoordsX(),ndest->getCoordsY());
			
			dc->SetPen(oldpen);
			//delete newpen;
		}
		else
		{
			dc->DrawLine(nsource->getCoordsX(),nsource->getCoordsY(),ndest->getCoordsX(),ndest->getCoordsY());
		}
	}
	
	
//IndividualAvatar Definitions
		
	IndividualAvatar::IndividualAvatar(shared_ptr<NEAT::GeneticIndividual> ind, NodePattern* pat): _pat(pat)
	{
		this->_construct(ind);
	}
	
	IndividualAvatar::IndividualAvatar(shared_ptr<NEAT::GeneticIndividual> ind)
	{
		
		this->_construct(ind);
	}
	
	void IndividualAvatar::_construct(shared_ptr<NEAT::GeneticIndividual> ind)
	{
		for (int a=0;a<ind->getNodesCount();++a)
		{
			//NEAT::GeneticNodeGene *node = ind->getNode(a);
			
			_nodelist.push_back(new NodeAvatar(ind->getNode(a)));
		}
		
		for(int a=0;a<ind->getLinksCount();++a)
		{
			NEAT::GeneticLinkGene* tmp = ind->getLink(a);
			NodeAvatar* tmpsrc = this->getNodeByID(tmp->getFromNodeID());
			NodeAvatar* tmpdst = this->getNodeByID(tmp->getToNodeID());
			
			LinkAvatar* tmplk = new LinkAvatar(tmp,tmpsrc,tmpdst);
			
			tmpsrc->addLink(tmplk);
			tmpdst->addLink(tmplk);
			
			_linklist.push_back(tmplk);
		}
	}
	
	
	NodeAvatar* IndividualAvatar::getNodeByID(int passid)
	{
		
		for(int a=0;a<(int)_nodelist.size();++a)
		{
			if(_nodelist[a]->getID()==passid)
			{
				return _nodelist[a];
			}
		}
	
		return NULL;
	}
	
	NodeAvatar* IndividualAvatar::getNodeByCoords(int xcoord, int ycoord)
	{
		for(int a=0;a<(int)_nodelist.size();++a)
		{
			if(_nodelist[a]->isSelected(xcoord,ycoord))
			{
				return _nodelist[a];
			}
		}
		
		return NULL;
	}
	
	/*
	void IndividualAvatar::draw()
	{
		if (_pat != NULL && _dc->isOK())
			{
			_pat->getCoords(_nodelist, _dc->GetSize());
			
			for(int a=0;a<(int)_nodelist.size();++a)
			{
				_nodelist[a]->draw(_dc)
			}
				
			for(_iterlist = _nodelist.begin(); _iterlist != _nodelist.end(); ++_iterlist)
			{
				(*_iterlist)->draw(_dc);
			}
				
			
		}
	}
 
	
	void IndividualAvatar::draw(NodePattern* pat)
	{
		if (pat != NULL && _dc->isOK())
		{
			pat->getCoords(_list, _dc->GetSize());
			
			for(_iterlist = _list.begin(); _iterlist != _list.end(); ++_iterlist)
			{
				(*_iterlist)->draw(_dc);
			}
		}
	}
 */
	
	void IndividualAvatar::draw(wxDC* dc)
	{
		if (_pat != NULL && dc->Ok())
		{
			_pat->getCoords(_nodelist, dc->GetSize());
			
			for(int a=0;a<(int)_nodelist.size();++a)
			{
				_nodelist[a]->draw(dc);
			}
			
			for(int a=0; a<(int)_linklist.size();++a)
			{
				_linklist[a]->draw(dc);
			}
		}
	}
	
	void IndividualAvatar::draw(NodePattern* pat, wxDC* dc)
	{
		if (pat != NULL && dc->Ok())
		{
			pat->getCoords(_nodelist, dc->GetSize());
			
			for(int a=0;a<(int)_nodelist.size();++a)
			{
				_nodelist[a]->draw(dc);
			}
			
			for(int a=0; a<(int)_linklist.size();++a)
			{
				_linklist[a]->draw(dc);
			}
		}
	}
	
}
	
	
	
	

