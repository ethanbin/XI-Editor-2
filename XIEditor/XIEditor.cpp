#include "XIEditor.h"
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
	userFile.close();
}

XIEditor::~XIEditor() {
	delete[] _arrayBuffer;
}

//This method prints all visible content
void XIEditor::printLines()
{
	for (int i = 0; i < _capacity; i++)
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
	switch (tolower(userInput))
	{
			//move up
		case KeyCode::UP:
		{
			_currentLine--;
			break;
		}
		//move down
		case KeyCode::DOWN:
		{
			_currentLine++;
			break;
		}

		//move right
		case KeyCode::RIGHT:
		{
			_currentChar++;
			//if not at last line
			if (_currentLine < _capacity-1)
				//move cursor to start of next line when going too far right
				if (_currentChar >= _arrayBuffer[_currentLine].length()) {
					_currentChar = 0;
					_currentLine++;
				}
			break;
		}
		//move left
		case KeyCode::LEFT:
		{
			_currentChar--;
			//if at first line, keep cursor from going too far left
			if (_currentLine <= 0) {
				_currentLine = 0;	
			}
			//if not at first line
			if(_currentLine>0)
				//move cursor to end of previous line when going too far left
				if (_currentChar < 0) {
					_currentLine--;
					_currentChar = _arrayBuffer[_currentLine].length() - 1;
				}
			break;
		}
		case KeyCode::ESC_1:
		{
			if (_getch() == ESC_2)
				exit(EXIT_SUCCESS);
			break;
		}
		case KeyCode::DEL_CHAR:
		{
			_arrayBuffer[_currentLine].erase(_currentChar,1);
			//so we arent left with blank line, make if statement to call deleteLine (will be made) when string is empty
			break;
		}
		case KeyCode::DEL_LINE: {
			if (_getch() == KeyCode::DEL_LINE)
				deleteLine(_currentLine);
			break;
		}
	}
	stayInText();
}

void XIEditor::deleteLine(int lineToDel) {
	for (int i = lineToDel; i < _capacity - 1; i++)
		_arrayBuffer[i] = _arrayBuffer[i + 1];
	resize(_capacity - 1);
}

void XIEditor::stayInText() {
	int currentLineLength = _arrayBuffer[_currentLine].length();

	//for going too far up
	if (_currentLine < 0)
		_currentLine = 0;
	//for going too far down
	if (_currentLine >= _capacity)
		_currentLine = _capacity - 1;
	//for going too far right
	if (_currentChar >= currentLineLength)
		_currentChar = currentLineLength - 1;
	//for going too far left
	if (_currentChar < 0)
		_currentChar = 0;
}