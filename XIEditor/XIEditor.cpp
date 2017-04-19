#include "XIEditor.h"
#include "Enums.h"
#include <iostream>
#include<string>
#include <fstream>
#include <conio.h>

using std::cout;
using std::endl;
using std::ifstream;

XIEditor::XIEditor(std::string fileName) {
	_capacity = 0;

	ifstream userFile;
	userFile.open(fileName);

	if (!userFile.is_open()) {
		cout << "ERROR: No File Found." << endl;
		userFile.close();
		exit(EXIT_FAILURE);
	}

	std::string lineCollector;
	for (; !userFile.eof(); _capacity++) {
		getline(userFile, lineCollector);
		//capacity+1 when inserting because LinkedList 
			//starts with position 1, unlike an array
		_listBuffer.insert(_capacity+1, lineCollector);
	}

	userFile.clear();
	userFile.seekg(0);
	
	//_arrayBuffer = new std::string[_capacity];

//	for (int i = 0; !userFile.eof(); i++)
//		_listBuffer.insert(userFile.getline());//getline(userFile, _arrayBuffer[i]);
	
	_currentLine = 1;
	_currentChar = 1;
	_usedLines = _capacity;
	userFile.close();
}

XIEditor::~XIEditor() {}

void XIEditor::start() {
	modeCommand();
}

//This method prints all visible content
void XIEditor::printLines()
{
	for (int i = 1; i < _usedLines+1; i++)
	{
		//This makes _currentLine relative to '*'
		if (i == _currentLine)
		{
			cout << '*';
			cout << _listBuffer.getEntry(i) << endl;
			//This makes current position relative to '^'
			for (int j = 0; j < _currentChar; j++)
			{
				cout << ' ';
			}
			cout << '^' << endl;
		}
		else
		{
			cout << ' ';
			cout << _listBuffer.getEntry(i) << endl;
		}
	}
	//when there is nothing to print
	if (_usedLines == 0)
		cout << "*" << endl << " ^";
}

