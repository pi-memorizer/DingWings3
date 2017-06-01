#include "Revengine.h"
#include "Blocking.h"

void nothing() {} //run variant
bool nothing(Player *p, int x, int y) { //interact variant
	if (x == -1 && y == 2)
	{
		bTextbox("Test 1");
		bTextbox("Test 2");
		bTextbox("Test 3");
		return true;
	}
	return false;
}
bool nothing(Player *p) //interact entity variant
{
	textbox(0, "Sup?");
	return true;
}

void init()
{
	//add entities to worlds and stuff here preferably
	World *test = new StaticWorld("test", &nothing, &nothing);
	worlds.add(test);
	Entity *testEntity = new Entity(test, -4, 9, 32, 32, nullptr, &nothing, &nothing);
	test->addEntity(testEntity);
}