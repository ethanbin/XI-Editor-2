#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include "XIEditor.h"

void main(int argc, char *argv[]) {
	XIEditor editor(argv[1]);
	char userEntry;

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

		std::cout << editor.displayText();
		userEntry = _getch();
		editor.userInput(userEntry);
	}
}