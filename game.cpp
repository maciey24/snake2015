#include <iostream> //chwilowo do debugowania
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <fstream>

#include "game.h"
#include "map.h"
#include "snake.h"
#include "ui.h"
#include "food.h"
#include "snakeelem.h"

using namespace std;

Game::Game(UI *ui)
    :
      time(0),
      score(0),
      difficulty(3),
      stepsCountdown(0),
      paused(false),
      initialized(false),
      toLoad(false),
      inProgress(false),
      soundsEnabled(true),
//      startTime(clock()),
      currentTime(0),
      clockLatency(2000),
      map(new Map()),
      snake(new Snake(map)),
      food(new Food(0,0,APPLE,map)),
      ui(ui)
{
    #ifdef _DEBUG
    cout << "Game constructor" << endl;
    #endif
}

Game::~Game()
{
    delete map;
    delete snake;
    delete food;
    delete ui;
}

int Game::getRandom(int min, int max)
{
    int random = rand()%(max-min+1)+min;
    return random;
}

void Game::init()
{
    map->init();
    placeObstacles();    //Postawienie przeszkód
    snake->init();
}

bool Game::game()
{
    //Gra wystartowała
    inProgress = true;

    //Inicjalizacja
    if(!initialized)
    {
        snake->setLives(3);
        snake->setSpeed(1);
        levels=true;
        init();
        initialized=true;
        startTime=clock();
    }

    if(toLoad)
    {
        load();
        snake->setToSub(3);
        toLoad=false;
    }
    if(snake->getLives()>0)
    {
        //Dobieranie opóźnienia zegara do prędkości węża-62*x+582
        if(snake->getTurboSteps()>0)
        {
            clockLatency = 80;
        }
        else
        {
            clockLatency = (-62*(snake->getSpeed())+582)*(snake->getSpeed()/(snake->getSteps()+1)+1);
        }
        //Spawnowanie jedzenia
        if(!(food->getSpawned()))
            spawnFood();

        if(!paused)
        {
            currentTime = clock() - startTime;
            ui->readBuffer();
            if(currentTime>=clockLatency)
            {
                ui->key2action();
                snake->move();
                if(soundsEnabled) ui->sdlPlayMoveSound();
                startTime = clock(); //zresetowanie zegara
                snake->setSteps(snake->getSteps()+1);
                if(stepsCountdown>0)
                {
                    stepsCountdown--;
                    if(stepsCountdown==0) food->remove();
                }
                if(snake->getLength()==snake->getSpeed()*3+7) {if(getLevels()){levelUp();}}
            }
            if(snake->getCollision()==SNAKE_COL || snake->getCollision()==OBSTACLE_COL) //TODO czy to nie w klamrach linii 102?
            {
                snake->setCollision(NO_COL);//TODO a moze to dac w metodzie snake init?
                map->clear();
                food->remove();
                map->init();
                snake->clear();
                placeObstacles();
                snake->init();
                snake->setLives(snake->getLives()-1);
                if(snake->getLives()!=0)
                {
                    if(soundsEnabled)
                        ui->sdlPlayLostLiveSound();
                    ui->sdlDisplayPrompt();
                }
                ui->getSdlKey()='d';
            }

            if(snake->getCollision()==FOOD_COL)
            {
                if(food->getType()==BANANA)
                {
                    if(soundsEnabled)
                    {
                        ui->sdlPlaySpeedSound();
                    }
                }
                snake->eat(food);
                if(soundsEnabled) ui->sdlPlayEatSound();
                if(score<9999990)
                score+=10;
                else
                score=9999999;
            }
        }
    }
    else
    {
        saveScore();
        map->clear();
        snake->clear();
        food->remove();
        initialized=false;
        inProgress = false;
        score = 0; //zresetowanie wyniku
    }

    return inProgress;
}

void Game::spawnFood()
{
    int x = getRandom(1,map->getWidth()-1);
    int y = getRandom(1,map->getHeight()-1);
    foodType type;
    while((*map)(x,y)!=empty)
    {
        x = getRandom(0,map->getWidth()-1);
        y = getRandom(0,map->getHeight()-1);
    }
    if(getRandom(0, 127)<5+snake->getLength()*0.25){type = BANANA; stepsCountdown=map->getWidth()+snake->getSpeed()*snake->getSpeed();}
    else if(getRandom(0, 127)<9-snake->getLives()){type = HEART; stepsCountdown=map->getWidth()+map->getHeight()/2+snake->getSpeed()*snake->getSpeed();}
    else {type = APPLE; stepsCountdown=0;}
    food->setXY(x,y);
    food->setType(type);
    food->spawn();
}

void Game::levelUp()
{
    ui->sdlLevelComplete();
    map->clear();
    map->init();
    snake->clear();
    snake->init();
    food->remove();
    snake->setSpeed(snake->getSpeed()+1);
    if(snake->getSpeed()==8) this->setLevels(false);
    placeObstacles();
}

