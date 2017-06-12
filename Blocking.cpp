#include "Blocking.h"
#include "Player.h"
#include "GameState.h"
#include "Sprite.h"
#include "Item.h"

ApplicationClosingException::ApplicationClosingException()
{

}
ApplicationClosingException::~ApplicationClosingException()
{

}

TextBox::TextBox(Player *p, string _msg) : GameState(p)
{
	msg = _msg;
	caller = p->getState();
}

void TextBox::run()
{
	caller->run();
	startMenu();
	if (keys[p->keyA] && !a)
	{
		p->popState();
		return;
	}
	endMenu();
}

void drawTextBox(string msg, int *count)
{
	const int MAX_LINES = 4;
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0);
	SDL_Rect rect;
	rect.x = 0;
	rect.w = WIDTH;
	rect.h = 8 + 2 * MAX_LINES * 8;
	rect.y = HEIGHT - rect.h;
	SDL_RenderFillRect(renderer, &rect);
	/*for (int i = 0; i < msg.length(); i++)
	{
	chars[msg[i]]->draw(8 + 8 * i, rect.y + 8);
	}*/
	int line = 1;
	int x = 0;
	int i = 0;
	int j = 0;
	while (j < msg.length())
	{
		while (msg[j] != ' '&&j < msg.length()) j++;
		if (x + j - i > WIDTH / 8 - 2)
		{
			line++;
			x = 0;
		}
		for (; i < j; i++)
		{
			if (!(x == 0 && msg[i] == ' '))
			{
				chars[msg[i]]->draw(8 + 8 * x, rect.y + 8 * line);
				x++;
			}
		}
		j++;
	}
	*count++;
}

void TextBox::draw()
{
	caller->draw();
	int count;
	drawTextBox(msg, &count);
}

class OptionPane : public GameState
{
	string msg;
	string *choices;
	int numChoices;
	string *output;
	int choice = 0;
	GameState *caller;
public:
	OptionPane(Player *p, string msg, string choices[], int numChoices, string *output);
	virtual void run();
	virtual void draw();
};

OptionPane::OptionPane(Player *p, string msg, string choices[], int numChoices, string *output) : GameState(p)
{
	this->msg = msg;
	this->choices = choices;
	this->numChoices = numChoices;
	this->output = output;
	caller = p->getState();
}

void OptionPane::run()
{
	caller->run();
	startMenu();
	if (keys[p->keyUp] && !up)
	{
		choice--;
		if (choice < 0) choice = numChoices - 1;
	}
	if (keys[p->keyDown] && !down)
	{
		choice++;
		if (choice >= numChoices) choice = 0;
	}
	if (keys[p->keyA] && !a)
	{
		*output = choices[choice];
		p->popState();
		return;
	}
	if (keys[p->keyB] && !b)
	{
		*output = "null";
		p->popState();
		return;
	}
	endMenu();
}

void OptionPane::draw()
{
	caller->draw();
	int count = 0;
	drawTextBox(msg, &count);
	chars['>']->draw(0, choice * 8);
	for (int i = 0; i < numChoices; i++)
	{
		int length = choices[i].length();
		for (int j = 0; j < length; j++)
		{
			chars[choices[i][j]]->draw(WIDTH - 8 * length + j * 8, i * 8);
		}
	}
}

void bTextbox(string s)
{
	TextBox * text = new TextBox(players[0], s);
	players[0]->pushState(text);
	while (text == players[0]->getState())
	{
		if (!theLoop())
		{
			throw ApplicationClosingException();
		}
	}
}

string bOptionPane(string msg, string choices[], int numChoices)
{
	string output;
	OptionPane * pane = new OptionPane(players[0], msg, choices, numChoices, &output);
	players[0]->pushState(pane);
	while (pane == players[0]->getState())
	{
		if (!theLoop())
		{
			throw ApplicationClosingException();
		}
	}
	return output;
}

