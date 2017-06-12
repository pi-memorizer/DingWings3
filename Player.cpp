#include "Player.h"
#include "GameState.h"
#include "Sprite.h"
#include "Item.h"

Player **players = new Player*[0];
int numPlayers = 0;

Player *getPlayer(int id)
{
	int index = -1;
	for (int i = 0; i < numPlayers; i++)
	{
		if (players[i]->id == id)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
		return nullptr;
	else
		return players[index];
}

void addPlayer(int id, int keyRight, int keyUp, int keyLeft, int keyDown, int keyA, int keyB)
{
	int index = -1;
	for (int i = 0; i < numPlayers; i++)
	{
		if (players[i]->id == id)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		Player **buffer = new Player*[numPlayers + 1];
		for (int i = 0; i < numPlayers; i++)
		{
			buffer[i] = players[i];
		}
		buffer[numPlayers] = new Player(id, keyRight, keyUp, keyLeft, keyDown, keyA, keyB);
		delete[] players;
		players = buffer;
		numPlayers++;
	}
}

void removePlayer(int id)
{
	int index = -1;
	for (int i = 0; i < numPlayers; i++)
	{
		if (players[i]->id == id)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		delete players[index];
		Player **buffer = new Player*[numPlayers - 1];
		for (int i = 0, j = 0; i < numPlayers; i++)
		{
			if (i != index)
			{
				buffer[j] = players[i];
				j++;
			}
		}
		delete[] players;
		players = buffer;
		numPlayers--;
	}
}

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
	for (int i = 0; i < INVENTORY_SLOTS; i++)
	{
		inventory[i].item = nullptr;
		inventory[i].number = 0;
	}
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

bool Player::hasItem(Item *item)
{
	for (int i = 0; i < INVENTORY_SLOTS; i++)
	{
		if (item == inventory[i].item)
		{
			return true;
		}
	}
	return false;
}
bool Player::hasItem(unsigned long long flag)
{
	for (int i = 0; i < INVENTORY_SLOTS; i++)
	{
		if (inventory[i].item->containsFlag(flag))
		{
			return true;
		}
	}
	return false;
}
ItemNumberPair* Player::getItem(Item *item)
{
	int start = abs(rand()) % INVENTORY_SLOTS;
	int i = start;
	do {
		if (inventory[i].item == item)
		{
			return &inventory[i];
		}
		i++;
		if (i >= INVENTORY_SLOTS)
		{
			i = 0;
		}
	} while (i != start);
	return nullptr;
}

ItemNumberPair* Player::getItem(unsigned long long category)
{
	int start = abs(rand()) % INVENTORY_SLOTS;
	int i = start;
	do {
		if (inventory[i].item!=nullptr&&(inventory[i].item->flags&category)== category)
		{
			return &inventory[i];
		}
		i++;
		if (i >= INVENTORY_SLOTS)
		{
			i = 0;
		}
	} while (i != start);
	return nullptr;
}

int Player::itemCount(Item *item)
{
	int count = 0;
	for (int i = 0; i < INVENTORY_SLOTS; i++)
	{
		if (inventory[i].item == item)
		{
			count += inventory[i].number;
		}
	}
	return count;
}