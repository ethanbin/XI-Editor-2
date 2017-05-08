#ifndef  XIEDITOR_
#define  XIEDITOR_

#include "StackInterface.h"
#include "LinkedList.h"
#include "LinkedStack.h"
#include "Command.h"
#include <fstream>
#include <string>
#include "OSFunctions.h"

class XIEditor {
	public: //functions

		/*constructor opens file fileName, counts lines, 
		and reads in each line to list Buffer.*/
		XIEditor(std::string);

		~XIEditor();

		void start();

	private: //functions
		//overwrites file given when creating this instance of XIEditor with
		//what is currently displayed.
		bool save();

		//overwrites given file/path with  what is currently displayed.
		bool save(std::string);

		//returns string with file's text with a star and carrot as a cursor
		void printLines();

		//move right. if going right goes past text, go to beginning of next line.
		void goRight();

		//move left. if going left goes past text, go to end of previous line.
		void goLeft();

		//deletes the node for a line at the given location.
		//when deleting the last remaining line/node, 
		//	the string of the node is replaced with an empty string.
		//returns true if line to delete is greater than 1.
		bool deleteLine(int);
		
		//inserts a new line on indicated line.
		//whatever was on this line is pushed to the next line.
		void insertLine(std::string, int);

		//prevents the carrot from going too far right.
		//returns true if the function corrected the cursor position.
		bool stayInText();

		//enters insert mode. 
		//Here, a user can insert text or delete text (with backspace or delete)
		//takes int originalCharPos
		//returns a string so the stack can know what to undo
		void modeInsert(int);

		//enters last line mode.
		//can execute powerful commands, such as write (save), quit, etc.
		//returns false if quit command used
		bool modeLastLine();

		/*accepts char and compares it to KeyCode,
		moves index and/or carrot depending on input,
		ensures cursor does not go beyond the text*/
		void modeCommand();

		//undo last command commited by user. commands are tracked by stack
		bool undo();

	private: //variables
		std::string _fileName;
		LinkedStack<CommandPlus> _commands;
		LinkedList<std::string> _listBuffer;
		int _capacity, _currentLine=1, _currentChar=1;
};
#endif