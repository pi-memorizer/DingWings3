#ifndef BLOCKING_H
#define BLOCKING_H
#include "Revengine.h"

void bTextbox(Player *p, string s);
void bTextbox(Player *p, string s, bool skippable);
string bOptionPane(Player *p, string msg, string choices[], int numChoices);
string bOptionPane(Player *p, string msg, string choice1, string choice2);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3, string choice4);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3, string choice4, string choice5);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6, string choice7);
string bOptionPane(Player *p, string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6, string choice7, string choice8);
int bNumberPane(Player *p, string msg, int start, int min, int max);
Item *bSelectItem(Player *p);
Item *bSelectItem(Player *p, unsigned long long categories);
struct ApplicationClosingException
{
	ApplicationClosingException();
	~ApplicationClosingException();
};

#endif