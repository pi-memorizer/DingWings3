#ifndef SPRITE_H
#define SPRITE_H
#include "Revengine.h"
#include "Graphics.h"
using namespace std;


class Sprite
{
	Texture *sprite = nullptr;
	Rect source;
	int xOffset = 0, yOffset = 0;
	bool hasOwnership = true;

public:
	Sprite(string filename, int xOffset, int yOffset);
	Sprite(Sprite *spritesheet, int x, int y, int width, int height, int xOffset, int yOffset);
	~Sprite();

	void draw(int x, int y);
};

extern List<Sprite*> sprites;

#endif