#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "ui.h"
#include "map.h"
#include "game.h"
#include "snake.h"

using namespace std;

UI::UI()
    :
      menuOption(1), //ta smienna jest nieuzywana nigdzie? xD
      pauseOption(3),
      optionsOption(1),
      key('d'),
      screenWidth(800),
      screenHeight(600),
      game(new Game(this)),
      menu(0),
      selectedMenu(-1),
      window(NULL),
      screen(NULL),
      icon(NULL),
      currentSurface(NULL),
      renderer(NULL),
      moveSound(NULL),
      eatSound(NULL),
      gameOverSound(NULL),
      speedUpSound(NULL),
      levelUpSound(NULL),
      lostLiveSound(NULL),
      textColor({0,192,0}),
      quit(false)

//      #TODO - init. buffer?
{
    for(int i=0;i<7;i++)
        buffer[i] = ' ';
}
UI::~UI()
{}

void UI::key2action()
{
    //Dla interfejsu SDL
    switch(sdlKey)
    {
        case 'd':
        case 'D':
        case SDLK_RIGHT: game->getSnake()->setDirection(RIGHT); break;

        case 'a':
        case 'A':
        case SDLK_LEFT: game->getSnake()->setDirection(LEFT); break;

        case 'w':
        case 'W':
        case SDLK_UP: game->getSnake()->setDirection(UP); break;

        case 's':
        case 'S':
        case SDLK_DOWN: game->getSnake()->setDirection(DOWN); break;
    }
}

void UI::key2buffer(char key, char buf[])
{
    for(int i=0; i<6 ; i++)
    {
        buf[i]=buf[i+1];
    }
    buf[6]=key;
}

void UI::readBuffer()
{
    if(compTabs(buffer, "longint")){game->getSnake()->setToAdd(10); key2buffer(' ', buffer);}
    if(compTabs(buffer, "endgame")){game->getSnake()->setLives(-1); key2buffer(' ', buffer);}
    if(compTabs(buffer, "levelup")){game->levelUp(); key2buffer(' ', buffer);}
    if(compTabs(buffer, "powerof")){game->getSnake()->setLives(-1); quit=true; key2buffer(' ', buffer);}
    if(compTabs(buffer, "lives--")){game->getSnake()->setLives(game->getSnake()->getLives()-1); key2buffer(' ', buffer);}
    if(compTabs(buffer, "iamacat")){game->getSnake()->setLives(9); key2buffer(' ', buffer);}
    if(compTabs(buffer, "baguvix")){game->getSnake()->setLives(99); key2buffer(' ', buffer);}
    if(compTabs(buffer, "hesoyam")){if(game->getScore()<=9999876) game->setScore(123); key2buffer(' ', buffer);}
    if(compTabs(buffer, "jumpjet")){game->getSnake()->setSpeed(5); key2buffer(' ', buffer);}
    if(compTabs(buffer, "turbooo")){game->getSnake()->setTurboSteps(15);key2buffer(' ', buffer);}
    if(compTabs(buffer, "easyasf")){game->getSnake()->setSpeed(-2); key2buffer(' ', buffer);}
    if(compTabs(buffer, "minus05")){game->getSnake()->setToSub(5); key2buffer(' ', buffer);}
    if(compTabs(buffer, "minus10")){game->getSnake()->setToSub(10); key2buffer(' ', buffer);}
    if(compTabs(buffer, "sudodie")){game->getSnake()->setDieCommand(1); key2buffer(' ', buffer);}
    if(compTabs(buffer, "pauseit")){game->setPaused(true); key2buffer(' ', buffer);}
    if(compTabs(buffer, "imghost")){game->getSnake()->setGhost((game->getSnake()->getGhost()+1)%2); key2buffer(' ', buffer);}
    if(compTabs(buffer, "infinit")){game->setLevels((game->getLevels()+1)%2); key2buffer(' ', buffer);}
    if(compTabs(buffer, "iamadot"))
    {
        game->getSnake()->setToAdd(-game->getSnake()->getToAdd());
        game->getSnake()->setToSub(-game->getSnake()->getToSub());
        game->getSnake()->setToSub(game->getSnake()->getLength()-1);
        key2buffer(' ', buffer);
    }
    if(compTabs(buffer, "resetme"))
    {
         game->getSnake()->setLives(3);
         game->getSnake()->setToAdd(-game->getSnake()->getToAdd());
         game->getSnake()->setToSub(-game->getSnake()->getToSub());
         game->getSnake()->setToSub(game->getSnake()->getLength()-3);
         game->getSnake()->setSpeed(1);
         game->getSnake()->setTurboSteps(0);
         game->getSnake()->setGhost(false);
         game->setLevels(true);
         key2buffer(' ', buffer);
    }
    if(key==27)
    {
        game->setPaused(true);
        key = ' ';
    }
}

