#include "Blocking.h"

void bTextbox(string s)
{
	TextBox * text = new TextBox(players[0], s);
	players[0]->pushState(text);
	while (text == players[0]->getState())
	{
		theLoop();
	}
}