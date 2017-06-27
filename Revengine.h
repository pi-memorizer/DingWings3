#ifndef REVENGINE_H
#define REVENGINE_H

#define DEBUG true
//#define UNIX true

#include <string>
#include "Collections.h"
using namespace std;

#if DEBUG == true
	#include <iostream>
	using namespace std;
	#define assert(a) if(!(a)) throw "Assertion failed";
	#define debug(a) cout << a << endl;
#else
	#define assert(a) ;;
	#define debug(a) ;;
#endif

void delay(int millis);
int startGame();
void exitGame();
extern string *args;
extern int numArgs;

#define WIDTH 320
#define HEIGHT 240
#define TILE_SIZE 32

class Sprite;
class GameState;
class Textbox;
class Player;
class SoundSystem;
class Sound;
class World;
class WorldState;
class Entity;
class Item;

extern Hashmap<string, int> flags;
int getFlag(string s);
void setFlag(string s, int value);
extern List<World*> worlds;
int getOnscreenX(Player *p, int x, int xOffset);
int getOnscreenY(Player *p, int y, int yOffset);
int safeDiv(int x, int factor);
bool rectCollides(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
bool theLoop();
void init();
#endif