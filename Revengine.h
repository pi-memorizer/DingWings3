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

#define WIDTH 320
#define HEIGHT 240

class Sprite;
class GameState;
class Player;
class SoundSystem;
class Sound;
class World;
class WorldState;
class Entity;

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
extern Hashmap<string, int> flags;
int getFlag(string s);
void setFlag(string s, int value);
extern List<World*> worlds;
int getOnscreenX(Player *p, int x, int xOffset);
int getOnscreenY(Player *p, int y, int yOffset);
int safeDiv(int x, int factor);
void textbox(int i, string msg);
bool rectCollides(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

//File IO
void save();
void load();
void init();
unsigned char readByte(SDL_RWops*file);
unsigned short readUShort(SDL_RWops * file);
unsigned int readUInt(SDL_RWops * file);
unsigned long long readULong(SDL_RWops * file);
char readChar(SDL_RWops * file);
short readShort(SDL_RWops * file);
int readInt(SDL_RWops * file);
long long readLong(SDL_RWops * file);
string readString(SDL_RWops * file);
void writeByte(SDL_RWops *file, unsigned char v);
void writeChar(SDL_RWops * file, char v);
void writeShort(SDL_RWops * file, short v);
void writeInt(SDL_RWops * file, int v);
void writeLong(SDL_RWops * file, long long v);
void writeString(SDL_RWops * file, string v);

#include "Game.h"
#include "Sprite.h"
#include "Player.h"
#include "GameState.h"
#include "SoundSystem.h"
#include "World.h"
#include "Entity.h"

#endif