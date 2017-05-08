#include "World.h"

List<World*> worlds;

void World::addEntity(Entity *e)
{
	e->world = this;
	entities.add(e);
}

StaticWorld::StaticWorld(string filename, void(*run)(), bool(*interact)(Player*, int, int))
{
	_run = run;
	_interact = interact;
	SDL_RWops *file = SDL_RWFromFile(("map/" + filename + ".rmap").c_str(), "r+b");
	if (file != nullptr)
	{
		width = readInt(file);
		height = readInt(file);
		xOffset = readInt(file);
		yOffset = readInt(file);
		map = new StaticBlock[width*height];
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				int i = x+y*width;
				map[i].lower = readUShort(file);
				map[i].upper = readUShort(file);
				map[i].effect = readByte(file);
				map[i].collision = readByte(file);
			}
		}
		SDL_RWclose(file);
		debug("Read map " + filename);
		debug(width << ", " << height << ", " << xOffset << ", " << yOffset);
	}
	else {
		debug("Error loading map file " + filename);
		map = nullptr;
		width = 0;
		height = 0;
		xOffset = 0;
		yOffset = 0;
	}
}

void StaticWorld::run()
{
	_run();
}

bool StaticWorld::interact(Player *p, int x, int y)
{
	return _interact(p, x, y);
}

bool rectCollides(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	if (y1 + height1 <= y2) return false;
	if (y1 >= y2 + height2) return false;
	if (x1 + width1 <= x2) return false;
	if (x1 >= x2 + width2) return false;
	return true;
}

int safeDiv(int x, int factor)
{
	if (x >= 0) return x / factor;
	//return ((-1 * x) / factor) * -1 - 1;
	return (x+1) / factor - 1;
}

bool StaticWorld::subCollision(int x, int y, int px, int py, int pwidth, int pheight)
{
	int c = getCollision(x, y);
	if (c == 0) return false;
	if (c <= 16)
	{
		return rectCollides(px, py, pwidth, pheight, x * 32 + 16 - c, y * 32 + 16 - c, 2 * c, 2 * c);
	}
	return false;
}

bool StaticWorld::collides(int x, int y, int xOffset, int yOffset, int width, int height)
{
	assert(width > 0);
	assert(height > 0);
	int _x = x * 32 + xOffset;
	int _y = y * 32 + yOffset;
	for (int cx = safeDiv(_x, 32); cx <= safeDiv(_x + width, 32); cx++)
	{
		for (int cy = safeDiv(_y, 32); cy <= safeDiv(_y + height, 32); cy++)
		{
			if (subCollision(cx, cy, _x, _y, width, height)) return true;
		}
	}
	return false;
}

int StaticWorld::getLower(int x, int y)
{
	if (x >= xOffset&&y >= yOffset&&x < xOffset + width&&y < yOffset + height)
	{
		int v = map[(x - xOffset) + (y - yOffset)*width].lower;
		if (v != 0) debug(v);
		return v;
	}
	else return 0;
}

int StaticWorld::getUpper(int x, int y)
{
	if (x >= xOffset&&y >= yOffset&&x < xOffset + width&&y < yOffset + height)
	{
		return map[(x - xOffset) + (y - yOffset)*width].upper;
	}
	else return 0;
}

int StaticWorld::getEffect(int x, int y)
{
	if (x >= xOffset&&y >= yOffset&&x < xOffset + width&&y < yOffset + height)
	{
		return map[(x - xOffset) + (y - yOffset)*width].effect;
	}
	else return 0;
}

int StaticWorld::getCollision(int x, int y)
{
	if (x >= xOffset&&y >= yOffset&&x < xOffset + width&&y < yOffset + height)
	{
		return map[(x - xOffset) + (y - yOffset)*width].collision;
	}
	else return 0;
}