string bOptionPane(string msg, string choice1, string choice2)
{
	string choices[] = {
		choice1, choice2
	};
	return bOptionPane(msg, choices, 2);
}
string bOptionPane(string msg, string choice1, string choice2, string choice3)
{
	string choices[] = {
		choice1, choice2, choice3
	};
	return bOptionPane(msg, choices, 3);
}
string bOptionPane(string msg, string choice1, string choice2, string choice3, string choice4)
{
	string choices[] = {
		choice1, choice2, choice3, choice4
	};
	return bOptionPane(msg, choices, 4);
}
string bOptionPane(string msg, string choice1, string choice2, string choice3, string choice4, string choice5)
{
	string choices[] = {
		choice1, choice2, choice3, choice4, choice5
	};
	return bOptionPane(msg, choices, 5);
}
string bOptionPane(string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6)
{
	string choices[] = {
		choice1, choice2, choice3, choice4, choice5, choice6
	};
	return bOptionPane(msg, choices, 6);
}
string bOptionPane(string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6, string choice7)
{
	string choices[] = {
		choice1, choice2, choice3, choice4, choice5, choice6, choice7
	};
	return bOptionPane(msg, choices, 7);
}
string bOptionPane(string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6, string choice7, string choice8)
{
	string choices[] = {
		choice1, choice2, choice3, choice4, choice5, choice6, choice7, choice8
	};
	return bOptionPane(msg, choices, 8);
}

class NumberPane : public GameState
{
	int delay = 0;
	string msg;
	int choice, min, max;
	int *output;
	GameState*caller;
public:
	NumberPane(Player *p, string msg, int start, int min, int max, int*output);
	virtual void run();
	virtual void draw();
};

NumberPane::NumberPane(Player *p, string msg, int start, int min, int max, int *output) : GameState(p)
{
	this->msg = msg;
	choice = start;
	this->min = min;
	this->max = max;
	this->output = output;
	caller = p->getState();
}

void NumberPane::draw()
{
	caller->draw();
	int count = 0;
	drawTextBox(msg, &count);
	string num = to_string(choice);
	for (int i = 0; i < num.length(); i++)
	{
		chars[num[i]]->draw(i * 8, 0);
	}
}

void NumberPane::run()
{
	caller->run();
	startMenu();
	if (keys[p->keyA] && !a)
	{
		*output = choice;
		p->popState();
		return;
	}
	if (keys[p->keyB] && !b)
	{
		*output = 0;
		p->popState();
		return;
	}
	if (keys[p->keyUp])
	{
		if (!up)
		{
			delay = 0;
			choice--;
			if (choice < min) choice = max;
		}
		else {
			if (delay >= 40)
			{
				choice--;
				if (choice < min) choice = max;
			}
			else delay++;
		}
	} else if (keys[p->keyDown])
	{
		if (!down)
		{
			delay = 0;
			choice++;
			if (choice >max) choice =  min;
		}
		else {
			if (delay >= 40)
			{
				choice++;
				if (choice > max) choice = min;
			}
			else delay++;
		}
	}
	else delay = 0;

	endMenu();
}

int bNumberPane(string msg, int start, int min, int max)
{
	int output;
	NumberPane *pane = new NumberPane(players[0], msg, start, min, max, &output);
	players[0]->pushState(pane);
	while (pane == players[0]->getState())
	{
		if (!theLoop())
		{
			throw ApplicationClosingException();
		}
	}
	return output;
}

class InventoryDialogue : public GameState
{
	static int pointerX, pointerY;
	static int positionPairs[];
	static Sprite *inventoryUI, *inventoryPointer, *inventoryBadSpot;
	Item **output;
	int moveToNearest();
	unsigned long long categories;
public:
	InventoryDialogue(Player *p, unsigned long long categories, Item**output);
	virtual void run();
	virtual void draw();
};

int InventoryDialogue::pointerX = 0;
int InventoryDialogue::pointerY = 0;

