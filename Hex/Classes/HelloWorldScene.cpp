#include "HelloWorldScene.h"
#include "HexCellGroup.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	cell_origin = Vec2(visibleSize.width*0.4, visibleSize.height*0.45);

//     ///////////////////////////
        // 2. add a menu item with "X" image, which is clicked to quit the program
        //    you may modify it.
    
        // add a "close" icon to exit the progress. it's an autorelease object
        auto closeItem = MenuItemImage::create(
                                               "CloseNormal.png",
                                               "CloseSelected.png",
                                               CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
        
    	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                    origin.y + closeItem->getContentSize().height/2));
    
        // create menu, it's an autorelease object
        auto menu = Menu::create(closeItem, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);
    
    current_score_label = Label::create();
    current_score_label->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.9));
    current_score_label->setSystemFontSize(40);
    this->addChild(current_score_label);
    current_score = 0;
    current_score_label->setString(String::createWithFormat("score: %d", current_score)->_string);
		current_hold_slot = nullptr;
		current_hec_cell_group = nullptr;
		cell_loc_now = Vec2::ZERO;
		hec_cell_group_1 = nullptr;
		hec_cell_group_2 = nullptr;
		hec_cell_group_3 = nullptr;

    
	for (int i = 0; i < HEX_RID * 2 + 1; i++){
		std::vector<HexCellSlot*> tempVec;
		for (int j = 0; j < HEX_RID * 2 + 1; j++){
			auto _cell = new HexCellSlot();
			_cell->x_index = j;
			_cell->y_index = i;
			_cell->isSet = false;
			if (abs(i - j) > HEX_RID){
				_cell->isEnable = false;
			}
			else{
				_cell->isEnable = true;
			};
			_cell->cellInBG = Sprite::create("test.png");
			tempVec.push_back(_cell);

			//float fixed_cell_w = HEX_CELL_SIZE * sin(60 * M_PI/180);
			float fixed_cell_w = HEX_CELL_SIZE * sqrt(0.75);
			float fixed_cell_h = HEX_CELL_SIZE * 0.75;
			float fixedPos_x = cell_origin.x + fixed_cell_w * (j - HEX_RID - 1) + (fixed_cell_w / 2.0) * (HEX_RID - i + 1);
			float fixedPos_y = cell_origin.y + fixed_cell_h * (HEX_RID - i);
			_cell->cellInBG->setPosition(fixedPos_x, fixedPos_y);
			this->addChild(_cell->cellInBG);

			if (!_cell->isEnable){
				_cell->cellInBG->setVisible(false);
			};
			
		}
		hex_vec.push_back(tempVec);
	}

 	auto listener = EventListenerTouchOneByOne::create();
 	listener->setSwallowTouches(true);
 	listener->onTouchBegan = [=](Touch* touch, Event* event){
		if (touch->getLocation().distance(hec_cell_group_1->getPosition()) < HEX_CELL_SIZE){
			current_hec_cell_group = hec_cell_group_1;
			current_hc_type = 1;
			current_hc_from = hec_cell_group_1->getPosition();
		}
		else if (touch->getLocation().distance(hec_cell_group_2->getPosition()) < HEX_CELL_SIZE)
		{
			current_hec_cell_group = hec_cell_group_2;
			current_hc_type = 2;
			current_hc_from = hec_cell_group_2->getPosition();
		}
		else if (touch->getLocation().distance(hec_cell_group_3->getPosition()) < HEX_CELL_SIZE)
		{
			current_hec_cell_group = hec_cell_group_3;
			current_hc_type = 3;
			current_hc_from = hec_cell_group_3->getPosition();
		}
		else return false;
		if (current_hec_cell_group != nullptr){
			current_hec_cell_group->setScale(1);
			current_hec_cell_group->setPosition(touch->getLocation());
		}
		return true;
 	};
	listener->onTouchMoved = [=](Touch* touch, Event* event){
		if (current_hec_cell_group != nullptr){
			current_hec_cell_group->setPosition(touch->getLocation());
			cell_loc_now = this->findCellLocation(current_hec_cell_group);
			this->showHintInStage(current_hec_cell_group, cell_loc_now);
		}
 	};
	listener->onTouchEnded = [=](Touch* touch, Event* event){
		if (setGroup(current_hec_cell_group)){
			current_hold_slot = nullptr;
			cell_loc_now = Vec2::ZERO;
			current_hec_cell_group == nullptr;
			current_hc_type = 0;
			current_hc_from = Vec2::ZERO;
			checkAndClearLines();
			resetCellGroup();
		}
 	};
	listener->onTouchCancelled = [](Touch* touch, Event* event){
 	};
 	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	resetCellGroup();
    return true;
}

