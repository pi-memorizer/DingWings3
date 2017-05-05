#include <iostream>
#include "Revengine.h"
using namespace std;

string *args = nullptr;
int numArgs = 0;
SoundSystem *soundSystem;
Sound *sounds[1024];

#define SPRITE Sprite
#include "SpriteList.h"
#undef SPRITE

#ifdef main
#undef main
#endif
int main(int argc, char **argv)
{
	int screenWidth = 160, screenHeight = 144;
	debug("Debugging mode active");
	bool fullscreen = true;
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

	SDL_DisplayMode displayMode;
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

	//load sprites
	male = new Sprite("guy", 0, 0);

	//add keys
	for (int i = SDLK_a; i < SDLK_z; i++)
		keys.add(i, false);
	keys.add(SDLK_F4, false);

	addPlayer(0,SDLK_d,SDLK_w,SDLK_a,SDLK_s,SDLK_j,SDLK_k);
	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_F4 && !keys[SDLK_F4])
				{
					if (fullscreen)
					{
						SDL_SetWindowFullscreen(window, 0);
						SDL_SetWindowSize(window, 4 * WIDTH, 4 * HEIGHT);
						SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
					} else {
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
					quit = true;
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
				int newHeight = (int)(width / 160.0 * 144.0);
				rect.h = newHeight;
				rect.w = width;
				rect.x = 0;
				rect.y = (height - newHeight) / 2;
			}
			else
			{
				int newWidth = (int)(height / 144.0 * 160.0);
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
	}
	quitSDL();
	return 0;
}