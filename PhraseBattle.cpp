#include "PhraseBattle.h"
#include "Revengine.h"
#include "Player.h"
#include "Event.h"
#include "Blocking.h"

Phrase::Phrase()
{
	msg = "";
	type = -1;
	ranking = -1;
}

Phrase::Phrase(std::string message, int type, int ranking)
{
	this->msg = message;
	this->type = type;
	this->ranking = ranking;
}

class PhraseLine : public GameState
{
	Phrase phrase;
	int *output;
	int x, y;
	GameState *caller;
	int lineLength;
	int frame = 0;
	int wait = 90;
	int lastChar = -1;
	int state = 0;
public:
	PhraseLine(Player *p, int x, int y, int lineLength, Phrase phrase, int *output);
	virtual void draw();
	virtual void run();
};

PhraseBattle::PhraseBattle(Player *player, Phrase*phrases, int length, int *output) : GameState(player)
{
	this->phrases = phrases;
	this->length = length;
	this->output = output;
	caller = player->getState();
	for (int i = 0; i < 4; i++)
	{
		int index = rand() % length;
		bool passes = false;
		while (!passes)
		{
			passes = true;
			for (int j = 0; j < i; j++)
			{
				if (chosen[j].msg == phrases[index].msg)
				{
					index = rand() % length;
					passes = false;
					break;
				}
			}
		}
		chosen[i] = phrases[index];
	}
	for (int i = 0; i < 4; i++)
	{
		int low = i;
		for (int j = i + 1; j < 4; j++)
		{
			if (chosen[low].ranking > chosen[j].ranking)
			{
				low = j;
			}
		}
		Phrase temp = chosen[low];
		chosen[low] = chosen[i];
		chosen[i] = temp;
	}
	chosen[0].ranking = 0;
	chosen[1].ranking = 50;
	chosen[2].ranking = 80;
	chosen[3].ranking = 100;
}

void PhraseBattle::draw()
{
	caller->draw();
	if (used) return;
	CharacterInfo info;
	for (int i = 0; i < 4; i++)
	{
		drawTextBox(chosen[i].msg, 1000000, i==0, true, 0, 1, HEIGHT / 8 - 9 + i*2, WIDTH / 8 - 2, info);
	}
	drawCharacter('>', 0, HEIGHT - 8 * (8 - choice * 2), 0, 0, 0);
}

void PhraseBattle::run()
{
	if (used)
	{
		p->popState();
		return;
	}
	startMenu();
	if (getKey(p, KEY_A) && !a)
	{
		*output = chosen[choice].ranking;
		if (chosen[choice].type == 0)
		{
			p->pushState(new PhraseLine(p, 1, HEIGHT / 8 - 9 + choice * 2, WIDTH/8-2,chosen[choice], output));
		}
		used = true;
		return;
	}
	if (getKey(p, KEY_UP) && !up)
	{
		choice--;
		if (choice == -1)
			choice = 3;
	}
	if (getKey(p, KEY_DOWN) && !down)
	{
		choice++;
		if (choice == 4)
			choice = 0;
	}
	endMenu();
}

PhraseLine::PhraseLine(Player *p, int x, int y, int lineLength, Phrase phrase, int *output) : GameState(p)
{
	this->x = x;
	this->y = y;
	this->lineLength = lineLength;
	this->phrase = phrase;
	this->output = output;
	this->caller = p->getState();
}

void PhraseLine::draw()
{
	setDrawColor(255, 255, 255, 255);
	clearScreen();
	CharacterInfo info;
	drawCharacter(0xDB, 8 * (x + frame/8), 8 * y + 8, 128, 128, 128);
	drawTextBox(phrase.msg, 1000000, false, true, -1, x, y, lineLength, info);
	if (wait > 0)
		drawCharacter('1' + ((wait - 1) / 30), 8 * x + frame, 8 * y, 255, 0, 0);
	if (state < 0)
	{
		drawTextBox("<cFF0000>MISS", 10000000, false, true, -1, 5, 10, 10, info);
		state++;
	}
	else if (state > 0)
	{
		drawTextBox("<c00FF00>HIT", 10000000, false, true, -1, 5, 10, 10,info);
		state--;
	}
	drawCharacter(0, 8 * x + frame, 8*y+8, 255, 0, 0);
}

void PhraseLine::run()
{
	startMenu();
	if (wait == 0)frame++; else wait--;
	CharacterInfo info;
	drawTextBox(phrase.msg, 10000000, false, false, frame / 8, 0, 0, 100, info);
	if (frame/8>info.length)
	{
		p->popState();
		return;
	}
	if (getKey(p, KEY_A) && !a)
	{
		if (lastChar == frame / 8 || info.g != 255)
		{
			*output -= 15;
			state = -30;
		}
		else {
			*output += 25;
			state = 30;
		}
		lastChar = frame / 8;
	}
	endMenu();
}