bool HelloWorld::setGroup(HexCellGroup* cellgroup){
	if (this->checkCanSet(cellgroup, cell_loc_now))
	{
		for (int i = 0; i < cellgroup->_current_cells_pos.size(); i++)
		{
			Vec2 other_loc = cell_loc_now + Vec2(cellgroup->_current_cells_pos.at(i).y, cellgroup->_current_cells_pos.at(i).x) - Vec2::ONE;
			hex_vec.at(other_loc.y - 1).at(other_loc.x - 1)->isSet = true;
			hex_vec.at(other_loc.y - 1).at(other_loc.x - 1)->cellInBG->initWithFile("hex_cell.png");
		}
		cellgroup->removeFromParentAndCleanup(true);
		switch (current_hc_type)
		{
		case 1:
			hec_cell_group_1 = nullptr;
			break;
		case 2:
			hec_cell_group_2 = nullptr;
			break;
		case 3:
			hec_cell_group_3 = nullptr;
			break;
		default:
			break;
		}
		return true;
	}
	else
	{
		cellgroup->setScale(0.8);
		cellgroup->runAction(MoveTo::create(0.1, current_hc_from));
		current_hc_type = 0;
		current_hc_from = Vec2::ZERO;
		return false;
	}
	
}

Vec2 HelloWorld::findCellLocation(HexCellGroup* cellgroup){
	float fixed_cell_w = HEX_CELL_SIZE * sqrt(0.75);
	float fixed_cell_h = HEX_CELL_SIZE * 0.75;
	if (cell_origin.distance(cellgroup->getPosition()) > fixed_cell_w * (HEX_RID+1))
	{
		return Vec2::ZERO;
	}
	int _line = -1;
	for (int i = 0; i < hex_vec.size(); i++	)
	{
		if (abs((cellgroup->_current_cells.at(0)->getPosition() + cellgroup->getPosition()).y - hex_vec.at(i).at(0)->cellInBG->getPositionY()) < fixed_cell_h / 2){
			_line = i+1 ;
			break;
		}
	}
	if (_line < 0)
	{
		return Vec2::ZERO;
	}
	int _row = -1;
	for (int i = 0; i < hex_vec.at(_line-1).size(); i++){
		if (abs((cellgroup->_current_cells.at(0)->getPosition() + cellgroup->getPosition()).x - hex_vec.at(_line-1).at(i)->cellInBG->getPositionX()) < fixed_cell_w / 2){
			_row = i+1 ;
			break;
		}
	}
	if (_row<0)
	{
		return Vec2::ZERO;
	}
	if (!hex_vec.at(_line-1).at(_row-1)->isEnable || hex_vec.at(_line-1).at(_row-1)->isSet)
	{
		return Vec2::ZERO;
	}
	return Vec2(_row, _line);
}


void HelloWorld::showHintInStage(HexCellGroup* cell_group, Vec2 loc){
	if (current_hold_slot != nullptr)
	{
		current_hold_slot->cellInBG->setOpacity(255);
		for (int i = 1; i < cell_group->_current_cells_pos.size(); i++)
		{
			Vec2 other_loc = Vec2(current_hold_slot->x_index + 1, current_hold_slot->y_index + 1) + Vec2(cell_group->_current_cells_pos.at(i).y, cell_group->_current_cells_pos.at(i).x) - Vec2::ONE;
			if (other_loc.x > HEX_RID * 2 + 1 || other_loc.y > HEX_RID * 2 + 1)
			{
				break;
			}
			hex_vec.at(other_loc.y - 1).at(other_loc.x - 1)->cellInBG->setOpacity(255);
		}
		current_hold_slot = nullptr;
	}
	if (loc != Vec2::ZERO && checkCanSet(cell_group, loc))
	{
		current_hold_slot = hex_vec.at(loc.y-1).at(loc.x-1);
		current_hold_slot->cellInBG->setOpacity(100);
		for (int i = 1; i < cell_group->_current_cells_pos.size(); i++)
		{
			Vec2 other_loc = loc + Vec2(cell_group->_current_cells_pos.at(i).y, cell_group->_current_cells_pos.at(i).x) - Vec2::ONE;
			if (other_loc.x > HEX_RID * 2+1 || other_loc.y > HEX_RID * 2+1)
			{
				break;
			}
			hex_vec.at(other_loc.y - 1).at(other_loc.x - 1)->cellInBG->setOpacity(100);
		}
	}
}

