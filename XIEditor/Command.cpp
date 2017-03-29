#include"Command.h"

CommandPlus::CommandPlus() {
}

CommandPlus::CommandPlus(Action::Action action)
{
	_action = action;
}

CommandPlus::CommandPlus(Action::Action action, std::string change)
{
	//records what Action was done
	_action = action;
	//record text that was changed, such as a deleted line
	_change = change;
}

CommandPlus::CommandPlus(Action::Action action, int charPos)
{
	_action = action;
	_charPos = charPos;
}

CommandPlus::CommandPlus(Action::Action action, std::string change, int charPos) {
	_action = action;
	_change = change;
	_charPos = charPos;
}

Action::Action CommandPlus::getAction() {
	return _action;
}

void CommandPlus::setAction(Action::Action command)
{
	_action = command;
}

std::string CommandPlus::getChange()
{
	return _change;
}

void CommandPlus::setChange(std::string change){
	_change = change;
}

int CommandPlus::getCharPos()
{
	return _charPos;
}

void CommandPlus::setCharPos(int charPos)
{
	_charPos = charPos;
}