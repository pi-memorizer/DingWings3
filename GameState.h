#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Revengine.h"

class Animation;

class GameState
{
protected:
	Player *p;
	bool a, b, up, down, left, right;
public:
	List<Animation*> animations;
	GameState(Player *player);
	virtual void draw() = 0;
	virtual void run() = 0;
	void startMenu();
	void endMenu();
};

class WorldState : public GameState
{
public:
	WorldState(Player *player);
	virtual void draw();
	virtual void run();
};

#endif