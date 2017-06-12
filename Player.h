#ifndef PLAYER_H
#define PLAYER_H
#include "Revengine.h"

#define INVENTORY_SLOTS 3

struct ItemNumberPair
{
	Item *item;
	int number;
};

class Player
{
	Stack<GameState*> states;
public:
	ItemNumberPair inventory[INVENTORY_SLOTS];
	int width, height;
	int dir = 3; int wait = 0;
	int keyUp, keyDown, keyLeft, keyRight, keyA, keyB;
	SDL_Texture *texture;
	int x, y, xOffset, yOffset;
	int id;
	int worldID;
	Player(int id, int keyRight, int keyUp, int keyLeft, int keyDown, int keyA, int keyB);
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
};

Player* getPlayer(int id);
void addPlayer(int id, int keyRight, int keyUp, int keyLeft, int keyDown, int keyA, int keyB);
void removePlayer(int id);
extern int numPlayers;
extern Player **players;

#endif