Sprite *InventoryDialogue::inventoryUI = nullptr;
Sprite *InventoryDialogue::inventoryPointer = nullptr;
Sprite *InventoryDialogue::inventoryBadSpot = nullptr;

int InventoryDialogue::positionPairs[2*INVENTORY_SLOTS] = {
	21,15,
	117,28,
	64,90
};

InventoryDialogue::InventoryDialogue(Player *p, unsigned long long categories, Item **output) : GameState(p)
{
	this->categories = categories;
	this->output = output;
}

void InventoryDialogue::run()
{
	int index = 0;
	const int MOVE_AMOUNT = 100;
	startMenu();
	if (keys[p->keyA] && !a)
	{
		if (p->inventory[index].item != nullptr)
		{
			if (categories == p->inventory[index].item->flags&categories)
			{
				*output = p->inventory[index].item;
				p->popState();
				return;
			}
		}
		else {
			*output = nullptr;
			p->popState();
			return;
		}
	}
	if (keys[p->keyB] && !b)
	{
		*output = nullptr;
		p->popState();
		return;
	}
	if (keys[p->keyUp] && !up)
	{
		pointerY -= MOVE_AMOUNT;
	}
	if (keys[p->keyDown] && !down)
	{
		pointerY += MOVE_AMOUNT;
	}
	if (keys[p->keyLeft] && !left)
	{
		pointerX -= MOVE_AMOUNT;
	}
	if (keys[p->keyRight] && !right)
	{
		pointerX += MOVE_AMOUNT;
	}
	bool found = false;
	for (int i = 0; i < 2 * INVENTORY_SLOTS; i += 2)
	{
		if (pointerX == positionPairs[i] && pointerY == positionPairs[i + 1])
		{
			found = true;
			index = i / 2;
			break;
		}
	}
	if (!found)
	{
		index = moveToNearest();
	}
	endMenu();
}

int InventoryDialogue::moveToNearest()
{
	int minD = 999999999;
	int x = pointerX;
	int y = pointerY;
	int index = -1;
	for (int i = 0; i < 2 * INVENTORY_SLOTS; i += 2)
	{
		int _x = positionPairs[i];
		int _y = positionPairs[i + 1];
		int d = (_x - pointerX)*(_x - pointerX) + (_y - pointerY)*(_y - pointerY);
		if (d < minD)
		{
			minD = d;
			x = _x;
			y = _y;
			index = i/2;
		}
	}
	pointerX = x;
	pointerY = y;
	return index;
}

void InventoryDialogue::draw()
{
	if (inventoryUI == nullptr)
	{
		inventoryUI = new Sprite("inventoryUI", 0, 0);
		inventoryPointer = new Sprite("inventoryPointer", 0, 0);
		inventoryBadSpot = new Sprite("inventoryBadSpot", 0, 0);
	}
	inventoryUI->draw(0, 0);
	inventoryPointer->draw(pointerX, pointerY);
	for (int i = 0; i < 2 * INVENTORY_SLOTS; i += 2)
	{
		int x = positionPairs[i];
		int y = positionPairs[i + 1];
		Item *item = p->inventory[i / 2].item;
		if (item != nullptr)
		{
			Sprite *s = item->sprite;
			if (s != nullptr)
			{
				s->draw(x, y);
				if (categories != (categories&item->flags))
				{
					inventoryBadSpot->draw(x, y);
				}
			}
		}
	}
}

Item* bSelectItem()
{
	return bSelectItem(~((unsigned long long)0));
}

Item* bSelectItem(unsigned long long categories)
{
	Item *output = nullptr;
	InventoryDialogue *state = new InventoryDialogue(players[0], categories, &output);
	players[0]->pushState(state);
	while (state == players[0]->getState())
	{
		if (!theLoop())
		{
			throw ApplicationClosingException();
		}
	}
	return output;
}