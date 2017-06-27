#include "Revengine.h"
#include "Player.h"
#include "Sprite.h"

Hashmap<string, int> flags;

int getOnscreenX(Player *p, int x, int xOffset)
{
	return WIDTH / 2 - TILE_SIZE/2 - p->xOffset + xOffset + TILE_SIZE * (x - p->x);
}

int getOnscreenY(Player *p, int y, int yOffset)
{
	return HEIGHT / 2 - TILE_SIZE/2 - p->yOffset + yOffset + TILE_SIZE * (y - p->y);
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