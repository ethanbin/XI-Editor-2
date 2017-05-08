#include "XIEditor.h"
#include "Enums.h"
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;

XIEditor::XIEditor(std::string fileName) {
	_size = 0;
	_fileName = fileName;
	ifstream userFile;
	userFile.open(fileName);

	if (!userFile.is_open()) {
		cout << "Error: File Not Found." << endl;
		userFile.close();
		exit(EXIT_FAILURE);
	}

	std::string lineCollector;
	for (; !userFile.eof(); _size++) {
		getline(userFile, lineCollector);
		//capacity+1 when inserting because LinkedList 
			//starts with position 1, unlike an array
		_listBuffer.insert(_size +1, lineCollector);
	}

	userFile.clear();
	userFile.seekg(0);
	
	_currentLine = 1;
	_currentChar = 1;
	userFile.close();
}

XIEditor::~XIEditor() {}

void XIEditor::start() {
	consoleBlackOnWhite();
	modeCommand();
	consoleWhiteOnBlack();
	clrscrn();
}

void XIEditor::displayError(std::string errorMsg) {
	moveCursorTo(0, _size);
	cout << "\n\n\n";
	cout << ":";
	consoleRedOnWhite();
	cout << errorMsg << endl;

	int totalWait = 4000;
	int waitCycle = 50;

	//this loop will wait a total of 3000 milliseconds or until a key is hit.
	for (int i = 0; i < totalWait; i += waitCycle) {
		wait(waitCycle);
		if (_kbhit()) {
			break;
		}
	}

	cin.clear();
	consoleBlackOnWhite();
}

bool XIEditor::save() {
	ofstream txtFile;
	txtFile.open(_fileName);
	if (!txtFile.is_open())
		return false;
	for (int i = 1; i <= _listBuffer.getLength()-1; i++)
		txtFile << _listBuffer.getEntry(i) << endl;
	txtFile << _listBuffer.getEntry(_listBuffer.getLength());
	txtFile.close();
	return true;
}

bool XIEditor::save(std::string fileName) {
	ofstream txtFile;
	txtFile.open(fileName);
	if (!txtFile.is_open())
		return false;
	for (int i = 1; i <= _listBuffer.getLength(); i++)
		txtFile << _listBuffer.getEntry(i) << endl;
	txtFile.close();
	return true;
}

//This method prints all visible content
void XIEditor::printLines()
{
	clrscrn();
	for (int i = 1; i < _size+1; i++)
		cout << _listBuffer.getEntry(i) << endl;
	moveCursorTo(_currentChar - 1, _currentLine - 1);
}

void XIEditor::goRight() {
	_currentChar++;
	//if not at last line
	if (_currentLine < _size)
		//move cursor to start of next line when going too far right
		if (_currentChar > _listBuffer.getEntry(_currentLine).length()) {
			_currentChar = 1;
			_currentLine++;
		}
	}

void XIEditor::goLeft() {
	_currentChar--;
	//if at first line, keep cursor from going too far left
	if (_currentLine < 1) {
		_currentLine = 1;
	}
	//if not at first line
	if (_currentLine > 1)
		//move cursor to end of previous line when going too far left
		if (_currentChar < 1) {
			_currentLine--;
			_currentChar = _listBuffer.getEntry(_currentLine).length();
		}
}

bool XIEditor::deleteLine(int deleteHere) {
	if (deleteHere < 1)
		return false;

	if (_size > 1) {
		_listBuffer.remove(deleteHere);
		_size--;
	}
	else if (_size == 1) {
		_listBuffer.replace(deleteHere, "");
		_size = 1;
	}
	return true;
}

void XIEditor::insertLine(std::string line, int insertHere) {
	_size++;
	_listBuffer.insert(insertHere, line);
}

