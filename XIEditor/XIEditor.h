#pragma once
#include <fstream>
#include <string>

class XIEditor {
	public:

		/*constructor opens file fileName, counts lines, 
		makes dynamic array from arrayBuffer,
		then reads in each line to arrayBuffer.*/
		XIEditor(std::string);

		~XIEditor();

		//returns string with file's text with a star and carrot as a cursor
		void printLines();

		/*accepts char and compares it to KeyCode, 
		moves index and/or carrot depending on input,
		ensures cursor does not go beyond the text*/
		void userInput();

	private: //functions to be used by public member functions only

		//prevents the carrot from going too far right
		void stayInText();

		//for comparing input with in userInput()
		enum KeyCode {
			UP = 'k',
			DOWN = 'j',
			RIGHT = 'l',
			LEFT = 'h',
			ESC = '\x1b',
			DEL_CHAR = 'x',
			DEL_LINE = 'd'
		};

	private:
		std::string *_arrayBuffer;
		int _capacity, _currentLine=0, _currentChar=0;
};