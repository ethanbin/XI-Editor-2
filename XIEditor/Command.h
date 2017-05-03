#ifndef COMMANDPLUS_
#define COMMANDPLUS_
#include "Enums.h"
#include<string>
class CommandPlus {
public:
	CommandPlus(); //do not delete! this is needed to run program
	CommandPlus(KeyCode::KeyCode);

	//KeyCode::KeyCode action, std::string change
	CommandPlus(KeyCode::KeyCode, std::string);

	//KeyCode::KeyCode action, int charPos
	CommandPlus(KeyCode::KeyCode, int);

	//KeyCode::KeyCode action, std::string change, int charPos
	CommandPlus(KeyCode::KeyCode, std::string, int);

	//KeyCode::KeyCode action, std::string change, int charPos, int linePos
	CommandPlus(KeyCode::KeyCode, std::string, int, int);


	KeyCode::KeyCode getAction();
	void setAction(KeyCode::KeyCode);
	
	std::string getChange();
	void setChange(std::string);

	int getCharPos();
	void setCharPos(int);

	int getLinePos();
	void setLinePos(int);

private:
	KeyCode::KeyCode _action;
	std::string _change;
	int _charPos, _linePos;
};
#endif