bool XIEditor::stayInText() {
	bool isCorrected = false;
	//for going too far up
	if (_currentLine < 1) {
		_currentLine = 1;
		isCorrected = true;
	}
	//for going too far down
	if (_currentLine > _size) {
		if (_size != 0)
			_currentLine = _size;
		isCorrected = true;
	}
	int currentLineLength = _listBuffer.getEntry(_currentLine).length();
	//for going too far right
	if (_currentChar > currentLineLength) {
		_currentChar = currentLineLength;
		isCorrected = true;
	}
	//for going too far left
	if (_currentChar < 1) {
		_currentChar = 1;
		isCorrected = true;
	}
	return isCorrected;
}

void XIEditor::modeInsert(int originalCharPos) {
	std::string input = "";
	std::string originalLine = _listBuffer.getEntry(_currentLine);
	int charPos = originalCharPos;
	bool notEsc = true;
	bool isEdited = false;
	while (notEsc) 
	{
		printLines();
		input = _getch();
		
		switch (input[0])
		{
			case KeyCode::ESC: {
				notEsc = false;
				if (isEdited) {
					_commands.push(CommandPlus(KeyCode::INSERT_HERE, originalLine, charPos, _currentLine));
					originalLine = _listBuffer.getEntry(_currentLine);
					isEdited = false;
				}
				else {//for now, this is primarily for undoing 'I' when isEdited wasnt set
					if (_currentChar != charPos) {
						_commands.push(CommandPlus(KeyCode::INSERT_START, charPos));
					}
				}
				break;
			}
			//this case takes care of special keys like delete and arrow keys
			case KeyCode::FUNC_KEY: {
				//certain special keys read in 2 characters, not just one.
				//these keys first read in à then another letter.
				//this extra getch takes in the second part of the special key
				char secChar = _getch();	
				switch (secChar) {
					case KeyCode::FUNC_DELETE: {
						if (_listBuffer.getEntry(_currentLine).length() > 0) {
							std::string change = _listBuffer.getEntry(_currentLine).erase(_currentChar-1, 1);
						_listBuffer.replace(_currentLine, change);
						isEdited = true;
						break;
						}
					}
					case KeyCode::FUNC_ARROW_RIGHT: {
						if (isEdited) {
							_commands.push(CommandPlus(KeyCode::INSERT_HERE, originalLine, charPos, _currentLine));
							originalLine = _listBuffer.getEntry(_currentLine);
							isEdited = false;
						}

						_currentChar++;
						//not using stayInText because thatll stop cursor from writing at end of line
						int currentLineLength = _listBuffer.getEntry(_currentLine).length();
						if (_currentChar > currentLineLength + 1)
							_currentChar = currentLineLength+1;
						else
							_commands.push(CommandPlus(KeyCode::RIGHT));
						charPos = _currentChar;
						break;
					}
					case KeyCode::FUNC_ARROW_LEFT: {
						if (isEdited){
							_commands.push(CommandPlus(KeyCode::INSERT_HERE, originalLine, charPos, _currentLine));
							originalLine = _listBuffer.getEntry(_currentLine);
							isEdited = false;
						}
						_currentChar--;
						if (!stayInText())
							_commands.push(CommandPlus(KeyCode::LEFT));
						stayInText();
						charPos = _currentChar;
						break;
					}
					case KeyCode::FUNC_ARROW_UP: {
						if (isEdited) {
							_commands.push(CommandPlus(KeyCode::INSERT_HERE, originalLine, charPos, _currentLine));
							originalLine = _listBuffer.getEntry(_currentLine);//to record correct line
							isEdited = false;
						}
						_currentLine--;
						if (_currentLine != 0)
							_commands.push(CommandPlus(KeyCode::UP, _currentChar));
						stayInText();
						//this is ok to have out of above if statement because 
						//moving up or down will change what originalLine should be regardless of 
						//whether or not anything was editted
						originalLine = _listBuffer.getEntry(_currentLine);
						charPos = _currentChar;
						break;
					}
					case KeyCode::FUNC_ARROW_DOWN: {
						if (isEdited) {
							_commands.push(CommandPlus(KeyCode::INSERT_HERE, originalLine, charPos, _currentLine));
							isEdited = false;
						}
						_currentLine++;
						if (_currentLine != _size + 1)
							_commands.push(CommandPlus(KeyCode::DOWN, _currentChar));
						stayInText();
						//this is ok to have out of above if statement because 
						//moving up or down will change what originalLine should be regardless of 
						//whether or not anything was editted
						originalLine = _listBuffer.getEntry(_currentLine);
						charPos = _currentChar;
						break;
					}
				}
				
				break;
			}
			case KeyCode::BACKSPACE: {
				_currentChar--;
				if (!stayInText()) {
					std::string change = _listBuffer.getEntry(_currentLine).erase(_currentChar-1, 1);
					_listBuffer.replace(_currentLine, change);
				}
				else if (_currentChar == 1){
					std::string change = _listBuffer.getEntry(_currentLine).erase(0, 1);
					_listBuffer.replace(_currentLine, change); 
				}
				//_currentChar++;
				isEdited = true;
				break;
			}/*
			case KeyCode::RETURN: {
				//copy text to right of cursor
				std::string temp = _listBuffer.getEntry(_currentLine).substr(_currentChar-1,
					_listBuffer.getEntry(_currentLine).length() - _currentChar+1);

				//delete text to right of cursor
				std::string cut = _listBuffer.getEntry(_currentLine).erase(_currentChar-1,
					_listBuffer.getEntry(_currentLine).length() - _currentChar+1);

				_listBuffer.replace(_currentLine, cut);
				
				insertLine(temp, ++_currentLine);
				_currentChar = 1;
				break;
			}*/
			default: {
								//excluding tab until it is supported
				if (input[0] != KeyCode::RETURN && input[0] != '\t') {
					input = input[0];
					std::string edited = _listBuffer.getEntry(_currentLine).insert((_currentChar++)-1, input);
					_listBuffer.replace(_currentLine, edited);
					isEdited = true;
				}
				break;
			}
		}
	}
}

