#ifndef BLOCKING_H
#define BLOCKING_H
#include "Revengine.h"

void bTextbox(string s);
string bOptionPane(string msg, string choices[], int numChoices);
string bOptionPane(string msg, string choice1, string choice2);
string bOptionPane(string msg, string choice1, string choice2, string choice3);
string bOptionPane(string msg, string choice1, string choice2, string choice3, string choice4);
string bOptionPane(string msg, string choice1, string choice2, string choice3, string choice4, string choice5);
string bOptionPane(string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6);
string bOptionPane(string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6, string choice7);
string bOptionPane(string msg, string choice1, string choice2, string choice3, string choice4, string choice5, string choice6, string choice7, string choice8);
int bNumberPane(string msg, int start, int min, int max);
Item *bSelectItem();
Item *bSelectItem(unsigned long long categories);
struct ApplicationClosingException
{
	ApplicationClosingException();
	~ApplicationClosingException();
};

#endif