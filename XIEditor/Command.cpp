#include"Command.h"

CommandPlus::CommandPlus() {
	_change = "";
	_charPos = 0;
}

CommandPlus::CommandPlus(KeyCode::KeyCode action)
{
	_action = action;
	_change = "";
	_charPos = 0;
}

CommandPlus::CommandPlus(KeyCode::KeyCode action, std::string change)
{
	//records what Action was done
	_action = action;
	//record text that was changed, such as a deleted line
	_change = change;
	_charPos = 0;
}

CommandPlus::CommandPlus(KeyCode::KeyCode action, int charPos)
{
	_action = action;
	_change = "";
	_charPos = charPos;
}

CommandPlus::CommandPlus(KeyCode::KeyCode action, std::string change, int charPos) {
	_action = action;
	_change = change;
	_charPos = charPos;
}

CommandPlus::CommandPlus(KeyCode::KeyCode action, std::string change, int charPos, int linePos) {
	_action = action;
	_change = change;
	_charPos = charPos;
	_linePos = linePos;
}

KeyCode::KeyCode CommandPlus::getAction() {
	return _action;
}

void CommandPlus::setAction(KeyCode::KeyCode command)
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

int CommandPlus::getLinePos()
{
	return _linePos;
}

void CommandPlus::setLinePos(int linePos)
{
	_linePos = linePos;
}