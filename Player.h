#ifndef PLAYER_H
#define PLAYER_H
#include "Revengine.h"
#include "Graphics.h"

#define INVENTORY_SLOTS 3

extern Sprite ** guy;

struct ItemNumberPair
{
	Item *item;
	int number;
};

class Player
{
	Stack<GameState*> states;
	int worldID;
	bool cameraLocked = true;
public:
	friend int getOnscreenX(Player *p, int x);
	friend int getOnscreenY(Player *p, int y);
	int cameraX, cameraY;
	ItemNumberPair inventory[INVENTORY_SLOTS];
	int width, height;
	int dir = 3; int wait = 0;
	Texture *texture;
	int x, y;
	int id;
	Player(int id);
	GameState *getState();
	void pushState(GameState *state);
	void popState();
	void setState(GameState *state);
	Sprite *getSprite();
	void draw();
	void run();
	bool hasItem(Item *item);
	bool hasItem(unsigned long long flag);
	ItemNumberPair* getItem(Item *item);
	ItemNumberPair* getItem(unsigned long long flag);
	int itemCount(Item *item);
	int getWorldID();
	void setWorldID(int id);
	void lockCamera();
	void unlockCamera();
	int getCameraCenterX();
	int getCameraCenterY();
};

const int MAX_PLAYERS = 4;
Player* getPlayer(int id);
void addPlayer(int id);
void removePlayer(int id);
extern int numPlayers;
extern Player **players;

#endif