bool HelloWorld::checkCanSet(HexCellGroup* cell_group, Vec2 loc){
	if (loc == Vec2::ZERO)
	{
		return false;
	}
	for (int i = 0; i < cell_group->_current_cells_pos.size(); i++)
	{
		Vec2 other_loc = loc + Vec2(cell_group->_current_cells_pos.at(i).y, cell_group->_current_cells_pos.at(i).x) - Vec2::ONE;
		if (other_loc.x <=0 || other_loc.y<=0 || other_loc.x > HEX_RID * 2 + 1 || other_loc.y > HEX_RID * 2 + 1 || abs(other_loc.x - other_loc.y) > HEX_RID)
		{
			return false;
		}
		if (hex_vec.at(other_loc.y - 1).at(other_loc.x - 1)->isSet == true)
		{
			return false;
		}
	}
	return true;
}

void HelloWorld::resetCellGroup(){
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (hec_cell_group_1 == nullptr)
	{
		hec_cell_group_1 = HexCellGroup::create(cg_list.at(random(0, 24)));
		hec_cell_group_1->setPosition(visibleSize.width*0.8, visibleSize.height*0.75);
		hec_cell_group_1->setScale(0);
		this->addChild(hec_cell_group_1);
		hec_cell_group_1->runAction(ScaleTo::create(0.2, 0.8));
	}
	if (hec_cell_group_2 == nullptr)
	{
		hec_cell_group_2 = HexCellGroup::create(cg_list.at(random(0, 24)));
		hec_cell_group_2->setPosition(visibleSize.width*0.8, visibleSize.height*0.5);
		hec_cell_group_2->setScale(0);
		this->addChild(hec_cell_group_2);
		hec_cell_group_2->runAction(ScaleTo::create(0.2, 0.8));
	}
	if (hec_cell_group_3 == nullptr)
	{
		hec_cell_group_3 = HexCellGroup::create(cg_list.at(random(0, 24)));
		hec_cell_group_3->setPosition(visibleSize.width*0.8, visibleSize.height*0.25);
		hec_cell_group_3->setScale(0);
		this->addChild(hec_cell_group_3);
		hec_cell_group_3->runAction(ScaleTo::create(0.2, 0.8));
	}
	if (checkCanContinue())
	{
		CCLOG("YEEEEEEEEEEE");
	}
	else
	{
		CCLOG("NOOOOOOOO");
	}
}

