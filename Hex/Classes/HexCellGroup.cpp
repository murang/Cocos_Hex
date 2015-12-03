#include "HexCellGroup.h"
#include "HelloWorldScene.h"

HexCellGroup::HexCellGroup()
{
}

HexCellGroup::~HexCellGroup()
{
}

bool HexCellGroup::initWithLocs(std::vector<Vec2> cells){
	_current_cells_pos = cells;
	for (int i = 0; i < cells.size(); i++){
		auto _cell_sprite = Sprite::create("hex_cell.png");
		float fixed_cell_w = HEX_CELL_SIZE * sqrt(0.75);
		float fixed_cell_h = HEX_CELL_SIZE * 0.75;
		float cell_x = cells.at(i).x;
		float cell_y = cells.at(i).y;
		float fixedPos_x = fixed_cell_w * cell_y - (fixed_cell_w / 2.0) *cell_x;
		float fixedPos_y = fixed_cell_h * cell_x * -1;
		_cell_sprite->setPosition(fixedPos_x, fixedPos_y);
		this->addChild(_cell_sprite);
		_current_cells.push_back(_cell_sprite);
	};
	Vec2 _offset = this->getGroupCenterOffset();
	for (int i = 0; i < _current_cells.size(); i++){
		_current_cells.at(i)->setPosition(_current_cells.at(i)->getPosition() - _offset);
	};

	return true;
};

Vec2 HexCellGroup::getGroupCenterOffset(){
	float x_min = _current_cells.at(0)->getPositionX();
	float x_max = _current_cells.at(0)->getPositionX();
	float y_min = _current_cells.at(0)->getPositionY();
	float y_max = _current_cells.at(0)->getPositionY();
	for (int i = 0; i < _current_cells.size(); i++){
		x_min = MIN(x_min, _current_cells.at(i)->getPositionX());
		x_max = MAX(x_max, _current_cells.at(i)->getPositionX());
		y_min = MIN(y_min, _current_cells.at(i)->getPositionY());
		y_max = MAX(y_max, _current_cells.at(i)->getPositionY());
	};
	float offset_x = x_min + (x_max - x_min) / 2;
	float offset_y = y_min + (y_max - y_min) / 2;
	return Vec2(offset_x, offset_y);
};

void HexCellGroup::setAllOpacity(int op){
	for (int i = 0; i < _current_cells.size(); i++)
	{
		_current_cells.at(i)->setOpacity(op);
	}
}