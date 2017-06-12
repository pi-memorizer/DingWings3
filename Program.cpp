#include <iostream>
#include "Revengine.h"
#include "Blocking.h"
#include "Sprite.h"
#include "SoundSystem.h"
#include "Player.h"
#include "IO.h"
#include "Item.h"
using namespace std;

string *args = nullptr;
int numArgs = 0;

#define SPRITE Sprite
#include "SpriteList.h"
#undef SPRITE

bool fullscreen = true;
int screenWidth = WIDTH, screenHeight = HEIGHT;
SDL_DisplayMode displayMode;

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
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		return -2;
	}
	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		quitSDL();
		return -3;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)<0)
	{
		quitSDL();
		return -4;
	}

	soundSystem = new SoundSystem();
	sounds[0] = new Sound("door");

	SDL_GetDesktopDisplayMode(0, &displayMode);
	window = SDL_CreateWindow("Revengine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (window == nullptr)
	{
		quitSDL();
		return -1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		quitSDL();
		return -1;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

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
				tileset[i * 256 + j] = new Sprite(s, 32 * (j % 16), 32 * (j / 16), 32, 32, 0, 0);
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

	//add keys
	for (int i = SDLK_a; i < SDLK_z; i++)
		keys.add(i, false);
	keys.add(SDLK_F4, false);

	addPlayer(0,SDLK_d,SDLK_w,SDLK_a,SDLK_s,SDLK_j,SDLK_k);

	addItems();
	init();
	load();

	try {
		while (theLoop());
	}
	catch (ApplicationClosingException &a)
	{
		debug("Application closing exception caught");
	}
	quitSDL();
	return 0;
}

bool theLoop()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_F4 && !keys[SDLK_F4])
			{
				if (fullscreen)
				{
					SDL_SetWindowFullscreen(window, 0);
					SDL_SetWindowSize(window, 2 * WIDTH, 2 * HEIGHT);
					SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				}
				else {
					SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
					SDL_SetWindowSize(window, displayMode.w, displayMode.h);
				}
				fullscreen = !fullscreen;
			}
			if (keys.contains(e.key.keysym.sym))
			{
				keys[e.key.keysym.sym] = true;
			}
			if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				return false;
			}
		}
		else if (e.type == SDL_KEYUP)
		{
			if (keys.contains(e.key.keysym.sym))
			{
				keys[e.key.keysym.sym] = false;
			}
		}
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	int width;
	int height;
	SDL_GetWindowSize(window, &width, &height);
	float wRat = width / (float)screenWidth;
	float hRat = height / (float)screenHeight;
	if (numPlayers == 1)
	{
		SDL_Rect rect;
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
		SDL_SetRenderTarget(renderer, players[0]->texture);
		players[0]->draw();
		SDL_SetRenderTarget(renderer, nullptr);
		SDL_RenderCopy(renderer, players[0]->texture, nullptr, &rect);
	}
	SDL_RenderPresent(renderer);

	for (int i = 0; i < numPlayers; i++)
	{
		players[i]->run();
	}

	SDL_Delay(16);
	return true;
}