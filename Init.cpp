#include "Revengine.h"

void nothing() {} //run variant
bool nothing(Player *p, int x, int y) { //interact variant
	if (x == -1 && y == 2)
	{
		textbox(0, "This is a test. I had to make the text multiple lines long to see what happens, you know? I wonder if this is long enough...");
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
	worlds.add(new StaticWorld("test", &nothing, &nothing));
	worlds[0]->addEntity(new Entity(worlds[0], -1, 1, 32,32,nullptr, &nothing, &nothing));
}