#ifndef BLOCKING_H
#define BLOCKING_H
#include "Revengine.h"
#include "GameState.h"

class Script : public GameState
{
public:
	struct Return{
		Return(string s);
		Return(int i);
		Return(Item* item);
		Return();
		/*Return(const Return&);
		~Return();
		Return &operator=(const Return&);*/
		string s = "";
		int i = 0;
		Item* item = nullptr;
	};
private:
	Script(Player *p,GameState*caller);
	List <Return> states;
	GameState*caller;
	int currentState = 0;
	void(*_script)(Player*,int x, int y,Script*);
	int x, y;
public:
	static void start(Player *p, int x, int y, void(*script)(Player*,int,int,Script*));
	void run();
	void draw();
	void textBox(string s, bool skippable);
	string optionPane(string msg, string choice1, string choice2);
	int numberPane(string msg, int start, int min, int max);
	Item* selectItem();
	Item* selectItem(unsigned long long categories);
};

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