bool UI::compTabs(char* a, char* b)
{
    for(int i=0; i<7; i++)
    {
        if(a[i]!=b[i]) {return false;}
    }
    return true;
}

char* UI::scoreToCharPtr(int n)
{
    stringstream sstr;
    sstr << n;
    string text;
    if(n<10)
        text = "000000"+sstr.str();
    else if(n<100)
        text = "00000"+sstr.str();
    else if(n<1000)
        text = "0000"+sstr.str();
    else if(n<10000)
        text = "000"+sstr.str();
    else if(n<100000)
        text = "00"+sstr.str();
    else if(n<1000000)
        text = "0"+sstr.str();
    else if(n<10000000)
        text = sstr.str();
    char* result = (char*)text.c_str();
    return result;
}

char* UI::intToCharPtr(int n)
{
    stringstream sstr;
    sstr << n;
    string text = sstr.str();
    char* result = (char*)text.c_str();
    return result;
}

//SDL Part

void UI::sdlMain()
{
    //Wystartowanie SDLa i utworzenie okna
    if( !sdlInit() )
    {
        cout << "Failed to initialize!" << endl;
    }
    else
    {
        //Wczytywanie obrazków
        if( !sdlLoadMedia() )
        {
            cout << "Failed to load media!" << endl;
        }
        else
        {
            // Ustawienie ikony okna
            SDL_SetWindowIcon(window, icon);

            //Ustawienie obecnego obrazu na pierwszy obrazek z menu
            currentSurface = menuSurfaces[ MENU_SURFACE1 ];
            while( !quit )
            {
                while( SDL_PollEvent( &event ) != 0)
                {
                    sdlSetKey();
                    //Wcisniecie X
                    if( event.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    if(selectedMenu==-1)
                    {
                        if( event.type == SDL_KEYDOWN)
                        {
                            //Wybor obrazka menu zaleznie od wcisniecia klawiszy
                            switch( event.key.keysym.sym )
                            {
                                case 'w':
                                case 'W':
                                case SDLK_UP:
                                    if(menu>0)
                                        menu--;
                                    break;
                                case 's':
                                case 'S':
                                case SDLK_DOWN:
                                    if(menu<5)
                                        menu++;
                                    break;
                                case 13:
                                    //case ' ':
                                    selectedMenu = menu;
                                    break;
                            }
                            currentSurface = menuSurfaces[menu];
                        }
                    }
                    switch(selectedMenu)
                    {
                        case 0:
                        currentSurface = promptSurface;
                        if(event.key.keysym.sym == SDLK_SPACE)
                            sdlDrawMap(game->getMap());
                        break;
                        case 1:
                        game->setToLoad(true);
                        currentSurface = promptSurface;
                        if(event.key.keysym.sym == SDLK_SPACE)
                            sdlDrawMap(game->getMap());
                        break;
                        case 2: sdlScoreboard(); selectedMenu = -1; break;
                        case 3: sdlOptionsMenu(); selectedMenu = -1; break;
                        case 4: currentSurface = authorsSurface; break;
                        case 5: quit = true; break;
                    }
                    if(event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        selectedMenu = -1;
                        currentSurface = menuSurfaces[menu];
                    }
                }

                //Wyswietlenie obecnej powierzchni
                SDL_BlitSurface( currentSurface, NULL, screen, NULL );

                //Aktualizacja powierzchni okna
                SDL_UpdateWindowSurface( window );
            }
        }
    }
    //Zwolnienie zasobow, zamkniecie SDLa
    sdlClose();
}

bool UI::sdlInit()
{
    //Flaga inicjalizacji
    bool success = true;
    //Inicjalizacja SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        cout << "Nie mozna zainicjalizowac SDL! SDL_Error: " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        //Ustawienia liniowego filtrowania tekstur
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            cout << "Uwaga: Liniowe filtrowanie tekstur nie jest wlaczone!" << endl;
        }
        //Utworzenie okna
        window = SDL_CreateWindow( "Snake", 25, 40, screenWidth, screenHeight, SDL_WINDOW_SHOWN ); // 2 i 3 argument to pozycja okna - latwiej debugowac jesli otworzysz menedzer zadan obok
        if( window == NULL )
        {
            cout << "Nie mozna utworzyc okna! SDL_Error: " << SDL_GetError() << endl;
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer == NULL)
            {
                cout << "Renderer nie zostal utworzony! SDL Error: " << SDL_GetError() << endl;
                success = false;
            }
            //Ustawienie powierzchni okna
            screen = SDL_GetWindowSurface( window );

        }
         //Inicjalizacja SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
            success = false;
        }
        //Inicjalizacja SDL_ttf
        if( TTF_Init() == -1 )
        {
            cout << "SDL_tto could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
            success = false;
        }
    }
    return success;
}