//returns false if quit command was used.
bool XIEditor::modeLastLine() {
	const std::string quit = "q", write = "w", writeQuit = "wq";
	moveCursorTo(0, _size);
	cout << "\n\n\n";
	cout << ":";
	std::string input;
	getline(cin,input);
	cin.clear();
	if (input == "")
		return true;
	else if (input == quit)
		return false;
	else if (input == write){
		save();
		return true;
	}
	else if (input == writeQuit) {
		save();
		return false;
	}
	else{
		displayError("Not an Editor command: " + input);
		return true;
	}
}

void XIEditor::modeCommand() {
	char userInput;

	bool notEsc = true;
	while (notEsc)
	{
		printLines();
		userInput = _getch();

		switch (userInput)
		{
				//move up
			case KeyCode::UP:
			{
				_currentLine--;
				if (_currentLine != 0)
					_commands.push(CommandPlus(KeyCode::UP, _currentChar));
				break;
			}
			//move down
			case KeyCode::DOWN:
			{
				_currentLine++;
				if (_currentLine != _size + 1)
					_commands.push(CommandPlus(KeyCode::DOWN, _currentChar));
				break;
			}

			//move right
			case KeyCode::RIGHT:
			{
				//goRight();
				_currentChar++;
				if (!stayInText())
					_commands.push(CommandPlus(KeyCode::RIGHT));
				break;
			}
			//move left
			case KeyCode::LEFT:
			{
				//goLeft();
				_currentChar--;
				if (!stayInText())
					_commands.push(CommandPlus(KeyCode::LEFT));
				break;
			}
			case KeyCode::DEL_CHAR:
			{
				//if the string is empty (no char to delete), delete the line.
				if (_listBuffer.getEntry(_currentLine) == "") {
					deleteLine(_currentLine);
					_commands.push(CommandPlus(KeyCode::DEL_LINE, std::string("")));
				}

				else {
					//push to stack the command and the letter being deleted
					_commands.push(CommandPlus(KeyCode::DEL_CHAR,
						std::string(1, _listBuffer.getEntry(_currentLine)[_currentChar-1]),
						_currentChar)
					);
					std::string changed = _listBuffer.getEntry(_currentLine).erase(_currentChar-1, 1);
					_listBuffer.replace(_currentLine, changed);
				}
				break;
			}
			case KeyCode::DEL_LINE: {
				//push to stack the command and the line being deleted
				if (_getch() == KeyCode::DEL_LINE && _size >= 0) {
					_commands.push(CommandPlus(KeyCode::DEL_LINE, _listBuffer.getEntry(_currentLine)));
					deleteLine(_currentLine);
				}
				break;
			}
			case KeyCode::UNDO: {
				undo();
				break;
			}/**/
			case KeyCode::UNDO_ALL: {
				int lineToUndo = _currentLine;
				while(_currentLine == lineToUndo)
					undo();
				break;
			}
			case KeyCode::INSERT_ABOVE: {
				_commands.push(CommandPlus(KeyCode::INSERT_ABOVE,_currentChar));
				_currentChar = 0; //set cursor to beginning of the line
				insertLine("", _currentLine);
				_currentChar = 1;
				modeInsert(_currentChar);
				break;
			}
			case KeyCode::INSERT_BELOW: {
				_commands.push(CommandPlus(KeyCode::INSERT_BELOW, _currentChar));
				insertLine("", ++_currentLine);
				_currentChar = 1;
				modeInsert(_currentChar);
				break;
			}
			case KeyCode::INSERT_HERE: {
				modeInsert(_currentChar);
				int startingLocation = _currentChar;
				break;
			}
			case KeyCode::INSERT_START: {
				int originalCharPos = _currentChar;
				_currentChar = 1;
				modeInsert(originalCharPos);
				break;
			}
			case KeyCode::LAST_LINE:
			{
				if (!modeLastLine()) {
					notEsc = false;
				}
				break;
			}
		}
		stayInText();
	}
}

