#ifndef WORLD_H
#define WORLD_H
#include "Revengine.h"

class World
{
public:
	List<Entity*> entities;
	void virtual run() = 0;
	bool virtual interact(Player *p, int x, int y) = 0;
	bool virtual collides(int x, int y, int xOffset, int yOffset, int width, int height) = 0;
	int virtual getLower(int x, int y) = 0;
	int virtual getUpper(int x, int y) = 0;
	int virtual getEffect(int x, int y) = 0;
	int virtual getCollision(int x, int y) = 0;
	void addEntity(Entity *e);
};

struct StaticBlock
{
	unsigned short lower, upper;
	unsigned char effect, collision;

	StaticBlock(int _lower, int _upper, int _effect, int _collision)
	{
		lower = (unsigned short)_lower;
		upper = (unsigned short)_upper;
		effect = (unsigned char)_effect;
		collision = (unsigned char)_collision;
	}
	StaticBlock() {}
};

class StaticWorld : public World
{
private:
	void(*_run)();
	bool(*_interact)(Player*,int,int);
	int width, height, xOffset, yOffset;
	StaticBlock *map = nullptr;
	bool subCollision(int x, int y, int px, int py, int pwidth, int pheight);
public:
	StaticWorld(string filename, void (*run)(), bool(*interact)(Player*,int,int));
	
	void run();
	bool interact(Player *p, int x, int y);
	bool virtual collides(int x, int y, int xOffset, int yOffset, int width, int height);
	int virtual getLower(int x, int y);
	int virtual getUpper(int x, int y);
	int virtual getEffect(int x, int y);
	int virtual getCollision(int x, int y);
};

#endif