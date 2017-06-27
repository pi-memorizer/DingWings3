#ifndef ITEM_H
#define ITEM_H
#include "Revengine.h"
#define ITEM_FOOD 1
#define ITEM_FURNITURE 2

class Item
{
public:
	string name;
	Sprite *sprite = nullptr;
	string displayName;
	unsigned long long flags;
	Item(string name);
	bool containsFlag(unsigned long long flag);
	Item* _flags(unsigned long long flags);
	Item* _displayName(string name);
	Item* _sprite(Sprite *sprite);
};

extern Hashmap<string, Item*> items;

Item* addItem(string name);

#endif