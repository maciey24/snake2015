#ifndef GAME_H
#define GAME_H

class Map;
class Snake;
class Food;
class UI;
class Game
{
private:
    int time;
    int score;
    int difficulty;
    int stepsCountdown;
    bool paused;
    bool initialized;
    bool levels;
    bool inProgress;
    bool toLoad;
    bool soundsEnabled;
    double startTime;
    double currentTime;
    double clockLatency;
    Map *map;
    Snake *snake;
    Food *food;
    int getRandom(int min, int max);
public:
    UI *ui;
    Game(UI *ui);
    ~Game();
    inline int getDifficulty(){return difficulty;}
    inline void setDifficulty(int difficulty){this->difficulty=difficulty;}
    inline int getScore(){return score;}
    inline void setScore(int addend){this->score = score + addend;}
    inline void setPaused(bool value){this->paused = value;}
    inline bool getPaused(){return paused;}
    inline void setLevels(bool value){this->levels = value;}
    inline bool getLevels(){return levels;}
    inline void setToLoad(bool value){this->toLoad=value;}
    inline Map* getMap(){return map;}
    inline Snake* getSnake(){return snake;}
    bool getSoundsEnabled(){return soundsEnabled;}
    void setSoundsEnabled(bool value){this->soundsEnabled = value;}
    void init();
    bool game();
    void save();
    void load();
    void saveScore();
    void spawnFood();
    void placeObstacles();
    void levelUp();
};

#endif // GAME_H
