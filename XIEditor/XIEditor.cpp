#include "XIEditor.h"
#include <iostream>
#include <fstream>

XIEditor::XIEditor() {
	_numLines = 3;
	_arrayBuffer = new std::string[_numLines];
}

XIEditor::XIEditor(std::string fileName) {
	_numLines = 0;

	std::ifstream userFile;
	userFile.open(fileName);

	if (!userFile.is_open()) { //throw;
		std::cout << "ERROR: No File Found." << std::endl;
		userFile.close();
		exit(EXIT_FAILURE);
	}

	std::string lineCollector;
	for (; !userFile.eof(); _numLines++)
		getline(userFile, lineCollector);
	userFile.clear();
	userFile.seekg(0);

	_arrayBuffer = new std::string[_numLines];

	for (int i = 0; !userFile.eof(); i++)
		getline(userFile, _arrayBuffer[i]);

	userFile.close();
}

XIEditor::~XIEditor() {
	delete[] _arrayBuffer;
}

std::string XIEditor::displayText() {
	std::string textResult = "";
	//loop for however many lines there are
	for (int i = 0; i < _numLines; i++) {
		//if we are about to output the current line the user is on
		if (i == _currentLine) {
			textResult += "*" + _arrayBuffer[i] + "\n";
			//then to output caret, pushed char the user is on by spaces
			for (int i = 0; i < _currentChar; i++)
				textResult += " ";

			//add extra space to account for asterisk shifting the above line
			textResult += " ";
			textResult = textResult + "^" + "\n";
		}

		else
			textResult += " " + _arrayBuffer[i] + "\n";
	}
	return textResult;
}

void XIEditor::userInput(char userInput) {
	_userInput = userInput;
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
			if (_currentLine < _numLines-1)
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
		case KeyCode::ESC:
		{
			exit(EXIT_SUCCESS);
			break;
		}
		case KeyCode::DEL_CHAR:
		{
			_arrayBuffer[_currentLine].erase(_currentChar,1);
			//so we arent left with blank line, make if statement to call deleteLine (will be made) when string is empty
			break;
		}
	}
	stayInText();
}

void XIEditor::stayInText() {
	int currentLineLength = _arrayBuffer[_currentLine].length();

	//for going too far up
	if (_currentLine < 0)
		_currentLine = 0;
	//for going too far down
	if (_currentLine >= _numLines)
		_currentLine = _numLines - 1;
	//for going too far right
	if (_currentChar >= currentLineLength)
		_currentChar = currentLineLength - 1;
	//for going too far left
	if (_currentChar < 0)
		_currentChar = 0;
}