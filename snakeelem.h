#ifndef SNAKEELEM_H
#define SNAKEELEM_H

#include "mapitem.h"

class SnakeElem : public MapItem
{
public:
    SnakeElem(int x, int y, Map* map);
    ~SnakeElem();

    inline void spawn(){(*map)(x,y) = snakebody;}
    inline void remove(){(*map)(x,y) = empty;}
};

#endif // SNAKEELEM_H
