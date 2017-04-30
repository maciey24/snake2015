#ifndef FOOD_H
#define FOOD_H

#include "mapitem.h"

enum foodType
{
    APPLE = 0,
    BANANA = 1,
    HEART = 2
};

class Food : public MapItem
{
private:
    foodType type;
    int lengthModifier;
    int speedModifier;
    int livesModifier;
    bool spawned;
public:
    Food(int x, int y, foodType type, Map* map);
    ~Food();

    inline void setXY(int x,int y){this->x = x;this->y = y;}
    void setType(foodType type);
    inline bool getSpawned(){return spawned;}
    inline int getLengthModifier(){return lengthModifier;}
    inline int getSpeedModifier(){return speedModifier;}
    inline int getLiveshModifier(){return livesModifier;}
    void spawn();
    void setType();
    foodType getType(){return type;}
    inline void remove(){(*map)(x,y) = empty;spawned=false;}
    inline void replace(){(*map)(x,y) = snakebody;spawned=false;}
};

#endif // FOOD_H
