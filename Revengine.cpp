#include "Revengine.h"

List<Sprite*> sprites;
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
Hashmap<int, bool> keys;
Player **players = new Player*[0];
int numPlayers = 0;

void quitSDL()
{
	if (window != nullptr) SDL_DestroyWindow(window);
	if (renderer != nullptr) SDL_DestroyRenderer(renderer);
	for (int i = 0; i < sprites.length(); i++)
	{
		delete sprites[i];
	}
	for (int i = 0; i < numPlayers; i++)
	{
		delete players[i];
	}
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

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
		buffer[numPlayers] = new Player(id,keyRight,keyUp,keyLeft,keyDown,keyA,keyB);
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