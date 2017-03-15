#include"Command.h"

Command::Command() {
}

Command::Command(Action::Action command)
{
	_action = command;
}

Command::Command(Action::Action command, std::string change)
{
	//records what Action was done
	_action = command;
	//record text that was changed, such as a deleted line
	_change = change;
}
Action::Action Command::getAction() {
	return _action;
}

void Command::setAction(Action::Action command)
{
	_action = command;
}

std::string Command::getChange()
{
	return _change;
}

void Command::setChange(std::string change){
	_change = change;
}