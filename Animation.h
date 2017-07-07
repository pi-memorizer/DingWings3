#ifndef ANIMATION_H
#define ANIMATION_H

#include "Player.h"
#include "GameState.h"

class Animation : public GameState
{
public:
	Animation(Player *p);
	virtual bool isDone() = 0;
	virtual void draw() = 0;
	virtual void run() = 0;
};

void bAnimation(Player *p, Animation *animation);
void nbAnimation(Player *p, Animation *animation);

#endif