bool UI::sdlLoadMedia()
{
    //Powodzenie zaladowania plikow
    bool success = true;

    //Ikona
    icon = loadSurface( "Graphics/icon.bmp" );

    //Menu1
    menuSurfaces[ MENU_SURFACE1 ] = loadSurface( "Graphics/menu1.bmp" );
    if( menuSurfaces[ MENU_SURFACE1 ] == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Menu2
    menuSurfaces[ MENU_SURFACE2 ] = loadSurface( "Graphics/menu2.bmp" );
    if( menuSurfaces[ MENU_SURFACE2 ] == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Menu3
    menuSurfaces[ MENU_SURFACE3 ] = loadSurface( "Graphics/menu3.bmp" );
    if( menuSurfaces[ MENU_SURFACE3 ] == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Menu4
    menuSurfaces[ MENU_SURFACE4 ] = loadSurface( "Graphics/menu4.bmp" );
    if( menuSurfaces[ MENU_SURFACE4 ] == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Menu5
    menuSurfaces[ MENU_SURFACE5 ] = loadSurface( "Graphics/menu5.bmp" );
    if( menuSurfaces[ MENU_SURFACE5 ] == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Menu6
    menuSurfaces[ MENU_SURFACE6 ] = loadSurface( "Graphics/menu6.bmp" );
    if( menuSurfaces[ MENU_SURFACE6 ] == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Authors
    authorsSurface = loadSurface( "Graphics/authors.bmp" );
    if( authorsSurface == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Pause1
    pauseSurface1 = loadSurface( "Graphics/pause1.bmp" );
    if( pauseSurface1 == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Pause2
    pauseSurface2 = loadSurface( "Graphics/pause2.bmp" );
    if( pauseSurface2 == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Pause3
    pauseSurface3 = loadSurface( "Graphics/pause3.bmp" );
    if( pauseSurface3 == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Prompt
    promptSurface = loadSurface("Graphics/prompt.bmp");
    if( promptSurface == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //LevelUp
    levelUpSurface = loadSurface("Graphics/levelup.bmp");
    if( levelUpSurface == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Game Over
    gameOverSurface = loadSurface("Graphics/gameover.bmp");
    if( gameOverSurface == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Game Saved
    gameSavedSurface = loadSurface("Graphics/gamesaved.bmp");
    if( gameSavedSurface == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }

    //Move Sound
    moveSound = Mix_LoadWAV("Sounds/move.wav");
    if(moveSound == NULL)
    {
        cout << "Nie mozna zaladowac dzwieku!" << endl;
        success = false;
    }

    //Eat Sound
    eatSound = Mix_LoadWAV("Sounds/eat.wav");
    if(eatSound == NULL)
    {
        cout << "Nie mozna zaladowac dzwieku!" << endl;
        success = false;
    }

    //GameOver Sound
    gameOverSound = Mix_LoadWAV("Sounds/gameover.wav");
    if(gameOverSound == NULL)
    {
        cout << "Nie mozna zaladowac dzwieku!" << endl;
        success = false;
    }

    speedUpSound = Mix_LoadWAV("Sounds/speedup.wav");
    if(speedUpSound == NULL)
    {
        cout << "Nie mozna zaladowac dzwieku!" << endl;
        success = false;
    }

    //LevelUp Sound
    levelUpSound = Mix_LoadWAV("Sounds/levelup.wav");
    if(levelUpSound == NULL)
    {
        cout << "Nie mozna zaladowac dzwieku!" << endl;
        success = false;
    }

    //LostLive Sound
    lostLiveSound = Mix_LoadWAV("Sounds/lostlive.wav");
    if(lostLiveSound == NULL)
    {
        cout << "Nie mozna zaladowac dzwieku!" << endl;
        success = false;
    }

    //Ehsmb Font
    fontEhsmb = TTF_OpenFont("Fonts/ehsmb.ttf",72);
    if(fontEhsmb == NULL)
    {
        cout << "Nie mozna zaladowac czcionki!" << endl;
        success = false;
    }
    //Arial Font
    fontArial = TTF_OpenFont("Fonts/arial.ttf",72);
    if(fontArial == NULL)
    {
        cout << "Nie mozna zaladowac czcionki!" << endl;
        success = false;
    }

    //Lives Heart
    livesSurface = loadSurface("Graphics/heart.bmp");
    if( gameSavedSurface == NULL )
    {
        cout << "Nie mozna zaladowac obrazka!" << endl;
        success = false;
    }


    return success;
}

void UI::sdlClose()
{
    //Czyszczenie surface'y
    for( int i = 0; i < MENU_SURFACE_TOTAL; ++i )
    {
        SDL_FreeSurface(menuSurfaces[i]);
        menuSurfaces[i] = NULL;
    }
    SDL_FreeSurface(screen);
    SDL_FreeSurface(icon);
    SDL_FreeSurface(currentSurface);
    SDL_FreeSurface(authorsSurface);
    SDL_FreeSurface(pauseSurface1);
    SDL_FreeSurface(pauseSurface2);
    SDL_FreeSurface(pauseSurface3);
    SDL_FreeSurface(promptSurface);
    SDL_FreeSurface(levelUpSurface);
    SDL_FreeSurface(gameOverSurface);
    SDL_FreeSurface(gameSavedSurface);

    SDL_FreeSurface(scoreSurface);
    SDL_FreeSurface(scoreTextSurface);
    SDL_FreeSurface(livesSurface);
    SDL_FreeSurface(scoreboardSurface);
    SDL_FreeSurface(optionsTextSurface1);
    SDL_FreeSurface(optionsTextSurface2);
    SDL_FreeSurface(optionsTextSurface3);
    SDL_FreeSurface(optionsTextSurface4);
    SDL_FreeSurface(optionsPtrSurface);

    screen = NULL;
    icon = NULL;
    currentSurface = NULL;
    authorsSurface = NULL;
    pauseSurface1 = NULL;
    pauseSurface2 = NULL;
    pauseSurface3 = NULL;
    promptSurface = NULL;
    levelUpSurface = NULL;
    gameOverSurface = NULL;
    gameSavedSurface = NULL;

    scoreSurface = NULL;
    scoreTextSurface = NULL;
    livesSurface = NULL;
    scoreboardSurface = NULL;
    optionsTextSurface1 = NULL;
    optionsTextSurface2 = NULL;
    optionsTextSurface3 = NULL;
    optionsTextSurface4 = NULL;
    optionsPtrSurface = NULL;

    //Czyszczenie dzwiekow
    Mix_FreeChunk(moveSound);
    Mix_FreeChunk(eatSound);
    Mix_FreeChunk(gameOverSound);
    Mix_FreeChunk(speedUpSound);
    Mix_FreeChunk(levelUpSound);
    Mix_FreeChunk(lostLiveSound);

    moveSound = NULL;
    eatSound = NULL;
    gameOverSound = NULL;
    speedUpSound = NULL;
    levelUpSound = NULL;
    lostLiveSound = NULL;

    //Czyszczenie tekstur
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(scoreTextTexture);
    SDL_DestroyTexture(livesTexture);
    SDL_DestroyTexture(scoreboardTexture);
    SDL_DestroyTexture(optionsTextTexture1);
    SDL_DestroyTexture(optionsTextTexture2);
    SDL_DestroyTexture(optionsTextTexture3);
    SDL_DestroyTexture(optionsTextTexture4);
    SDL_DestroyTexture(optionsPtrTexture);

    scoreTexture = NULL;
    scoreTextTexture = NULL;
    livesTexture = NULL;
    scoreboardTexture = NULL;
    optionsTextTexture1 = NULL;
    optionsTextTexture2 = NULL;
    optionsTextTexture3 = NULL;
    optionsTextTexture4 = NULL;
    optionsPtrTexture = NULL;

    //Czyszczenie TTF
    TTF_CloseFont(fontEhsmb);
    TTF_CloseFont(fontArial);

    fontEhsmb = NULL;
    fontArial = NULL;

    //Niszczenie renderera i okna
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow( window );
    window = NULL;
    renderer = NULL;

    //Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

void UI::sdlSetKey()
{
    if(event.type == SDL_KEYDOWN)
    {
        sdlKey = event.key.keysym.sym;
        key = (char)sdlKey;
        key2buffer(key,buffer);
    }
}

void UI::sdlDrawMap(Map *map)
{
    while(game->game())
    {
        while( SDL_PollEvent( &event ) != 0)
        {
            sdlSetKey();
            //U¿ytkownik nacisn¹³ X
//            if( event.type == SDL_QUIT )
//            {
//                quit = true;
//            }
        }
        if(!(game->getPaused()))
        {
            //To dzia³a w tym miejscu, tylko ten break...
            if( event.type == SDL_QUIT )
            {
                quit = true;
                break;
            }
            //Wyczyszczenie ekranu
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);


            for(int i=0;i<map->getHeight();i++)
            {
                for(int j=0;j<map->getWidth();j++)
                {
                    //Obliczanie wspó³rzêdnych kolejnych kwadratów, bêd¹cych elementami mapy
                    //Zale¿nie od elementu rysowany jest kwadrat w innym kolorze
                    SDL_Rect fillRect = {(j)*((screenWidth-200)/map->getWidth()), (i)*(screenHeight/map->getHeight()), (screenWidth-200)/map->getWidth()-2, (screenHeight/map->getHeight())-2};
                    if(map->getField(j,i)==empty)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                        SDL_RenderFillRect(renderer, &fillRect);
                    }
                    if(map->getField(j,i)==snakebody)
                    {
                        if(game->getSnake()->getNextX()==j&&game->getSnake()->getNextY()==i) //&&game->getSnake()->getSnakeElem(0)->getHead==1)
                        {
                            SDL_SetRenderDrawColor(renderer, 0, 234, 0, 0);
                            SDL_RenderFillRect(renderer, &fillRect);
                        }
                        else
                        {
                            if(game->getSnake()->getGhost())
                            {
                                SDL_SetRenderDrawColor(renderer, 0, 64, 0, 0);
                                SDL_RenderFillRect(renderer, &fillRect);
                            }
                            else
                            {
                                SDL_SetRenderDrawColor(renderer, 0, 149, 0, 0);
                                SDL_RenderFillRect(renderer, &fillRect);
                            }
                        }
                    }
                    if(map->getField(j,i)==apple)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 128, 0, 0);
                        SDL_RenderFillRect(renderer, &fillRect);
                    }
                    if(map->getField(j,i)==banana)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
                        SDL_RenderFillRect(renderer, &fillRect);
                    }
                    if(map->getField(j,i)==heart)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
                        SDL_RenderFillRect(renderer, &fillRect);
                    }
                    if(map->getField(j,i)==obstacle)
                    {
                        SDL_SetRenderDrawColor(renderer, 145, 145, 145, 0);
                        SDL_RenderFillRect(renderer, &fillRect);
                    }
                }
            }
//            Wypelnienie pozostalej czesci ekranu kolorem czarnym
            SDL_Rect fillBg = {(screenWidth-200), 0, screenWidth , screenHeight};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderFillRect(renderer, &fillBg);

            //Wyœwietlenie napisu wynik
            scoreTextSurface = TTF_RenderText_Solid(fontEhsmb,"WYNIK",textColor);
            scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);
            SDL_Rect scoreRect = {(screenWidth-180),20,160,50};
            SDL_RenderCopy(renderer, scoreTextTexture, NULL, &scoreRect);

            //Wyœwietlanie wyniku
            scoreSurface = TTF_RenderText_Solid(fontEhsmb,scoreToCharPtr(game->getScore()),textColor);
            scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            SDL_Rect scoreRect2 = {(screenWidth-180),80,160,50};
            SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect2);

            //Wyœwietlanie iloœci ¿yæ
            livesTexture = SDL_CreateTextureFromSurface(renderer,livesSurface);
            for(int i=0;i<game->getSnake()->getLives();i++)
            {
                SDL_Rect livesRect = {((screenWidth-175)+((i%3)*50)),(140+(60*(i/3))),50,50};
                SDL_RenderCopy(renderer, livesTexture, NULL, &livesRect);
            }

            SDL_DestroyTexture(scoreTextTexture);
            SDL_DestroyTexture(scoreTexture);
            SDL_DestroyTexture(livesTexture);


            SDL_FreeSurface(scoreTextSurface);
            SDL_FreeSurface(scoreSurface);
            //SDL_FreeSurface(scoreTextSurface);


            //Linia oddzielaj¹ca mapê od reszty ekranu
            SDL_SetRenderDrawColor(renderer, 0, 0, 145, 0);
            SDL_RenderDrawLine(renderer, (screenWidth-200), 0, (screenWidth-200), screenHeight);

            //Aktualizacja wyswietlania
            SDL_RenderPresent(renderer);
        }
        else
        {
            sdlPauseMenu();
        }
    }
    sdlGameOver();
    selectedMenu = -1;
    currentSurface = menuSurfaces[menu];
}

void UI::sdlScoreboard()
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

    //Wyczyszczenie ekranu
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for(int i=0;i<10;i++) // ta petle wystarczylo wyjac z ponizszej petli do while, by nie przydzielalo pamieci w nieskonczonosc
    {
        scoreboardSurface = TTF_RenderText_Solid(fontEhsmb, intToCharPtr(i+1),textColor);
        scoreboardTexture = SDL_CreateTextureFromSurface(renderer, scoreboardSurface);
        SDL_Rect scoreboardRect = {((screenWidth/4)-50),(50+(i*50)),25,50};
        SDL_RenderCopy(renderer, scoreboardTexture, NULL, &scoreboardRect);
        scoreboardSurface = TTF_RenderText_Solid(fontEhsmb, scoreToCharPtr(tab[i]),textColor);
        scoreboardTexture = SDL_CreateTextureFromSurface(renderer, scoreboardSurface);
        SDL_Rect scoreboardRect2 = {(screenWidth/4),(50+(i*50)),(screenWidth/2),50};
        SDL_RenderCopy(renderer, scoreboardTexture, NULL, &scoreboardRect2);
    }

    //Aktualizacja wyswietlania
    SDL_RenderPresent(renderer);

    do
    {
        while( SDL_PollEvent( &event ) != 0)
        {
            sdlSetKey();
        }
    }
    while(sdlKey!=SDLK_ESCAPE);

}

void UI::sdlOptionsMenu()
{
    optionsOption=1;
    sdlKey='x';


    while(optionsOption!=3)
    {
        while(sdlKey!=SDLK_SPACE && sdlKey!=SDLK_RETURN && sdlKey!=SDLK_ESCAPE)
        {
            while( SDL_PollEvent( &event ) != 0)
            {
                sdlSetKey();
            }
            //Wyczyszczenie ekranu
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);

                //U¿ytkownik nacisn¹³ X
//                if( event.type == SDL_QUIT )
//                {
//                    quit = true;
//                }
            optionsPtrSurface = TTF_RenderText_Solid(fontArial, ">",textColor);
            optionsPtrTexture = SDL_CreateTextureFromSurface(renderer, optionsPtrSurface);
            if(optionsOption==1)
            {
                SDL_Rect optionsPtrRect1 = {((screenWidth/4)-50),100,25,50};
                SDL_RenderCopy(renderer, optionsPtrTexture, NULL, &optionsPtrRect1);
            }
            if(optionsOption==2)
            {
                SDL_Rect optionsPtrRect1 = {((screenWidth/4)-50),160,25,50};
                SDL_RenderCopy(renderer, optionsPtrTexture, NULL, &optionsPtrRect1);
            }
                optionsTextSurface1 = TTF_RenderText_Solid(fontArial, "Poziom trudnosci:",textColor);
                optionsTextTexture1 = SDL_CreateTextureFromSurface(renderer, optionsTextSurface1);
                SDL_Rect optionsRect1 = {((screenWidth/4)-20),100,200,50};
                SDL_RenderCopy(renderer, optionsTextTexture1, NULL, &optionsRect1);

                optionsTextSurface2 = TTF_RenderText_Solid(fontArial, intToCharPtr(game->getDifficulty()),textColor);
                optionsTextTexture2 = SDL_CreateTextureFromSurface(renderer, optionsTextSurface2);
                SDL_Rect optionsRect2 = {((screenWidth/4)-20)+210,100,25,50};
                SDL_RenderCopy(renderer, optionsTextTexture2, NULL, &optionsRect2);

                optionsTextSurface3 = TTF_RenderText_Solid(fontArial, "Dzwieki:", textColor);
                optionsTextTexture3 = SDL_CreateTextureFromSurface(renderer, optionsTextSurface3);
                SDL_Rect optionsRect3 = {((screenWidth/4)-20),160,200,50};
                SDL_RenderCopy(renderer, optionsTextTexture3, NULL, &optionsRect3);

                if(game->getSoundsEnabled())
                    optionsTextSurface4 = TTF_RenderText_Solid(fontArial, "Wlaczone", textColor);
                else
                    optionsTextSurface4 = TTF_RenderText_Solid(fontArial, "Wylaczone", textColor);
                optionsTextTexture4 = SDL_CreateTextureFromSurface(renderer, optionsTextSurface4);
                SDL_Rect optionsRect4 = {((screenWidth/4)-20)+210,160,200,50};
                SDL_RenderCopy(renderer, optionsTextTexture4, NULL, &optionsRect4);

                //czyszczenie pamiêci
                SDL_FreeSurface(optionsPtrSurface);
                SDL_FreeSurface(optionsTextSurface1);
                SDL_FreeSurface(optionsTextSurface2);
                SDL_FreeSurface(optionsTextSurface3);
                SDL_FreeSurface(optionsTextSurface4);

                SDL_DestroyTexture(optionsPtrTexture);
                SDL_DestroyTexture(optionsTextTexture1);
                SDL_DestroyTexture(optionsTextTexture2);
                SDL_DestroyTexture(optionsTextTexture3);
                SDL_DestroyTexture(optionsTextTexture4);

                switch(sdlKey)
                {
                    case SDLK_UP:
                    case 'w':
                    case 'W':
                    case SDLK_DOWN:
                    case 's':
                    case 'S': optionsOption=optionsOption%2+1; sdlKey='x'; break;
                    case SDLK_ESCAPE: optionsOption=3; break;
                }

                //Aktualizacja wyswietlania
                SDL_RenderPresent(renderer);
        }
        switch(optionsOption)
        {
            case 1: game->setDifficulty((game->getDifficulty())%3+1); sdlKey='x'; break;
            case 2: game->setSoundsEnabled(!(game->getSoundsEnabled())); sdlKey='x'; break;
            case 3: break;
        }
    }
}

void UI::sdlPauseMenu()
{
    pauseOption=3;
    currentSurface = pauseSurface3;
    sdlKey='x';
    while(pauseOption!=1)
    {
        while(sdlKey!=SDLK_SPACE&&sdlKey!=SDLK_RETURN) //SDLK_RETURN = ENTER
        {
            while( SDL_PollEvent( &event ) != 0)
            {
                sdlSetKey();
                //U¿ytkownik nacisn¹³ X
                if( event.type == SDL_QUIT )
                {
                    quit = true;
                }
                switch(pauseOption)
                {
                    case 1: currentSurface = pauseSurface1; break;
                    case 2: currentSurface = pauseSurface2; break;
                    case 3: currentSurface = pauseSurface3; break;
                }
                //chwilowe prowizoryczne menu
                switch(sdlKey)
                {
                    case 'w':
                    case 'W':
                    case SDLK_UP: pauseOption=pauseOption%3+1; sdlKey='x'; break;

                    case 's':
                    case 'S':
                    case SDLK_DOWN: pauseOption=(pauseOption+1)%3+1; sdlKey='x'; break;

                    case SDLK_ESCAPE:game->setPaused(false);sdlKey = ' ';pauseOption=3;break;
                }
            }
            SDL_BlitSurface( currentSurface, NULL, screen, NULL );
            SDL_UpdateWindowSurface( window );
        }
        switch(pauseOption)
        {
            case 1: game->getSnake()->setLives(0); sdlKey='x'; break;
            case 2:
            {
                game->save();
                sdlGameSaved();
                //sdlKey='x';
            } break;
            case 3: break;
        }
        if(event.key.keysym.sym == SDLK_ESCAPE)
        {
            pauseOption = -1;
            currentSurface = pauseSurface1;
        }
        if(pauseOption==3||pauseOption==1)
            game->setPaused(false); break;
    }
}

void UI::sdlGameSaved()
{
    currentSurface = gameSavedSurface;
    do
    {
        while( SDL_PollEvent( &event ) != 0)
        {
            sdlSetKey();
            //currentSurface = gameOverSurface;
        }
        SDL_BlitSurface( currentSurface, NULL, screen, NULL );
        SDL_UpdateWindowSurface( window );
    }
    while(sdlKey!=SDLK_ESCAPE);
}

void UI::sdlGameOver()
{
    currentSurface = gameOverSurface;
    if(game->getSoundsEnabled())
        Mix_PlayChannel(-1,gameOverSound,0);
    do
    {
        while( SDL_PollEvent( &event ) != 0)
        {
            sdlSetKey();
            //currentSurface = gameOverSurface;
        }
        SDL_BlitSurface( currentSurface, NULL, screen, NULL );
        SDL_UpdateWindowSurface( window );
    }
    while(sdlKey!=SDLK_SPACE&&sdlKey!=SDLK_RETURN);
}

void UI::sdlLevelComplete()
{
    currentSurface = levelUpSurface;
    if(game->getSoundsEnabled())
        Mix_PlayChannel(-1,levelUpSound,0);
    do
    {
        while( SDL_PollEvent( &event ) != 0)
        {
            sdlSetKey();
            //currentSurface = levelUpSurface;
        }
        SDL_BlitSurface( currentSurface, NULL, screen, NULL );
        SDL_UpdateWindowSurface( window );
    }
    while(sdlKey!=SDLK_SPACE&&sdlKey!=SDLK_RETURN);
}

void UI::sdlDisplayPrompt()
{
    currentSurface = promptSurface;
    do
    {
        while( SDL_PollEvent( &event ) != 0)
        {
            sdlSetKey();
            //currentSurface = promptSurface;
        }
        SDL_BlitSurface( currentSurface, NULL, screen, NULL );
        SDL_UpdateWindowSurface( window );
    }
    while(sdlKey!=SDLK_SPACE&&sdlKey!=SDLK_RETURN);
}

SDL_Surface* UI::loadSurface( string path )
{
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        cout << "Nie mozna zaladowac obrazka: " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
    }
    return loadedSurface;
}

void UI::sdlPlayMoveSound()
{
    Mix_PlayChannel(-1,moveSound,0);
}

void UI::sdlPlayEatSound()
{
    Mix_PlayChannel(-1,eatSound,0);
}

void UI::sdlPlayLostLiveSound()
{
    Mix_PlayChannel(-1,lostLiveSound,0);
}

void UI::sdlPlaySpeedSound()
{
    Mix_PlayChannel(-1,speedUpSound,0);
}
