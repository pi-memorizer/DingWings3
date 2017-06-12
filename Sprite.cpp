#include "Sprite.h"

Sprite::Sprite(string filename, int xOffset, int yOffset)
{
	sprite = IMG_LoadTexture(renderer, ("image/"+filename+".png").c_str());
	if (sprite != nullptr)
	{
		SDL_QueryTexture(sprite, nullptr, nullptr, &source.w, &source.h);
		source.x = 0;
		source.y = 0;
		this->xOffset = xOffset;
		this->yOffset = yOffset;
		this->hasOwnership = true;
		sprites.add(this);
	}
	else {
		throw - 1;
	}
}

Sprite::Sprite(Sprite *spritesheet, int x, int y, int width, int height, int xOffset, int yOffset)
{
	sprite = spritesheet->sprite;
	this->hasOwnership = false;
	source.h = height;
	source.w = width;
	source.x = x;
	source.y = y;
	this->xOffset = xOffset;
	this->yOffset = yOffset;
	sprites.add(this);
}

Sprite::~Sprite()
{
	if (hasOwnership)
	{
		SDL_DestroyTexture(sprite);
	}
}

void Sprite::draw(int x, int y)
{
	SDL_Rect d;
	if (this == nullptr)
	{
		cout << "wat" << endl;
		return;
	}
	d.h = source.h;
	d.w = source.w;
	d.x = x + xOffset;
	d.y = y + yOffset;
	SDL_RenderCopy(renderer, sprite, &source, &d);
}

List<Sprite*> sprites;