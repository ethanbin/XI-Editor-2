//Source.cpp - Brian Keegan & Ethan Binyaminov CSC 326 
#include <iostream>
#include <string>
#include "XIEditor.h"

void main(int argc, char *argv[]) {
	XIEditor editor(argv[1]);

	std::string startMsg = std::string("\t\t\t\t-- XI-Editor --\n") + 
		"Esc to quit, " +
		"K to move up, " +
		"J to move down, " +
		"L to move right, " +
		"H to move left.\n\n\n";

	while (true)
	{
		system("cls");
		std::cout << startMsg;

		editor.printLines();
		editor.userInput();
	}
}