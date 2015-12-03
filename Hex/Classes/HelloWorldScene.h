#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#define HEX_RID  4
#define HEX_CELL_SIZE  64

#include "cocos2d.h"
#include <math.h>

class HexCellGroup;

using namespace cocos2d;
using namespace ui;

const std::vector<Vec2> cg_1 = { Vec2(1, 1) };
const std::vector<Vec2> cg_2 = { Vec2(1, 1), Vec2(1, 2), Vec2(2, 2), Vec2(2, 3) };
const std::vector<Vec2> cg_3 = { Vec2(1, 1), Vec2(1, 2), Vec2(2, 1), Vec2(2, 2) };
const std::vector<Vec2> cg_4 = { Vec2(1, 1), Vec2(2, 1), Vec2(2, 2), Vec2(3, 2) };
const std::vector<Vec2> cg_5 = { Vec2(1, 1), Vec2(1, 2), Vec2(1, 3), Vec2(2, 3) };
const std::vector<Vec2> cg_6 = { Vec2(1, 1), Vec2(1, 2), Vec2(1, 3), Vec2(2, 2) };
const std::vector<Vec2> cg_7 = { Vec2(1, 1), Vec2(2, 1), Vec2(2, 2), Vec2(2, 3) };
const std::vector<Vec2> cg_8 = { Vec2(1, 1), Vec2(2, 0), Vec2(2, 1), Vec2(2, 2) };
const std::vector<Vec2> cg_9 = { Vec2(1, 1), Vec2(2, 1), Vec2(2, 2), Vec2(3, 1) };
const std::vector<Vec2> cg_10 = { Vec2(1, 1), Vec2(2, 1), Vec2(3, 1), Vec2(3, 2) };
const std::vector<Vec2> cg_11 = { Vec2(1, 1), Vec2(2, 0), Vec2(2, 1), Vec2(3, 1) };
const std::vector<Vec2> cg_12 = { Vec2(1, 1), Vec2(1, 2), Vec2(2, 2), Vec2(3, 2) };
const std::vector<Vec2> cg_13 = { Vec2(1, 1), Vec2(2, 1), Vec2(2, 2), Vec2(3, 3) };
const std::vector<Vec2> cg_14 = { Vec2(1, 1), Vec2(2, 2), Vec2(3, 2), Vec2(3, 3) };
const std::vector<Vec2> cg_15 = { Vec2(1, 1), Vec2(1, 2), Vec2(2, 2), Vec2(3, 3) };
const std::vector<Vec2> cg_16 = { Vec2(1, 1), Vec2(2, 2), Vec2(2, 3), Vec2(3, 3) };
const std::vector<Vec2> cg_17 = { Vec2(1, 1), Vec2(2, 1), Vec2(3, 2), Vec2(3, 3) };
const std::vector<Vec2> cg_18 = { Vec2(1, 1), Vec2(1, 3), Vec2(2, 2), Vec2(2, 3) };
const std::vector<Vec2> cg_19 = { Vec2(1, 1), Vec2(2, 2), Vec2(3, 1), Vec2(3, 2) };
const std::vector<Vec2> cg_20 = { Vec2(1, 1), Vec2(1, 2), Vec2(2, 1), Vec2(2, 3) };
const std::vector<Vec2> cg_21 = { Vec2(1, 1), Vec2(1, 2), Vec2(2, 3), Vec2(3, 3) };
const std::vector<Vec2> cg_22 = { Vec2(1, 1), Vec2(2, 2), Vec2(3, 1), Vec2(3, 2) };
const std::vector<Vec2> cg_23 = { Vec2(1, 1), Vec2(2, 1), Vec2(3, 1), Vec2(4, 1) };
const std::vector<Vec2> cg_24 = { Vec2(1, 1), Vec2(2, 2), Vec2(3, 3), Vec2(4, 4) };
const std::vector<Vec2> cg_25 = { Vec2(1, 1), Vec2(1, 2), Vec2(1, 3), Vec2(1, 4) };
const std::vector<std::vector<Vec2>> cg_list = { cg_1, cg_2, cg_3, cg_4, cg_5, cg_6, cg_7, cg_8, cg_9, cg_10, cg_11, cg_12, cg_13, cg_14, cg_15, cg_16, cg_17, cg_18, cg_19, cg_20, cg_21, cg_22, cg_23, cg_24, cg_25};

struct HexCellSlot
{
public:
	int x_index = 0;
	int y_index = 0;
	bool isEnable = false;
	bool isSet = false;
	Sprite* cellInBG = nullptr;
};

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	bool setGroup(HexCellGroup* cellgroup);
	Vec2	findCellLocation(HexCellGroup* cellgroup);
	void showHintInStage(HexCellGroup* cell_group, Vec2 loc);
	bool checkCanSet(HexCellGroup* cell_group, Vec2 loc);
	void resetCellGroup();
	bool checkCanContinue();
	void checkAndClearLines();
	void resetGame();

	Vec2 cell_origin;
	std::vector< std::vector<HexCellSlot*> >  hex_vec;
	HexCellSlot* current_hold_slot;
	HexCellGroup* current_hec_cell_group;
	int current_hc_type;
	Vec2 current_hc_from;
	Vec2 cell_loc_now;
	HexCellGroup* hec_cell_group_1;
	HexCellGroup* hec_cell_group_2;
	HexCellGroup* hec_cell_group_3;
    Label* current_score_label;
    int current_score;
};


#endif // __HELLOWORLD_SCENE_H__
