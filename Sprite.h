#ifndef SPRITE_H
#define SPRITE_H
#include "Revengine.h"
#include "Graphics.h"
using namespace std;


class Sprite
{
	Rect source;
	int xOffset = 0, yOffset = 0;
	bool hasOwnership = true;

public:
	Texture *sprite = nullptr;
	Sprite(string filename, int xOffset, int yOffset);
	Sprite(Sprite *spritesheet, int x, int y, int width, int height, int xOffset, int yOffset);
	~Sprite();

	void draw(int x, int y);
	void draw(int x, int y, int alpha);
};

extern List<Sprite*> sprites;

#endif