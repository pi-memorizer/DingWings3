#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Revengine.h"

class GameState
{
	Player *p;
public:
	GameState(Player *player);
	virtual void draw() = 0;
	virtual void run() = 0;
};

#endif