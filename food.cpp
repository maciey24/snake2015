#include "food.h"

Food::Food(int x, int y, foodType type, Map *map)
{
    this->x = x;
    this->y = y;
    this->type = APPLE;
    this->spawned = false;
    this->map = map;
}

Food::~Food(){}

void Food::setType(foodType type)
{
    this->type=type;
    if(type==APPLE)
    {
        this->lengthModifier = 1;
        this->speedModifier = 0;
        this->livesModifier = 0;
    }
    if(type==BANANA)
    {
        this->lengthModifier = -5;
        this->speedModifier = 10;
        this->livesModifier = 0;
    }
    if(type==HEART)
    {
        this->lengthModifier = 0;
        this->speedModifier = 0;
        this->livesModifier = 1;
    }
}

void Food::spawn()
{
    if(type==APPLE)(*map)(x,y) = apple;
    if(type==BANANA)(*map)(x,y) = banana;
    if(type==HEART)(*map)(x,y) = heart;
    spawned=true;
}
