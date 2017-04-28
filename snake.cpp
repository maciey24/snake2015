#include <cmath>

#include "snake.h"
#include "snakeelem.h"
#include "food.h"

Snake::Snake(Map *map)
{
    this->lives = 3; //lives(3),
    this->length = 1;
    this->speed = 1;//TODO w wersji tag wartość 1
    this->direction = RIGHT;
    this->collision = NO_COL;
    this->map = map;
    this->toadd = 2;
    this->tosub = 0;
}

Snake::~Snake()
{
    for(int i=0;i<length;i++)
        delete snakeElems[i];
    snakeElems.clear();
}

void Snake::clear()
{
    for(int i=0;i<length;i++)
        delete snakeElems[i];
    snakeElems.clear();
}

void Snake::init()
{
    length = 1;
    toadd = 2;
    ghost = 0;
    steps = 0;
    turboSteps = 0;
    dieCommand = 0;
    nextX = map->getWidth()/2;
    nextY = map->getHeight()/2;
    direction = RIGHT;
    snakeElems.push_back(new SnakeElem(map->getWidth()/2,map->getHeight()/2,map));//TODO dla mapy o wysokości 1?

        snakeElems.back()->spawn();
}

void Snake::newSnakeElem(int x,int y, Map* map)
{
    snakeElems.push_back(new SnakeElem(x,y,map));
}

void Snake::move()
{
    if(tosub-toadd>=length){toadd=0; tosub=0; tosub=length-1;}
    if(toadd-tosub>=0) {toadd=toadd-tosub; tosub=0;}//żeby nie czekało ze skróceniem węża do całego wyjścia z ziemi
    else {tosub=tosub-toadd; toadd=0;}
    oldX=snakeElems.back()->getX(); oldY=snakeElems.back()->getY();
    if(toadd==0)//implementacja wydłużania - mówisz wężowi tylko o ile się ma wydłużyć (zmienna toadd)
    {
        tosub++;
        do{
            if(!anotherSnakeElemHasSameXY())
            {snakeElems.front()->remove();}//^jeżeli żaden inny klocek nie ma takich współrzędnych (może się krzyżować)
            tailX=snakeElems.front()->getX();
            tailY=snakeElems.front()->getY();
            snakeElems.pop_front();
            tosub--;
            } while(tosub>0);
        toadd++;
    }
    toadd--;
    computeNextXY(&nextX, &nextY, direction);
    detectCollision(&nextX, &nextY);
    if(collision==OBSTACLE_COL)
    {
        snakeElems.push_back(new SnakeElem(tailX, tailY, map));
        nextX=oldX; nextY=oldY;
    }
    else
    {snakeElems.push_back(new SnakeElem(nextX, nextY, map));}
    if(turboSteps>0) {this->setTurboSteps(-1);}
    snakeElems.back()->spawn();
    length=snakeElems.size();
}

void Snake::computeNextXY(int *X, int *Y, directionType direction)
{
    switch(direction)
    {
        case UP:
        {
            if(snakeElems.back()->getY()-1 == -1)
            {*X=snakeElems.back()->getX(); *Y=map->getHeight()-1;}
            else
            {*X=snakeElems.back()->getX(); *Y=snakeElems.back()->getY()-1;}
        } break;
        case DOWN:
        {
            if(snakeElems.back()->getY()+1 == map->getHeight())
            {*X=snakeElems.back()->getX(); *Y=0;}
            else
            {*X=snakeElems.back()->getX(); *Y=snakeElems.back()->getY()+1;}
        } break;
        case LEFT:
        {
            if(snakeElems.back()->getX()-1 == -1)
            {*X=map->getWidth()-1; *Y=snakeElems.back()->getY();}
            else
            {*X=snakeElems.back()->getX()-1; *Y=snakeElems.back()->getY();}
        } break;
        case RIGHT:
        {
            if(snakeElems.back()->getX()+1 == map->getWidth())
            {*X=0; *Y=snakeElems.back()->getY();}
            else
            {*X=snakeElems.back()->getX()+1; *Y=snakeElems.back()->getY();}
        } break;
    }
}

void Snake::detectCollision(int *X, int *Y)
{
    if((*map)(*X, *Y)==snakebody){collision = SNAKE_COL;}
    if((*map)(*X, *Y)==obstacle){collision = OBSTACLE_COL;}
    if((*map)(*X, *Y)==apple || (*map)(*X, *Y)==banana || (*map)(*X, *Y)==heart){collision = FOOD_COL;}
    if((*map)(*X, *Y)==empty){collision = NO_COL;}
    if(ghost==true&&(collision==SNAKE_COL)){collision=NO_COL;}
    if(dieCommand==true){dieCommand=false; collision=OBSTACLE_COL;}
}

void Snake::eat(Food *food)
{
    this->collision=NO_COL;
    food->replace();
    setToAdd(food->getLengthModifier());
    if(lives+food->getLiveshModifier()<=99)
    setLives(lives+food->getLiveshModifier());
    else setLives(99);
    //TODO implementacja reakcji na wartość livesModifier - lives jest zmmienną w klasie Game...
    if(turboSteps==0) {this->setTurboSteps(food->getSpeedModifier());}
}

void Snake::setDirection(directionType direction)
{
    if((abs(this->direction-direction)!=1) || (((this->direction)*(direction))==2))
        this->direction=direction;
}

bool Snake::anotherSnakeElemHasSameXY() //#TODO: argument - element kolejki, jeżeli się nie mylę to można to będzie wykorzystać przy funkcji detect collision
{//funkcja sprawdza, czy pierwszy elem. kolejki ma takie same XY jak jakiś inny element
    for(unsigned i=1; i<snakeElems.size();i++)
    {
        if(snakeElems.front()->getX()==snakeElems[i]->getX() && snakeElems.front()->getY()==snakeElems[i]->getY())
        return true;
    }
    return false;
}
