#include "snakeelem.h"

SnakeElem::SnakeElem(int x, int y, Map *map)
{
    this->x = x;
    this->y = y;
    this->map = map;
}

SnakeElem::~SnakeElem(){}
