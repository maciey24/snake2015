#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#undef main

using namespace std;

enum menuSurfacesList
{
    MENU_SURFACE1,
    MENU_SURFACE2,
    MENU_SURFACE3,
    MENU_SURFACE4,
    MENU_SURFACE5,
    MENU_SURFACE6,
    MENU_SURFACE_TOTAL
};

class Map;
class Game;
class UI
{
private:
    int menuOption;
    int pauseOption;
    int selectedPauseOption;
    int optionsOption;
    char key;
    char buffer[7];
    //Rozdzielczość okna w SDLu
    int screenWidth;
    int screenHeight;
    Game *game;
    //SDL Part
    int menu;
    int selectedMenu;
    SDL_Keycode sdlKey;
    SDL_Window *window;
    SDL_Surface *screen;
    SDL_Surface *icon;
    SDL_Surface *currentSurface;
    SDL_Surface *menuSurfaces[MENU_SURFACE_TOTAL];
    SDL_Surface *authorsSurface;
    SDL_Surface *pauseSurface1;
    SDL_Surface *pauseSurface2;
    SDL_Surface *pauseSurface3;
    SDL_Surface *promptSurface;
    SDL_Surface *levelUpSurface;
    SDL_Surface *gameOverSurface;
    SDL_Surface *gameSavedSurface;
    SDL_Event event;
    SDL_Renderer *renderer;
    //SDL Audio Part
    Mix_Chunk *moveSound;
    Mix_Chunk *eatSound;
    Mix_Chunk *gameOverSound;
    Mix_Chunk *levelUpSound;
    Mix_Chunk *lostLiveSound;
    Mix_Chunk *speedUpSound;
    //SDL TTF
    TTF_Font *fontEhsmb;
    TTF_Font *fontPixeled;
    SDL_Surface *scoreSurface;
    SDL_Surface *scoreTextSurface;
    SDL_Surface *livesSurface;
    SDL_Surface *scoreboardSurface;
    SDL_Surface *optionsTextSurface1;
    SDL_Surface *optionsTextSurface2;
    SDL_Surface *optionsTextSurface3;
    SDL_Surface *optionsTextSurface4;
    SDL_Surface *optionsPtrSurface;
    SDL_Texture *scoreTexture;
    SDL_Texture *scoreTextTexture;
    SDL_Texture *livesTexture;
    SDL_Texture *scoreboardTexture;
    SDL_Texture *optionsTextTexture1;
    SDL_Texture *optionsTextTexture2;
    SDL_Texture *optionsTextTexture3;
    SDL_Texture *optionsTextTexture4;
    SDL_Texture *optionsPtrTexture;
    SDL_Color textColor;
    SDL_Color markerColor;
    bool quit;
public:
    UI();
    ~UI();
    Game* getGame(){return game;}
    inline char& getKey(){return key;}
    inline SDL_Keycode& getSdlKey(){return sdlKey;}
    void key2action();
    inline char* getBuffer(){return buffer;}
    void key2buffer(char key, char buf[]);
    void readBuffer();
    bool compTabs(char* a, char* b);
    char* scoreToCharPtr(int n);
    char* intToCharPtr(int n);
    //SDL Part
    void sdlMain();
    bool sdlInit();
    bool sdlLoadMedia();
    void sdlClose();
    void sdlSetKey();
    void sdlDrawMap(Map*);
    void sdlScoreboard();
    void sdlOptionsMenu();
    void sdlPauseMenu();
    void sdlGameOver();
    void sdlLevelComplete();
    void sdlDisplayPrompt();
    void sdlGameSaved();
    SDL_Surface* loadSurface( std::string path );
    void sdlPlayMoveSound();
    void sdlPlayEatSound();
    void sdlPlayLostLiveSound();
    void sdlPlaySpeedSound();
};


#endif // UI_H
