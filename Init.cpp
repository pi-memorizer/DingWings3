#include "Revengine.h"
#include "Blocking.h"
#include "World.h"
#include "Player.h"
#include "Entity.h"
#include "Item.h"

void nothing() {} //run variant
bool nothing(Player *p, int x, int y) { //interact variant
	if (x == -1 && y == 2)
	{
		bSelectItem(p,ITEM_FOOD);
		return true;
	}
	return false;
}
bool nothing(Player *p) //interact entity variant
{
	return true;
}
void enterNothing(Player *p)
{

}

void init()
{
	//add entities to worlds and stuff here preferably
	World *test = new StaticWorld("test", &enterNothing,&nothing, &nothing);
	worlds.add(test);
	Entity *testEntity = new Entity(test, -4, 9, 32, 32, nullptr, &nothing, &nothing);
	test->addEntity(testEntity);
	addItem("null")
		->_flags(ITEM_FOOD | ITEM_FURNITURE)
		->_displayName("Nothing");
}