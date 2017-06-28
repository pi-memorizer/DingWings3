#include "Revengine.h"
#include "Player.h"
#include "Sprite.h"
#include "Event.h"

#ifdef MOBILE
string to_string(int value);
#endif

int frames = 0;
Hashmap<string, int> flags;
int screenWidth = WIDTH, screenHeight = HEIGHT;

bool theLoop()
{
	Event e;
	while (getEvent(&e))
	{
		if (e.type == EVENT_QUIT)
		{
			return false;
		}
		else if (e.type == EVENT_KEY_DOWN)
		{

		}
		else if (e.type == EVENT_KEY_UP)
		{

		}
	}

	setDrawColor(0, 0, 0, 0);
	clearScreen();

	int width;
	int height;
	getWindowSize(&width, &height);
	float wRat = width / (float)screenWidth;
	float hRat = height / (float)screenHeight;
	if (numPlayers == 1)
	{
		Rect rect;
		if (wRat < hRat)
		{
			int newHeight = (int)(width / (float)WIDTH * (float)HEIGHT);
			rect.h = newHeight;
			rect.w = width;
			rect.x = 0;
			rect.y = (height - newHeight) / 2;
		}
		else
		{
			int newWidth = (int)(height / (float)HEIGHT * (float)WIDTH);
			rect.w = newWidth;
			rect.h = height;
			rect.x = (width - newWidth) / 2;
			rect.y = 0;
		}
		startDrawingPlayer(0);
		players[0]->draw();
		startDrawingPlayer(-1);
		drawTexture(players[0]->texture, &rect);
	}
	display();

	for (int i = 0; i < numPlayers; i++)
	{
		players[i]->run();
	}

	delay(16);
	frames++;
	return true;
}

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