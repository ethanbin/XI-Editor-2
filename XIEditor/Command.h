#ifndef COMMAND_
#define COMMAND_
#include "Enums.h"
#include<string>
class Command {
public:
	Command(); //do not delete! this is needed to run program
	Command(Action::Action);
	Command(Action::Action, std::string);

	Action::Action getAction();
	void setAction(Action::Action);
	
	std::string getChange();
	void setChange(std::string);


private:
	Action::Action _action;
	std::string _change;
};
#endif