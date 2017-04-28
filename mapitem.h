#ifndef MAPITEM_H
#define MAPITEM_H

#include "map.h"

class MapItem
{
protected:
    int x;
    int y;
    field type;
    Map *map;
public:
    MapItem();
    virtual ~MapItem();
    inline int getX(){return x;}
    inline int getY(){return y;}
    inline void setX(int x){this->x = x;}
    inline void setY(int y){this->y = y;}
    virtual void spawn() = 0;
    virtual void remove() = 0;
};

#endif // MAPITEM_H