void Game::save()
{
    ofstream saving;
    saving.open("save.bin");
    saving<<score; saving<<"\n";
    saving<<difficulty; saving<<"\n";
    saving<<initialized; saving<<"\n";
    saving<<levels; saving<<"\n";
    saving<<inProgress; saving<<"\n";

    saving<<snake->getLives(); saving<<"\n";
    saving<<snake->getLength(); saving<<"\n";
    saving<<snake->getSpeed(); saving<<"\n";
    saving<<snake->getSteps(); saving<<"\n";
    saving<<snake->getToAdd(); saving<<"\n";
    saving<<snake->getToSub(); saving<<"\n";
    saving<<snake->getDirection(); saving<<"\n";
    saving<<snake->getTurboSteps(); saving<<"\n";
    saving<<snake->getNextX(); saving<<"\n";
    saving<<snake->getNextY(); saving<<"\n";
    saving<<snake->getGhost(); saving<<"\n";

    for(int i=0;i<map->getHeight();i++)
    {
        for(int j=0;j<map->getWidth();j++)
        {
            saving<<(*map)(j,i); saving<<"\n";
        }
    }
    for(int i=0;i<snake->getLength();i++)
    {
        saving<<snake->getSnakeElem(i)->getX(); saving<<"\n";
        saving<<snake->getSnakeElem(i)->getY(); saving<<"\n";
    }
    saving.close();
}

void Game::load()
{
    int x, y;

    ifstream loading;
    loading.open("save.bin");
    loading>>x; score=x;
    loading>>x; difficulty=x;
    loading>>x; initialized=x;
    loading>>x; levels=x;
    loading>>x; inProgress=x;

    loading>>x; getSnake()->setLives(x);
    loading>>x; getSnake()->setLength(x);
    loading>>x; getSnake()->setSpeed(x);
    loading>>x; getSnake()->setSteps(x);
    loading>>x; getSnake()->setToAdd(x);
    loading>>x; getSnake()->setToSub(x);
    loading>>x;
    switch(x)
    {
    case 0: getSnake()->setDirection(UP); break;
    case 1: getSnake()->setDirection(DOWN); break;
    case 2: getSnake()->setDirection(LEFT); break;
    case 3: getSnake()->setDirection(RIGHT); break;
    }
    loading>>x; getSnake()->setTurboSteps(x);
    loading>>x; getSnake()->setNextX(x);
    loading>>x; getSnake()->setNextY(x);
    loading>>x; getSnake()->setGhost(x);

    for(int i=0;i<map->getHeight();i++)
    {
        for(int j=0;j<map->getWidth();j++)
        {
            loading>>x;
            switch(x)
            {
            case 0: (*map)(j,i)=empty; break;
            case 1: (*map)(j,i)=snakebody; break;
            case 2: (*map)(j,i)=empty; break;
            case 3: (*map)(j,i)=obstacle; break;
            case 4: (*map)(j,i)=empty; break;
            case 5: (*map)(j,i)=empty; break;
            }

        }
    }
    for(int i=0;i<snake->getLength();i++)
    {
        loading>>x; loading>>y;
        snake->newSnakeElem(x, y, map);
    }
    loading.close();
}

void Game::saveScore()
{
    int tab[10];
    int x;
    ifstream read;
    read.open("scores.txt");
    for(int i=0; i<10; i++)
    {
        read>>x;
        tab[i]=x;
    }
    read.close();

    if(score>tab[9])
    {
        tab[9]=score;
    }

    for (int i = 0; i<10; i++)
    {
        for (int j=0; j<10-1-i; j++)
        {
            if (tab[j] < tab[j+1])
            {
                x = tab[j+1];
                tab[j+1] = tab[j];
                tab[j] = x;
            }
        }
    }

    ofstream saving;
    saving.open("scores.txt");
    for (int i=0; i<10; i++)
    {
        saving<<tab[i]; saving<<"\n";

    }
    saving.close();
}

