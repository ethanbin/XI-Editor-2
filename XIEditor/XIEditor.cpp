#include "XIEditor.h"
#include "Enums.h"
#include <iostream>
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
	for (; !userFile.eof(); _capacity++)
		getline(userFile, lineCollector);

	userFile.clear();
	userFile.seekg(0);

	_arrayBuffer = new std::string[_capacity];

	for (int i = 0; !userFile.eof(); i++)
		getline(userFile, _arrayBuffer[i]);

	_currentLine = 0;
	_currentChar = 0;
	_usedLines = _capacity;
	userFile.close();
}

XIEditor::~XIEditor() {
	delete[] _arrayBuffer;
}

//This method prints all visible content
void XIEditor::printLines()
{
	for (int i = 0; i < _usedLines; i++)
	{
		//This makes _currentLine relative to '*'
		if (i == _currentLine)
		{
			cout << '*';
			cout << _arrayBuffer[i] << endl;
			cout << ' ';
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
			cout << _arrayBuffer[i] << endl;
		}
	}
}

void XIEditor::userInput() {
	char userInput = _getch();

	switch (userInput)
	{
			//move up
		case KeyCode::UP:
		{
			_currentLine--;
			_commands.push(Command(Action::UP));
			break;
		}
		//move down
		case KeyCode::DOWN:
		{
			_currentLine++;
			_commands.push(Command(Action::DOWN));
			break;
		}

		//move right
		case KeyCode::RIGHT:
		{
			goRight();
			_commands.push(Command(Action::RIGHT));
			break;
		}
		//move left
		case KeyCode::LEFT:
		{
			goLeft();
			_commands.push(Command(Action::LEFT));
			break;
		}
		case KeyCode::ESC_1:
		{
			if (_getch() == KeyCode::ESC_2) {
				exit(EXIT_SUCCESS);
			}
			break;
		}
		case KeyCode::DEL_CHAR:
		{
			//if the string is empty (no char to delete), delete the line.
			if (_arrayBuffer[_currentLine] == "") {
				deleteLine(_currentLine);
				_commands.push(Command(Action::DEL_CHAR, std::string("")));
			}

			else {
				//push to stack the command and the letter being deleted
				_commands.push(Command(Action::DEL_CHAR,
					std::string(1, _arrayBuffer[_currentLine][_currentChar]))
				);
				_arrayBuffer[_currentLine].erase(_currentChar, 1);
			}
			break;
		}
		case KeyCode::DEL_LINE: {
			//push to stack the command and the line being deleted

			if (_getch() == KeyCode::DEL_LINE && _usedLines > 0) {
				_commands.push(Command(Action::DEL_LINE, _arrayBuffer[_currentLine]));
				deleteLine(_currentLine);
			}
			break;
		}
		case KeyCode::INSERT_ABOVE: {
			insertLine("", _currentLine);
			break;
		}
		case KeyCode::UNDO: {
			undo();
			break;
		}
	}
	stayInText();
}

void XIEditor::goRight() {
	_currentChar++;
	//if not at last line
	if (_currentLine < _capacity - 1)
		//move cursor to start of next line when going too far right
		if (_currentChar >= _arrayBuffer[_currentLine].length()) {
			_currentChar = 0;
			_currentLine++;
		}
	}

void XIEditor::goLeft() {
	_currentChar--;
	//if at first line, keep cursor from going too far left
	if (_currentLine <= 0) {
		_currentLine = 0;
	}
	//if not at first line
	if (_currentLine > 0)
		//move cursor to end of previous line when going too far left
		if (_currentChar < 0) {
			_currentLine--;
			_currentChar = _arrayBuffer[_currentLine].length() - 1;
		}
}

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

void XIEditor::deleteLine(int deleteHere) {
	for (int i = deleteHere; i < _usedLines - 1; i++)
		_arrayBuffer[i] = _arrayBuffer[i + 1];
	resize(_capacity - 1);
	_usedLines--;
}

void XIEditor::insertLine(std::string line, int insertHere) {
	if (_usedLines == _capacity)
		resize(_capacity+1);
	_usedLines++;

	for (int i = _usedLines - 1; i > insertHere; i--)
		_arrayBuffer[i] = _arrayBuffer[i - 1];
	_arrayBuffer[insertHere] = line;
}

bool XIEditor::stayInText() {
	int currentLineLength = _arrayBuffer[_currentLine].length();
	bool isCorrected = false;
	//for going too far up
	if (_currentLine < 0) {
		_currentLine = 0;
		isCorrected = true;
	}
	//for going too far down
	if (_currentLine >= _usedLines) {
		_currentLine = _usedLines - 1;
		isCorrected = true;
	}
	//for going too far right
	if (_currentChar >= currentLineLength) {
		_currentChar = currentLineLength - 1;
		isCorrected = true;
	}
	//for going too far left
	if (_currentChar < 0) {
		_currentChar = 0;
		isCorrected = true;
	}
	return isCorrected;
}

bool XIEditor::undo() {
	if (_commands.isEmpty())
		return false;
	//will undo an action
	switch (_commands.peek().getAction())
	{
		case Action::UP: {
			_currentLine++;
			_commands.pop();
			break;
		}
		case Action::DOWN: {
			_currentLine--;
			_commands.pop();
			break;
		}
		case Action::RIGHT: {
			goLeft();
			_commands.pop();
			break;
		}
		case Action::LEFT: {
			goRight();
			_commands.pop();
			break;
		}
		case Action::DEL_CHAR: {
			_arrayBuffer[_currentLine].insert(_currentChar,_commands.peek().getChange());
			_commands.pop();
			break;
		}
		case Action::DEL_LINE: {
			insertLine(_commands.peek().getChange(), _currentLine);
			_commands.pop();
			break;
		}
		default: return false;
	}
	return true;
}