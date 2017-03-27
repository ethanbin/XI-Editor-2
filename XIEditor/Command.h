#ifndef COMMANDPLUS_
#define COMMANDPLUS_
#include "Enums.h"
#include<string>
class CommandPlus {
public:
	CommandPlus(); //do not delete! this is needed to run program
	CommandPlus(Action::Action);
	CommandPlus(Action::Action, std::string);
	CommandPlus(Action::Action, int);

	Action::Action getAction();
	void setAction(Action::Action);
	
	std::string getChange();
	void setChange(std::string);

	int getCharPos();
	void setCharPos(int);


private:
	Action::Action _action;
	std::string _change;
	int _charPos;
};
#endif