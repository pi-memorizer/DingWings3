#ifndef SPRITE_H
#define SPRITE_H
#include "Revengine.h"
using namespace std;

class Sprite
{
	SDL_Texture *sprite = nullptr;
	SDL_Rect source;
	int xOffset = 0, yOffset = 0;
	bool hasOwnership = true;

public:
	Sprite(string filename, int xOffset, int yOffset);
	Sprite(Sprite *spritesheet, int x, int y, int width, int height, int xOffset, int yOffset);
	~Sprite();

	void draw(int x, int y);
};

extern List<Sprite*> sprites;

//sprite list
#define SPRITE extern Sprite
#include "SpriteList.h"
#undef SPRITE

#endif