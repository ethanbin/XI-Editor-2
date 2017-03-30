//Source.cpp - Brian Keegan & Ethan Binyaminov CSC 326 
#include <iostream>
#include <string>
#include "XIEditor.h"

void main(int argc, char *argv[]) {
	XIEditor editor(argv[1]);

	std::string startMsg = std::string("\t\t\t\t-- XI-Editor --\n") +
		";q to quit, " +
		"k to move up, " +
		"j to move down, " +
		"l to move right, " +
		"h to move left.\n\n\n";

	system("cls");

	editor.commandMode();
}