void XIEditor::goRight() {
	_currentChar++;
	//if not at last line
	if (_currentLine < _capacity)
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

/*
bool XIEditor::resize(int resizeTo) {
	if (resizeTo<0)
		return false;
	std::string *temp = new std::string[_capacity];
	
	int itemsCopied = 0;
	for (int i = 0; i < _capacity && i < resizeTo && i < _usedLines; i++) {
		temp[i] = _arrayBuffer[i];
		itemsCopied++;
	}

	_arrayBuffer = new std::string[resizeTo];

	for (int i = 0; i < itemsCopied; i++)
		_arrayBuffer[i] = temp[i];
	
	_capacity = resizeTo;
	_usedLines = itemsCopied;
	return true;
}
*/

void XIEditor::deleteLine(int deleteHere) {
	//_arrayBuffer[deleteHere] = "";
	//for (int i = deleteHere; i < _usedLines - 1; i++)
	//	_arrayBuffer[i] = _arrayBuffer[i + 1];
	_listBuffer.remove(deleteHere);
	_usedLines--;
}

void XIEditor::insertLine(std::string line, int insertHere) {
	/*
	if (_usedLines == _capacity)
		resize(_capacity+1);

	for (int i = _usedLines - 1; i > insertHere; i--)
		_arrayBuffer[i] = _arrayBuffer[i - 1];
	_arrayBuffer[insertHere] = line;
	*/
	_usedLines++;
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
	if (_currentLine > _usedLines) {
		if (_usedLines != 0)
			_currentLine = _usedLines;
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

std::string XIEditor::modeInsert() {
	std::string input;
	std::string fullInput = "";
	bool notEsc = true;
	while (notEsc) 
	{
		system("cls");
		printLines();
		input = _getch();
		
		switch (input[0])
		{
			case KeyCode::ESC: {
				notEsc = false;
				break;
			}
			//this case takes care of special keys like delete and arrow keys
			case 'à': {
				//certain special keys read in 2 characters, not just one.
				//these keys first read in à then another letter.
				//this extra getch takes in the second part of the special key
				char const arrowLeft = 'K', arrowRight = 'M', arrowUp = 'H', arrowDown = 'P', del = 'S';
				char secChar = _getch();	
				switch (secChar) {
					case del: {
						std::string change = _listBuffer.getEntry(_currentLine).erase(_currentChar, 1);
						_listBuffer.replace(_currentLine, change);
						break;
					}
					case arrowRight: {
						_currentChar++;
						int currentLineLength = _listBuffer.getEntry(_currentLine).length();
						//for going too far right
						if (_currentChar > currentLineLength + 1)
							_currentChar = currentLineLength+1;
						break;
					}
					case arrowLeft: {
						_currentChar--;
						stayInText();
						break;
					}
					case arrowUp: {
						_currentLine--;
						stayInText();
						break;
					}
					case arrowDown: {
						_currentLine++;
						stayInText();
						break;
					}
				}
				
				break;
			}
			case '\b': {
				_currentChar--;
				if (!stayInText()) {
					std::string change = _listBuffer.getEntry(_currentLine).erase(_currentChar, 1);
					_listBuffer.replace(_currentLine, change);
				}
				else if (_currentChar == 1){
					std::string change = _listBuffer.getEntry(_currentLine).erase(0, 1);
					_listBuffer.replace(_currentLine, change); 
				}
				break;
			}
			case '\r': {
				//copy text to right of cursor
				std::string temp = _listBuffer.getEntry(_currentLine).substr(_currentChar-1,
					_listBuffer.getEntry(_currentLine).length() - _currentChar+1);

				//delete text to right of cursor
				std::string cut = _listBuffer.getEntry(_currentLine).erase(_currentChar-1,
					_listBuffer.getEntry(_currentLine).length() - _currentChar+1);

				_listBuffer.replace(_currentLine, cut);
				
				insertLine(temp, ++_currentLine);
				_currentChar = 1;
				//stayInText();
				//_arrayBuffer[_currentLine].insert(_currentChar, temp);
				break;
			}
			default: {
				if (input[0] != '\b') {
					fullInput += input;
					input = input[0];
					std::string edited = _listBuffer.getEntry(_currentLine).insert((_currentChar++)-1, input);
					_listBuffer.replace(_currentLine, edited);
				}
				break;
			}
		}
	}
	return fullInput;
}

void XIEditor::modeCommand() {
	char userInput;

	bool notEsc = true;
	while (notEsc)
	{
		system("cls");
		printLines();
		userInput = _getch();

		switch (userInput)
		{
				//move up
			case KeyCode::UP:
			{
				_currentLine--;
				//if (!stayInText())
				_commands.push(CommandPlus(KeyCode::UP, _currentChar));
				break;
			}
			//move down
			case KeyCode::DOWN:
			{
				_currentLine++;
				//if (!stayInText())
				_commands.push(CommandPlus(KeyCode::DOWN, _currentChar));
				break;
			}

			//move right
			case KeyCode::RIGHT:
			{
				goRight();
				//if (!stayInText())
				_commands.push(CommandPlus(KeyCode::RIGHT));
				break;
			}
			//move left
			case KeyCode::LEFT:
			{
				goLeft();
				//if (!stayInText())
				_commands.push(CommandPlus(KeyCode::LEFT));
				break;
			}
			case KeyCode::QUIT_1:
			{
				if (_getch() == KeyCode::QUIT_2) {
					notEsc = false;
				}
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
						std::string(1, _listBuffer.getEntry(_currentLine)[_currentChar-1]))
					);
					std::string changed = _listBuffer.getEntry(_currentLine).erase(_currentChar-1, 1);
					_listBuffer.replace(_currentLine, changed);
				}
				break;
			}
			case KeyCode::DEL_LINE: {
				//push to stack the command and the line being deleted

				if (_getch() == KeyCode::DEL_LINE && _usedLines > 1) {
					_commands.push(CommandPlus(KeyCode::DEL_LINE, _listBuffer.getEntry(_currentLine)));
					deleteLine(_currentLine);
				}
				break;
			}
			case KeyCode::UNDO: {
				undo();
				break;
			}
			case KeyCode::INSERT_ABOVE: {
				_currentChar = 0; //set cursor to beginning of the line
				_commands.push(CommandPlus(KeyCode::INSERT_ABOVE));
				insertLine("", _currentLine);
				modeInsert();
				break;
			}
			case KeyCode::INSERT_BELOW: {
				_commands.push(CommandPlus(KeyCode::INSERT_BELOW));
				insertLine("", ++_currentLine);
				modeInsert();
				break;
			}
			case KeyCode::INSERT_HERE: {
				std::string insertedText = modeInsert();
				int startingLocation = _currentChar;
				_commands.push(CommandPlus(KeyCode::INSERT_HERE, insertedText, startingLocation));
				break;
			}
			case KeyCode::INSERT_START: {
				_currentChar = 0;
				int startingLocation = _currentChar;
				std::string insertedText = modeInsert();
				_commands.push(CommandPlus(KeyCode::INSERT_START, insertedText, startingLocation));
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
			goLeft();
			_commands.pop();
			break;
		}
		case KeyCode::LEFT: {
			goRight();
			_commands.pop();
			break;
		}
		case KeyCode::DEL_CHAR: {
			std::string edited = _listBuffer.getEntry(_currentLine).insert(_currentChar-1,
															_commands.peek().getChange());
			_listBuffer.replace(_currentLine, edited);
			_commands.pop();
			break;
		}
		case KeyCode::DEL_LINE: {
			insertLine(_commands.peek().getChange(), _currentLine);
			_commands.pop();
			break;
		}
		case KeyCode::INSERT_ABOVE: {
			deleteLine(_currentLine);
			_commands.pop();
			break;
		}
		default: return false;
	}
	return true;
}