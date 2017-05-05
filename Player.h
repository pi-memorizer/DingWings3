#ifndef PLAYER_H
#define PLAYER_H
#include "Revengine.h"

class Player
{
	Stack<GameState*> states;
	int dir = 3; int wait = 0;
public:
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
};

#endif