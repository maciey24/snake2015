# snake2015
gra snake pobi c++ sdl programowanie obiektowe politechnika łódzka ftims

tldr - skrócona wersja instrukcji:
-sklonuj repo
-pobierz 
	SDL2-2.0.5
	SDL2_mixer-2.0.1
	SDL2_ttf-2.0.14
do C:/SDL/
-wklej
	"C:\SDL\SDL2_mixer-2.0.1\i686-w64-mingw32\bin\SDL2_mixer.dll"
	"C:\SDL\SDL2_ttf-2.0.14\i686-w64-mingw32\bin\libfreetype-6.dll"
	"C:\SDL\SDL2_ttf-2.0.14\i686-w64-mingw32\bin\SDL2_ttf.dll"
	"C:\SDL\SDL2-2.0.5\i686-w64-mingw32\bin\SDL2.dll"
do katalogu projektu (repo)
-powinno działać

-------------------------------------------------------------------------------

konfiguracja IDE codeblocks z sdl:

mamy ściągnięte:
SDL-1.2.15
SDL2-2.0.5
SDL2_mixer-2.0.1
SDL2_ttf-2.0.14

umieszczamy to wszystko w katalogu C:/SDL/

potrzebne są w katalogu programu (w katalogu projektu środowiska IDE, lub w katalogu lokalizacji snake.exe) dodatkowo takie pliki:
(
z plików dll wystarczą chyba tylko te: - w katalogu projektu ide, lub w katalogu lokalizacji exe
libfreetype-6.dll
SDL2.dll
SDL2_mixer.dll
SDL2_ttf.dll
)

Fonts
Graphics
libFLAC-8.dll
libfreetype-6.dll
libmikmod-2.dll
libmodplug-1.dll
libogg-0.dll
libvorbis-0.dll
libvorbisfile-3.dll
SDL2.dll
SDL2_mixer.dll
SDL2_ttf.dll
smpeg2.dll
Sounds
zlib1.dll

całe drzewo plików:
C:\Users\maciek\programy\codeblocks\snake1\data2>tree /f
Folder PATH listing for volume Windows
C:.
│   libFLAC-8.dll
│   libfreetype-6.dll
│   libmikmod-2.dll
│   libmodplug-1.dll
│   libogg-0.dll
│   libvorbis-0.dll
│   libvorbisfile-3.dll
│   SDL2.dll
│   SDL2_mixer.dll
│   SDL2_ttf.dll
│   smpeg2.dll
│   zlib1.dll
│
├───Fonts
│       arial.ttf
│       ehsmb.ttf
│
├───Graphics
│       authors.bmp
│       gameover.bmp
│       gamesaved.bmp
│       heart.bmp
│       levelup.bmp
│       menu1.bmp
│       menu2.bmp
│       menu3.bmp
│       menu4.bmp
│       menu5.bmp
│       menu6.bmp
│       pause1.bmp
│       pause2.bmp
│       pause3.bmp
│       prompt.bmp
│
└───Sounds
        eat.wav
        gameover.wav
        levelup.wav
        lostlive.wav
        move.wav

(Nowy projekt stworzony jako console application / empty - chyba)

Ustawić build options projektu:
Search directories: 
Compiler: 
podajemy ścieżki do katalogu SDL2, który jest w katalogu include, który jest w katalogu i686-mingw-32, który jest w katalogach SDL2-* (sdl, sdl mixer, i sdl ttf)

build options projektu:
Search directories: 
Linker: 
podajemy ścieżki do katalogu lib, który jest w katalogu i686-mingw-32, który jest w katalogach SDL2-* (sdl, sdl mixer, i sdl ttf)

other linker settings:
-lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf
-
w ustawieniach IDE ustawieniach kompiilatora nie trzeba nic zmieniać, wystarczy ustawić w build options projektu.

powinno działać