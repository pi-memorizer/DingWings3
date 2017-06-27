#ifndef UNIX
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#else
#include "SDL/SDL.H"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#endif

#include "Revengine.h"
#include "Event.h"
#include "Player.h"
#include "Sprite.h"
#include "Graphics.h"

SDL_Window *window;
SDL_Renderer *renderer;
Hashmap<int, bool> keys;
SDL_DisplayMode displayMode;
Hashmap<string, Mix_Chunk*> sounds;
Mix_Music *music;
string song;

class File
{
public:
	SDL_RWops *file;
};

class Texture
{
public:
	SDL_Texture *texture;
};

void setDrawColor(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void clearScreen()
{
	SDL_RenderClear(renderer);
}

void getWindowSize(int *w, int *h)
{
	SDL_GetWindowSize(window, w, h);
}

void display()
{
	SDL_RenderPresent(renderer);
}

void startDrawingPlayer(int index)
{
	if (index == -1)
	{
		SDL_SetRenderTarget(renderer, nullptr);
	}
	else {
		SDL_SetRenderTarget(renderer, players[index]->texture->texture);
	}
}

void setWindowFullscreen(bool b)
{
	SDL_SetWindowFullscreen(window, b ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void setWindowSize(int width, int height)
{
	SDL_SetWindowSize(window, width, height);
}

void centerWindow()
{
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

int getScreenWidth()
{
	return displayMode.w;
}

int getScreenHeight()
{
	return displayMode.h;
}

void drawTexture(Texture *texture, Rect *rect)
{
	SDL_Rect r;
	r.w = rect->w;
	r.h = rect->h;
	r.x = rect->x;
	r.y = rect->y;
	SDL_RenderCopy(renderer, texture->texture, nullptr, &r);
}

void drawTexture(Texture *texture, Rect *source, Rect *dest)
{
	SDL_Rect s, d;
	s.w = source->w;
	s.h = source->h;
	s.x = source->x;
	s.y = source->y;
	d.w = dest->w;
	d.h = dest->h;
	d.x = dest->x;
	d.y = dest->y;
	SDL_RenderCopy(renderer, texture->texture, &s, &d);
}

Texture *createTexture(int width, int height)
{
	Texture * t = new Texture();
	t->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width, height);
	return t;
}

Texture *loadTexture(string filename)
{
	Texture * t = new Texture();
	t->texture = IMG_LoadTexture(renderer, ("image/" + filename + ".png").c_str());
	return t;
}

void getTextureSize(Texture *t, int *w, int *h)
{
	SDL_QueryTexture(t->texture, nullptr, nullptr, w, h);
}

void destroyTexture(Texture *texture)
{
	SDL_DestroyTexture(texture->texture);
	delete texture;
}
void fillRect(Rect *rect)
{
	SDL_Rect r;
	r.w = rect->w;
	r.h = rect->h;
	r.x = rect->x;
	r.y = rect->y;
	SDL_RenderFillRect(renderer, &r);
}

void delay(int millis)
{
	SDL_Delay(millis);
}

int startGame()
{
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		return -2;
	}
	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		exitGame();
		return -3;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)<0)
	{
		exitGame();
		return -4;
	}
	SDL_GetDesktopDisplayMode(0, &displayMode);
	window = SDL_CreateWindow("Revengine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (window == nullptr)
	{
		exitGame();
		return -1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		exitGame();
		return -1;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	for (int i = SDLK_a; i <= SDLK_z; i++)
		keys.add(i, false);
	keys.add(SDLK_F4, false);
	keys.add(SDLK_F11, false);
	keys.add(SDLK_ESCAPE, false);
	return 0;
}

void exitGame()
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
	if (Mix_PlayingMusic() == 1)
	{
		Mix_HaltMusic();
	}
	Mix_FreeMusic(music);
	music = nullptr;
	List<Mix_Chunk*> soundList = sounds.values();
	for (int i = 0; i < soundList.length(); i++)
	{
		Mix_FreeChunk(soundList[i]);
	}
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

File * openFile(string s, bool writing)
{
	File * file = new File();
	file->file = nullptr;
	if (writing)
	{
		file->file = SDL_RWFromFile(s.c_str(), "w+b");
	}
	else {
		file->file = SDL_RWFromFile(s.c_str(), "r+b");
	}
	if (file->file == nullptr)
	{
		delete file;
		return nullptr;
	}
	return file;
}

void closeFile(File *file)
{
	SDL_RWclose(file->file);
	delete file;
}

unsigned char readByte(File * file)
{
	unsigned char c;
	SDL_RWread(file->file, &c, sizeof(unsigned char), 1);
	return c;
}
char readChar(File * file)
{
	char c;
	SDL_RWread(file->file, &c, sizeof(char), 1);
	return c;
}
short readShort(File * file)
{
	return SDL_ReadBE16(file->file);
}
int readInt(File * file)
{
	return SDL_ReadBE32(file->file);
}
long long readLong(File * file)
{
	return SDL_ReadBE64(file->file);
}
unsigned short readUShort(File * file)
{
	return SDL_ReadBE16(file->file);
}
unsigned int readUInt(File * file)
{
	return SDL_ReadBE32(file->file);
}
unsigned long long readULong(File * file)
{
	return SDL_ReadBE64(file->file);
}
string readString(File * file)
{
	int length = readShort(file);
	char * c = new char[length + 1];
	c[length] = 0;
	SDL_RWread(file->file, c, length, 1);
	string s(c);
	delete[] c;
	return s;
}
void writeByte(File * file, unsigned char v)
{
	SDL_RWwrite(file->file, &v, sizeof(unsigned char), 1);
}
void writeChar(File * file, char v)
{
	SDL_RWwrite(file->file, &v, sizeof(char), 1);
}
void writeShort(File * file, short v)
{
	SDL_WriteBE16(file->file, v);
}
void writeInt(File * file, int v)
{
	SDL_WriteBE32(file->file, v);
}
void writeLong(File * file, long long v)
{
	SDL_WriteBE64(file->file, v);
}
void writeString(File * file, string v)
{
	writeShort(file, (short)v.length());
	SDL_RWwrite(file->file, v.c_str(), v.length(), 1);
}

//Key press related stuff

unsigned int keySymPairs[] = {
	SDLK_ESCAPE,KEY_ESCAPE,
	SDLK_F11,KEY_FULLSCREEN,
	SDLK_j,KEY_A,
	SDLK_k,KEY_B,
	SDLK_w,KEY_UP,
	SDLK_a,KEY_LEFT,
	SDLK_s,KEY_DOWN,
	SDLK_d,KEY_RIGHT,
	0,0
};

bool fullscreen = true;

bool getEvent(Event *e)
{
	SDL_Event sdl;
	if (SDL_PollEvent(&sdl))
	{
		if (sdl.type == SDL_QUIT)
		{
			e->type = EVENT_QUIT;
		}
		else if (sdl.type==SDL_KEYDOWN&&!sdl.key.repeat)
		{
			e->type = EVENT_KEY_DOWN;
			if (keys.contains(sdl.key.keysym.sym))
			{
				e->keys.prevState = keys[sdl.key.keysym.sym];
				keys[sdl.key.keysym.sym] = true;
				e->keys.key = KEY_UNKNOWN;
				for (int i = 0; keySymPairs[i] != 0; i += 2)
				{
					if (keySymPairs[i] == sdl.key.keysym.sym)
					{
						e->keys.key = (EventKey)keySymPairs[i + 1];
						break;
					}
				}
				if (e->keys.key == KEY_FULLSCREEN && !e->keys.prevState)
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
				if (e->keys.key == KEY_ESCAPE)
				{
					e->type = EVENT_QUIT;
				}
			}
			else {
				e->keys.key = KEY_UNKNOWN;
				e->keys.prevState = false;
			}
		}
		else if (sdl.type==SDL_KEYUP&&!sdl.key.repeat)
		{
			e->type = EVENT_KEY_UP;
			if (keys.contains(sdl.key.keysym.sym))
			{
				e->keys.prevState = keys[sdl.key.keysym.sym];
				keys[sdl.key.keysym.sym] = false;
				e->keys.key = KEY_UNKNOWN;
				for (int i = 0; keySymPairs[i] != 0; i += 2)
				{
					if (keySymPairs[i] == sdl.key.keysym.sym)
					{
						e->keys.key = (EventKey)keySymPairs[i + 1];
						break;
					}
				}
			}
			else {
				e->keys.key = KEY_UNKNOWN;
				e->keys.prevState = false;
			}
		}
		else {
			e->type = EVENT_UNKNOWN;
		}
		return true;
	}
	else {
		e->type = EVENT_UNKNOWN;
		return false;
	}
}

bool getKey(Player *p, EventKey key)
{
	if (p->id == 0)
	{
		switch (key)
		{
		case KEY_FULLSCREEN: return keys[SDLK_F11];
		case KEY_A: return keys[SDLK_j];
		case KEY_B: return keys[SDLK_k];
		case KEY_UP: return keys[SDLK_w];
		case KEY_DOWN: return keys[SDLK_s];
		case KEY_LEFT: return keys[SDLK_a];
		case KEY_RIGHT: return keys[SDLK_d];
		case KEY_ESCAPE: return keys[SDLK_ESCAPE];
		}
	}
	return false;
}

void createSound(string name)
{
	sounds.add(name, Mix_LoadWAV(("sound/" + name + ".wav").c_str()));
}

void createSong(string name)
{

}

void setBackgroundMusic(string name)
{
	if (song != name)
	{
		if (Mix_PlayingMusic() == 1)
		{
			Mix_HaltMusic();
		}
		if (music != nullptr)
		{
			Mix_FreeMusic(music);
		}
		music = Mix_LoadMUS(("sound/" + name + ".wav").c_str());
		song = name;
		if (music != nullptr)
		{
			Mix_PlayMusic(music, -1);
		}
	}
}

void playSound(string name)
{
	if (sounds[name] != nullptr)
	{
		Mix_PlayChannel(-1, sounds[name], 0);
	}
}