#ifndef __HEX_CELL_GROUP_H__
#define __HEX_CELL_GROUP_H__

#include "cocos2d.h"

USING_NS_CC;

class HexCellGroup : public Node
{
public:
	HexCellGroup();
	~HexCellGroup();

	static HexCellGroup* create(std::vector<Vec2> cells)
	{ 
	HexCellGroup *pRet = new(std::nothrow) HexCellGroup(); 
	if (pRet && pRet->initWithLocs(cells))
	{ 
	pRet->autorelease(); 
	return pRet; 
	} 
	else 
	{ 
	delete pRet; 
	pRet = NULL; 
	return NULL; 
	} 
	}

	bool initWithLocs(std::vector<Vec2> cells);
	Vec2 getGroupCenterOffset();
	void setAllOpacity(int op);

	std::vector<Vec2> _current_cells_pos;
	std::vector<Sprite*> _current_cells;

private:

};

#endif