#ifndef MAP_H
#define MAP_H

using namespace std;

enum field
{
    empty = 0,
    snakebody = 1,
    apple = 2,
    obstacle = 3,
    banana = 4,
    heart = 5
};

class Map
{
    friend class MapItem;
private:
    int width;
    int height;
    field **fields;
public:
    Map();
    ~Map();
    void clear();
    void init();
    void reset();
    field& operator()(int x, int y){return fields[y][x];}
    inline field& getField(int x, int y){return fields[y][x];}
    void setField(int x, int y, int field);
    inline int getHeight(){return height;}
    inline int getWidth(){return width;}
};

#endif // MAP_H
