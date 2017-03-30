//Source.cpp - Brian Keegan & Ethan Binyaminov CSC 326 
#include <iostream>
#include <string>
#include "XIEditor.h"

using std::cout;
using std::endl;

void main(int argc, char *argv[]) {
	if (argc >= 2) {
		XIEditor editor(argv[1]);
		system("cls");

		editor.start();
	}
	else
		cout << "Error: No file specified." << endl;
}