#include "Player.h"

Player::Player(int id, int keyRight, int keyUp, int keyLeft, int keyDown, int keyA, int keyB)
{
	this->id = id;
	this->keyUp = keyUp;
	this->keyDown = keyDown;
	this->keyLeft = keyLeft;
	this->keyRight = keyRight;
	this->keyA = keyA;
	this->keyB = keyB;
	x = -2;
	y = -2;
	xOffset = 0;
	yOffset = 0;
	worldID = 0;
	width = 16;
	height = 16;
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
	pushState(new WorldState(this));
}

GameState *Player::getState()
{
	if (states.length() > 0)
		return states.peek();
	else
		return nullptr;
}

void Player::pushState(GameState *state)
{
	states.push(state);
}

void Player::popState()
{
	if(states.length()>0)
		delete states.pop();
}

void Player::setState(GameState *state)
{
	while (states.length() > 0)
	{
		delete states.pop();
	}
	states.push(state);
}

Sprite *Player::getSprite()
{
	int i = dir;
	if ((wait / 10) % 2 == 1) i += 4;
	if ((dir == 1 || dir == 3) && (wait / 10) % 4 == 3) i += 4;
	return guy[i];
}

void Player::draw()
{
	GameState *state = getState();
	if (state != nullptr)
	{
		state->draw();
	}
	
}

void Player::run()
{
	GameState *state = getState();
	if (state != nullptr)
	{
		state->run();
	}
	//if (keys[keyA])
		//sounds[0]->play();
}