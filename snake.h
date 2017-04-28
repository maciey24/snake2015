#ifndef SNAKE_H
#define SNAKE_H

#include <deque>

using namespace std;

enum directionType
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

enum collisionType
{
    NO_COL = 0,
    SNAKE_COL = 1,
    OBSTACLE_COL = 2,
    FOOD_COL = 3
};

class Map;
class SnakeElem;
class Food;

class Snake
{
private:
    int lives;
    int length;
    int speed;
    int steps;
    int toadd;
    int tosub;
    int turboSteps;
    int nextX, nextY, tailX, tailY, oldX, oldY;
    bool ghost;
    bool dieCommand;
    directionType direction;
    collisionType collision;
    Map *map;
    deque <SnakeElem*> snakeElems;
public:
    Snake(Map *map);
    ~Snake();

    inline int getLength(){return length;}
    inline void setLength(int value){this->length=value;}
    inline directionType getDirection(){return direction;}
    void setDirection(directionType direction);
    inline collisionType getCollision(){return collision;}
    inline void setCollision(collisionType collision){this->collision=collision;}
    inline int getLives(){return lives;}
    inline void setLives(int value){lives = value;}
    inline int getSpeed(){return speed;}
    inline void setSpeed(int value){speed=value;}
    inline int getToAdd(){return toadd;}
    inline void setToAdd(int addend){toadd = toadd + addend;}
    inline int getToSub(){return tosub;}
    inline void setSteps(int value){this->steps=value;}
    inline int getSteps(){return steps;}
    inline void setToSub(int subtr){tosub = tosub + subtr;}
    inline void setGhost(bool value){this->ghost=value;}
    inline bool getGhost(){return ghost;}
    inline void setTurboSteps(int addend){this->turboSteps+=addend;}
    inline void setDieCommand(int value){this->dieCommand=value;}
    inline bool getDieCommand(){return dieCommand;}
    inline void setNextX(int value){this->nextX=value;}
    inline void setNextY(int value){this->nextY=value;}
    inline int getTurboSteps(){return turboSteps;}
    inline int getNextX(){return nextX;}
    inline int getNextY(){return nextY;}
    void newSnakeElem(int x, int y, Map* map);
    inline SnakeElem* getSnakeElem(int i){return snakeElems[i];}

    void clear();
    void init();
    void move();
    void eat(Food *food);
    void detectCollision(int *X, int *Y);
    void computeNextXY(int *X, int *Y, directionType direction);

    bool anotherSnakeElemHasSameXY();
};

#endif // SNAKE_H
