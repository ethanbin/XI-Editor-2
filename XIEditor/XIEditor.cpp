#include "XIEditor.h"
#include "Enums.h"
#include <iostream>
#include<string>
#include <fstream>
#include <conio.h>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;

XIEditor::XIEditor(std::string fileName) {
	_capacity = 0;
	_fileName = fileName;
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

bool XIEditor::deleteLine(int deleteHere) {
	if (deleteHere < 1)
		return false;

	if (_usedLines > 1) {
		_listBuffer.remove(deleteHere);
		_usedLines--;
	}
	else if (_usedLines == 1) {
		_listBuffer.replace(deleteHere, "");
		_usedLines = 1;
	}
	return true;
}

void XIEditor::insertLine(std::string line, int insertHere) {
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
			case KeyCode::FUNC_KEY: {
				//certain special keys read in 2 characters, not just one.
				//these keys first read in à then another letter.
				//this extra getch takes in the second part of the special key
				char secChar = _getch();	
				switch (secChar) {
				case KeyCode::FUNC_DELETE: {
						if (_listBuffer.getEntry(_currentLine).length() > 0) {
							std::string change = _listBuffer.getEntry(_currentLine).erase(_currentChar, 1);
						_listBuffer.replace(_currentLine, change);
						break;
						}
					}
					case KeyCode::FUNC_ARROW_RIGHT: {
						_currentChar++;
						int currentLineLength = _listBuffer.getEntry(_currentLine).length();
						//for going too far right
						if (_currentChar > currentLineLength + 1)
							_currentChar = currentLineLength+1;
						break;
					}
					case KeyCode::FUNC_ARROW_LEFT: {
						_currentChar--;
						stayInText();
						break;
					}
					case KeyCode::FUNC_ARROW_UP: {
						_currentLine--;
						stayInText();
						break;
					}
					case KeyCode::FUNC_ARROW_DOWN: {
						_currentLine++;
						stayInText();
						break;
					}
				}
				
				break;
			}
			case KeyCode::BACKSPACE: {
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
			}
			default: {
				if (input[0] != KeyCode::BACKSPACE) {
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

bool XIEditor::modeLastLine() {
	std::string quit = "q", write = "w", writeQuit = "wq";
	cout << "\n\n\n";
	cout << ":";
	std::string input;
	cin >> input;
	if (input == quit)
		return false;
	if (input == write){
		ofstream txtFile;
		txtFile.open(_fileName);
		for (int i = 1; i <= _listBuffer.getLength(); i++)
			txtFile << _listBuffer.getEntry(i) << endl;
		txtFile.close();
	}
	if (input == writeQuit) {
		ofstream txtFile;
		txtFile.open(_fileName);

		//print to file and prevent extra endline at end of file
		for (int i = 1; i < _usedLines; i++)
			txtFile << _listBuffer.getEntry(i) << endl;
		txtFile << _listBuffer.getEntry(_usedLines);

		txtFile.close();
		return false;
	}
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
				if (_currentLine != 0)
					_commands.push(CommandPlus(KeyCode::UP, _currentChar));
				break;
			}
			//move down
			case KeyCode::DOWN:
			{
				_currentLine++;
				if (_currentLine != _usedLines + 1)
					_commands.push(CommandPlus(KeyCode::DOWN, _currentChar));
				break;
			}

			//move right
			case KeyCode::RIGHT:
			{
				goRight();
				if (!stayInText())
					_commands.push(CommandPlus(KeyCode::RIGHT));
				break;
			}
			//move left
			case KeyCode::LEFT:
			{
				goLeft();
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
						std::string(1, _listBuffer.getEntry(_currentLine)[_currentChar-1]))
					);
					std::string changed = _listBuffer.getEntry(_currentLine).erase(_currentChar-1, 1);
					_listBuffer.replace(_currentLine, changed);
				}
				break;
			}
			case KeyCode::DEL_LINE: {
				//push to stack the command and the line being deleted
				if (_getch() == KeyCode::DEL_LINE && _usedLines >= 0) {
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
				_commands.push(CommandPlus(KeyCode::INSERT_ABOVE,_currentChar));
				_currentChar = 0; //set cursor to beginning of the line
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