#include <iostream>

#include "map.h"

Map::Map():width(20),height(20)
{
    #ifdef _DEBUG
        cout << "Map constructor" << endl;
    #endif
}

Map::~Map()
{
    //Wyczyszczenie pamieci przydzielonej na tablice pól
    for(int i=0;i<height;i++)
        delete [] fields[i];
    delete [] fields;
    #ifdef _DEBUG
        cout << "Map destructor" << endl;
    #endif
}

void Map::clear()
{
    //Wyczyszczenie pamieci przydzielonej na tablice pól
    for(int i=0;i<height;i++)
        delete [] fields[i];
    delete [] fields;
}

void Map::init()
{
    //Inicjalizacja tablicy char fields[height][width]
    fields = new field*[height];
    for(int i=0;i<height;i++)
        fields[i] = new field[width];
    reset(); //ustawienie pustych pol na mapie
    #ifdef _DEBUG
        cout << "Map initialization" << endl;
    #endif
}

void Map::reset()
{
    //Wypelnienie mapy pustymi polami
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
            fields[i][j] = empty;
}
