#ifndef PHRASE_BATTLE_H
#define PHRASE_BATTLE_H
#include <string>
#include "GameState.h"
#include "Collections.h"

struct Phrase
{
	std::string msg;
	int type;
	int ranking;
	Phrase();
	Phrase(std::string message, int type, int ranking);
};

class PhraseBattle : public GameState
{
	Phrase*phrases;
	int length;
	Phrase chosen[4];
	int *output;
	int choice = 0;
	GameState *caller;
	bool used = false;
public:
	PhraseBattle(Player *player, Phrase*phrases, int length, int *output);
	virtual void draw();
	virtual void run();
};

#endif