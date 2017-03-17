#include "StackInterface.h"
//#include "ArrayStack.h"
#include "LinkedStack.h"
#include "Command.h"
#include <fstream>
#include <string>

class XIEditor {
	public: //functions

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

	private: //functions

		//move right. if going right goes past text, go to beginning of next line.
		void goRight();

		//move left. if going left goes past text, go to end of previous line.
		void goLeft();

		//resizes an array to a given size
		//if the given size is smaller than the current capacity, then addresses beyond resizeTo are lost.
		//after resizing, usedLines is set to however many items were copied back into the array buffer.
		bool resize(int);

		//deletes a line at the given location
		void deleteLine(int);
		
		//inserts a new line on indicated line.
		//whatever was on this line is pushed to the next line.
		void insertLine(std::string, int);

		//prevents the carrot from going too far right 
		//returns true if the function corrected the cursor position
		bool stayInText();

		//undo last command commited by user. commands are tracked by stack
		bool undo();

	private: //variables
		LinkedStack<Command> _commands;
		std::string *_arrayBuffer;
		int _capacity, _usedLines, _currentLine=0, _currentChar=0;
};