bool XIEditor::undo() {
	if (_commands.isEmpty())
		return false;
	CommandPlus lastCommand = _commands.peek();
	//will undo an action
	switch (lastCommand.getAction())
	{
		case KeyCode::UP: {
			//converts getChange to int
			_currentChar = lastCommand.getCharPos();
			_currentLine++;
			_commands.pop();
			break;
		}
		case KeyCode::DOWN: {
			//converts getChange to int
			_currentChar = lastCommand.getCharPos();
			_currentLine--;
			_commands.pop();
			break;
		}
		case KeyCode::RIGHT: {
			//goLeft();
			_currentChar--;
			_commands.pop();
			break;
		}
		case KeyCode::LEFT: {
			//goRight();
			_currentChar++;
			_commands.pop();
			break;
		}
		case KeyCode::DEL_CHAR: {
			_currentChar = lastCommand.getCharPos();
			std::string edited = _listBuffer.getEntry(_currentLine).insert(_currentChar-1,
															lastCommand.getChange());
			_listBuffer.replace(_currentLine, edited);
			

			_commands.pop();
			break;
		}
		case KeyCode::DEL_LINE: {
			insertLine(lastCommand.getChange(), _currentLine);
			_commands.pop();
			break;
		}
		case KeyCode::INSERT_HERE: {
			_currentLine = lastCommand.getLinePos();
			_listBuffer.replace(_currentLine, lastCommand.getChange());

			_currentChar = lastCommand.getCharPos();
			_commands.pop();
			break;
		}
		case KeyCode::INSERT_START: {
			_currentChar = lastCommand.getCharPos();
			break;
		}
		case KeyCode::INSERT_ABOVE: {
			deleteLine(_currentLine);
			_currentChar = lastCommand.getCharPos();
			_commands.pop();
			break;
		}
		case KeyCode::INSERT_BELOW: {
			deleteLine(_currentLine);
			_currentLine--;
			_currentChar = lastCommand.getCharPos();
			_commands.pop();
			break;
		}
		default: return false;
	}
	return true;
}