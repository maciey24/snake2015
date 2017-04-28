#include <iostream>
#include <ctime>
#include <cstdlib>

#include "game.h"
#include "map.h"
#include "ui.h"
#include "snake.h"

using namespace std;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    UI *ui = new UI();

    ui->sdlMain();
    return 0;
}
