#ifndef COMMANDPLUS_
#define COMMANDPLUS_
#include "Enums.h"
#include<string>
class CommandPlus {
public:
	CommandPlus(); //do not delete! this is needed to run program
	CommandPlus(KeyCode::KeyCode);
	CommandPlus(KeyCode::KeyCode, std::string);
	CommandPlus(KeyCode::KeyCode, int);
	CommandPlus(KeyCode::KeyCode, std::string, int);

	KeyCode::KeyCode getAction();
	void setAction(KeyCode::KeyCode);
	
	std::string getChange();
	void setChange(std::string);

	int getCharPos();
	void setCharPos(int);


private:
	KeyCode::KeyCode _action;
	std::string _change;
	int _charPos;
};
#endif