#ifndef ENTITY_H
#define ENTITY_H
#include "Revengine.h"

class Entity
{
	void(*_run)();
	bool(*_interact)(Player *p);
public:
	bool isAlive = true;
	int x, y, width, height;
	Entity(World *world, int x, int y, int width, int height, Sprite *sprite, void(*_run)(), bool(*_interact)(Player *p));
	virtual void run();
	virtual bool interact(Player *p);
	Sprite * sprite = nullptr;
	World *world = nullptr;
	virtual void save();
	virtual void load();
};

class SaveableEntity : public Entity
{
	void(*_save)();
	void(*_load)();
public:
	SaveableEntity(World *world, int x, int y, int width, int height, Sprite *sprite, void(*_run)(), bool(*_interact)(Player *p), void(*_save)(), void(*load)());
	virtual void save();
	virtual void load();
};

#endif