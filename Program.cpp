#include <iostream>
#include "Revengine.h"
#include "Blocking.h"
#include "Sprite.h"
#include "SoundSystem.h"
#include "Player.h"
#include "IO.h"
#include "Item.h"
#include "Graphics.h"
#include "Event.h"
using namespace std;

string *args = nullptr;
int numArgs = 0;

#define SPRITE Sprite
#include "SpriteList.h"
#undef SPRITE

bool fullscreen = true;
int screenWidth = WIDTH, screenHeight = HEIGHT;

#ifdef main
#undef main
#endif
int main(int argc, char **argv)
{
	debug("Debugging mode active");
	
	args = new string[argc];
	numArgs = argc;
	for (int i = 0; i < argc; i++)
	{
		args[i] = string(argv[i]);
	}

	if (startGame() != 0)
	{
		return -1;
	}

	createSound("door");

	//load sprites
	Sprite *guySheet = new Sprite("guy", 0, 0);
	guy = new Sprite*[12];
	for (int i = 0; i < 12; i++)
	{
		guy[i] = new Sprite(guySheet, 16 * (i % 4), 24 * (i / 4), 16, 24, 0, -8);
	}
	tileset = new Sprite*[65536];
	for (int i = 0; i < 65536; i++)
	{
		tileset[i] = nullptr;
	}
	for (int i = 0; i < 256; i++)
	{
		try {
			Sprite *s = new Sprite("tileset" + to_string(i+1), 0, 0);
			for (int j = 0; j < 256; j++)
			{
				tileset[i * 256 + j] = new Sprite(s, TILE_SIZE * (j % 16), TILE_SIZE * (j / 16), TILE_SIZE, TILE_SIZE, 0, 0);
			}
		}
		catch (int) {
		}
	}
	Sprite *s = new Sprite("charset", 0, 0);
	chars = new Sprite*[256];
	for (int i = 0; i < 256; i++)
	{
		chars[i] = new Sprite(s, 8 * (i % 16), 8 * (i / 16), 8, 8, 0, 0);
	}

	addPlayer(0);

	init();
	load();

	setBackgroundMusic("PossibleTheme");

	try {
		while (theLoop());
	}
	catch (ApplicationClosingException &a)
	{
		debug("Application closing exception caught");
	}
	exitGame();
	return 0;
}

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
			if (e.keys.key == KEY_FULLSCREEN && !e.keys.prevState)
			{
				if (fullscreen)
				{
					setWindowFullscreen(false);
					setWindowSize(2 * WIDTH, 2 * HEIGHT);
					centerWindow();
				}
				else {
					setWindowFullscreen(true);
					setWindowSize(getScreenWidth(), getScreenHeight());
				}
				fullscreen = !fullscreen;
			}
			if (e.keys.key == KEY_ESCAPE)
			{
				return false;
			}
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
	return true;
}