void Game::placeObstacles()
{
    if(difficulty>1)
    {
        for(int j=0; j<map->getWidth(); j++)
        {
            for(int i=0; i<map->getHeight(); i++)
            {
                if(i==0||j==0||i==map->getHeight()-1||j==map->getWidth()-1)
                {
                    (*map)(j,i) = obstacle;
                }
            }
        }
    }
    if(difficulty>2)
    {
        switch(snake->getSpeed())
        {
            case 1:
            {
            for(int j=0; j<map->getWidth(); j++)
            {
                for(int i=0; i<map->getHeight(); i++)
                {
                    if((i==map->getHeight()/4||i==map->getHeight()-map->getHeight()/4)&&(j>map->getWidth()/4&&j<map->getWidth()-map->getWidth()/4))
                    {
                        (*map)(j,i) = obstacle;
                    }
                    if((i==0||(i==map->getHeight()-1))&&j==map->getWidth()/2) (*map)(j,i) = empty;
                }

            }
            } break;
            case 2:
            {
            for(int j=0; j<map->getWidth(); j++)
            {
                for(int i=0; i<map->getHeight(); i++)
                {
                    if(((j-map->getWidth()/4)*(j-map->getWidth()/4)+(i-map->getHeight()/4)*(i-map->getHeight()/4)<2)||((j-(map->getWidth()-map->getWidth()/4))*(j-(map->getWidth()-map->getWidth()/4))+(i-map->getHeight()/4)*(i-map->getHeight()/4)<2)
                            ||((j-map->getWidth()/4)*(j-map->getWidth()/4)+(i-(map->getHeight()-map->getHeight()/4))*(i-(map->getHeight()-map->getHeight()/4))<2)
                            ||((j-(map->getWidth()-map->getWidth()/4))*(j-(map->getWidth()-map->getWidth()/4))+(i-(map->getHeight()-map->getHeight()/4))*(i-(map->getHeight()-map->getHeight()/4))<2))
                    {
                        (*map)(j,i) = obstacle;
                    }
                }
            }
            } break;
            case 3:
            {
            for(int j=0; j<map->getWidth(); j++)
            {
                for(int i=0; i<map->getHeight(); i++)
                {
                    if(i<=(-((j-map->getWidth()/2)*(j-map->getWidth()/2))+map->getHeight()/2-2))
                    {
                        (*map)(j,i) = obstacle;
                    }
                }
            }
            } break;
//            case 3:
//            {
//                for(int j=0; j<map->getWidth(); j++)
//                {
//                    for(int i=0; i<map->getHeight(); i++)
//                    {
//                        if(getRandom(0,100)<3&&(((j-map->getWidth()/2)*(j-map->getWidth()/2)+(i-map->getHeight()/2)*(i-map->getHeight()/2))>36))
//                        {
//                            (*map)(j,i) = obstacle;
//                        }
//                    }
//                }
//            } break;
            case 4:
            {
            for(int j=0; j<map->getWidth(); j++)
            {
                for(int i=0; i<map->getHeight(); i++)
                {
                    if(i<(50.0/j))
                    {
                        (*map)(j,i) = obstacle;
                    }
                }
            }
            } break;
            case 5:
            {
            for(int j=0; j<map->getWidth(); j++)
            {
                for(int i=0; i<map->getHeight(); i++)
                {
                    if(j*i%21==0&&j%21!=0&&i%21!=0)
                    {
                        (*map)(j,i) = obstacle;
                    }
                    if(((j>=map->getWidth()/3&&j<=map->getHeight()-map->getHeight()/3)||(i>=map->getHeight()/3&&i<=map->getHeight()-map->getHeight()/3))&&(i==0||j==0||i==map->getHeight()-1||j==map->getWidth()-1))
                    (*map)(j,i) = empty;
                }
            }
            } break;
            case 6:
            {
            for(int j=0; j<map->getWidth(); j++)
            {
                for(int i=0; i<map->getHeight(); i++)
                {
                    if(i==0||j==0||i==map->getHeight()-1||j==map->getWidth()-1)
                    {
                        (*map)(j,i) = empty;
                    }
                    if((j==map->getWidth()/3)||(i==map->getHeight()/2&&(j<=map->getWidth()/3||j==map->getWidth()-1))||((i==map->getHeight()/3||i==map->getHeight()-map->getHeight()/3)&&(j>=map->getWidth()/3||j==0)))
                    {
                        (*map)(j,i) = obstacle;
                    }
                }
            }
            } break;
            case 7:
            {
            for(int j=0; j<map->getWidth(); j++)
            {
                for(int i=0; i<map->getHeight(); i++)
                {
                    if((j+i>((map->getWidth()+map->getHeight())-11))||(j+i<9))
                    {
                        (*map)(j,i) = obstacle;
                    }
                }
            }
            } break;
            case 8:
            {
            for(int j=0; j<map->getWidth(); j++)
            {
                for(int i=0; i<map->getHeight(); i++)
                {
                    if(((i==map->getHeight()/2)||(j==map->getWidth()/2))&&((j-map->getWidth()/2)*(j-map->getWidth()/2)+(i-map->getHeight()/2)*(i-map->getHeight()/2))>16)
                    {
                        (*map)(j,i) = obstacle;
                    }
                }
            }
            } break;
            default:
            {
            for(int j=0; j<map->getWidth(); j++)
            {
                for(int i=0; i<map->getHeight(); i++)
                {
                    if(i==0||j==0||i==map->getHeight()-1||j==map->getWidth()-1)
                    {
                        (*map)(j,i) = empty;
                    }
                    if((j==map->getWidth()/3)||(i==map->getHeight()/2&&(j<=map->getWidth()/3||j==map->getWidth()-1))||((i==map->getHeight()/3||i==map->getHeight()-map->getHeight()/3)&&(j>=map->getWidth()/3||j==0)))
                    {
                        (*map)(j,i) = obstacle;
                    }
                }
            }
            } break;
//            case 8:
//            {
//            for(int j=0; j<map->getWidth(); j++)
//            {
//                for(int i=0; i<map->getHeight(); i++)
//                {
//                    if(i==j+3)
//                    {
//                        (*map)(j,i) = obstacle;
//                    }
//                    if(i==0||j==0||i==map->getHeight()-1||j==map->getWidth()-1)
//                    {
//                        (*map)(j,i) = empty;
//                    }
//                }
//            }
//            } break;
        }
    }
}

