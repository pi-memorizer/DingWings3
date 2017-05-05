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
	x = 0;
	y = 0;
	xOffset = 0;
	yOffset = 0;
	worldID = 0;
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 160, 144);
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
	return male;
}

void Player::draw()
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0);
	SDL_RenderClear(renderer);
	getSprite()->draw(0, 0);
}

void Player::run()
{
	GameState *state = getState();
	if (state != nullptr)
	{
		state->run();
	}
	if (keys[keyA])
		sounds[0]->play();
}