bool HelloWorld::checkCanContinue(){
	bool cell_1 = false;
	bool cell_2 = false;
	bool cell_3 = false;
	for (int i = 0; i < hex_vec.size(); i++){
		for (int j = 0; j < hex_vec.at(0).size(); j++){
			HexCellSlot* _slot = hex_vec.at(i).at(j);
			if (_slot->isEnable && !_slot->isSet)
			{
				if (checkCanSet(hec_cell_group_1, Vec2(j, i) + Vec2::ONE)){
					hec_cell_group_1->setAllOpacity(255);
					cell_1 = true;
				}
				else if (!cell_1)
				{
					hec_cell_group_1->setAllOpacity(100);
				}
				if (checkCanSet(hec_cell_group_2, Vec2(j, i) + Vec2::ONE)){
					hec_cell_group_2->setAllOpacity(255);
					cell_2 = true;
				}
				else if(!cell_2)
				{
					hec_cell_group_2->setAllOpacity(100);
				}
				if (checkCanSet(hec_cell_group_3, Vec2(j, i) + Vec2::ONE)){
					hec_cell_group_3->setAllOpacity(255);
					cell_3 = true;
				}
				else if (!cell_3)
				{
					hec_cell_group_3->setAllOpacity(100);
				}
			}
		}
	}
	if (cell_1 || cell_2 || cell_3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void HelloWorld::checkAndClearLines(){
	std::vector<std::vector<HexCellSlot*>> willClearHine;

	for (int i = 0; i < hex_vec.size(); i++){
		std::vector<HexCellSlot*> _tempLine;
		bool line_flag = true;
		for (int j = 0; j < hex_vec.at(i).size(); j++){
			HexCellSlot* _tempSlot = hex_vec.at(i).at(j);
			if (_tempSlot->isEnable && !_tempSlot->isSet)
			{
				line_flag = false;
			}
			if (_tempSlot->isEnable)
			{
				_tempLine.push_back(_tempSlot);
			}
		}
		if (line_flag	)
		{
			willClearHine.push_back(_tempLine);
		}
	}

	for (int i = 0; i < hex_vec.size(); i++){
		std::vector<HexCellSlot*> _tempLine;
		bool line_flag = true;
		for (int j = 0; j < hex_vec.at(i).size(); j++){
			HexCellSlot* _tempSlot = hex_vec.at(j).at(i);
			if (_tempSlot->isEnable && !_tempSlot->isSet)
			{
				line_flag = false;
			}
			if (_tempSlot->isEnable)
			{
				_tempLine.push_back(_tempSlot);
			}
		}
		if (line_flag)
		{
			willClearHine.push_back(_tempLine);
		}
	}

	std::vector<Vec2> line_45;
	for (int i = 0; i <= HEX_RID; i++){
		line_45.push_back(Vec2(1, 1 + i));
	}
	for (int i = 1; i <= HEX_RID; i++){
		line_45.push_back(Vec2(1+i, 1));
	}
	for (int i = 0; i < line_45.size(); i++){
		std::vector<HexCellSlot*> _tempLine;
		bool line_flag = true;
		Vec2 _head = line_45.at(i);
		for (int j = 0; j < HEX_RID * 2 + 1; j++)
		{
			if (_head.x + j>HEX_RID * 2 + 1 || _head.y + j>HEX_RID * 2 + 1)
			{
				continue;
			}
			HexCellSlot* _tempSlot = hex_vec.at(_head.x + j -1).at(_head.y + j -1);
			if (_tempSlot->isEnable && !_tempSlot->isSet)
			{
				line_flag = false;
			}
			if (_tempSlot->isEnable)
			{
				_tempLine.push_back(_tempSlot);
			}
		}
		if (line_flag)
		{
			willClearHine.push_back(_tempLine);
		}
	}

	for (int i = 0; i < willClearHine.size(); i++){
		for (int j = 0; j < willClearHine.at(i).size(); j++){
			HexCellSlot* _tempSlot = willClearHine.at(i).at(j);
			_tempSlot->cellInBG->initWithFile("test.png");
			_tempSlot->isSet = false;
            current_score += 1;
		}
	}
    current_score_label->setString(String::createWithFormat("score: %d", current_score)->_string);
}

void HelloWorld::resetGame(){
	for (int i = 0; i < hex_vec.size(); i++){
		for (int j = 0; j < hex_vec.at(i).size(); j++){
			HexCellSlot* _slot = hex_vec.at(i).at(j);
			_slot->cellInBG->initWithFile("test.png");
			_slot->isSet = false;
		}
	}
	hec_cell_group_1->removeAllChildrenWithCleanup(true);
	hec_cell_group_2->removeAllChildrenWithCleanup(true);
	hec_cell_group_3->removeAllChildrenWithCleanup(true);
	hec_cell_group_1 = nullptr;
	hec_cell_group_2 = nullptr;
	hec_cell_group_3 = nullptr;
	resetCellGroup();
    current_score = 0;
    current_score_label->setString(String::createWithFormat("score: %d", current_score)->_string);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
//     Director::getInstance()->end();
	resetGame();
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//     exit(0);
// #endif
}
