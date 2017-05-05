#ifndef REVENGINE_H
#define REVENGINE_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
using namespace std;

#define DEBUG true

#if DEBUG == true
#define assert(a) if(!(a)) throw "Assertion failed";
#define debug(a) cout << a << endl;
#else
#define assert(a) ;;
#define debug(a) ;;
#endif

void quitSDL();
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern string *args;
extern int numArgs;

#define WIDTH 160
#define HEIGHT 144

class Sprite;
class GameState;
class Player;
class SoundSystem;
class Sound;

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

//sprite list
#define SPRITE extern Sprite
#include "SpriteList.h"
#undef SPRITE

#include "Collections.h"
extern List<Sprite*> sprites;
extern Hashmap<int, bool> keys;
Player* getPlayer(int id);
void addPlayer(int id, int keyRight, int keyUp, int keyLeft, int keyDown, int keyA, int keyB);
void removePlayer(int id);
extern int numPlayers;
extern Player **players;
extern SoundSystem *soundSystem;
extern Sound *sounds[];

#include "Game.h"
#include "Sprite.h"
#include "Player.h"
#include "GameState.h"
#include "SoundSystem.h"

#endif