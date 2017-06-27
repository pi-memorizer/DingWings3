#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Revengine.h"

extern Sprite **chars;

class GameState
{
protected:
	Player *p;
	bool a, b, up, down, left, right;
public:
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

class TextBox : public GameState
{
	string msg;
	GameState *caller;
	int count = 0;
public:
	TextBox(Player *p, string _msg);
	void run();
	void draw();
};

#endif