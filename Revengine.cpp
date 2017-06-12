#include "Revengine.h"
#include "Player.h"
#include "Sprite.h"

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
Hashmap<int, bool> keys;
Hashmap<string, int> flags;

int getOnscreenX(Player *p, int x, int xOffset)
{
	return WIDTH / 2 - 16 - p->xOffset + xOffset + 32 * (x - p->x);
}

int getOnscreenY(Player *p, int y, int yOffset)
{
	return HEIGHT / 2 - 16 - p->yOffset + yOffset + 32 * (y - p->y);
}

int getFlag(string s)
{
	if (flags.contains(s))
	{
		return flags[s];
	}
	else return 0;
}

void setFlag(string s, int v)
{
	if (flags.contains(s))
	{
		flags[s] = v;
	}
	else {
		flags.add(s, v);
	}
}

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