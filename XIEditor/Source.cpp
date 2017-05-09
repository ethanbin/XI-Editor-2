//Source.cpp - Brian Keegan & Ethan Binyaminov CSC 326 
#include <iostream>
#include <string>
#include "XIEditor.h"
#include "OSFunctions.h"

using std::cout;
using std::endl;

void main(int argc, char *argv[]) {
	if (argc >= 2) {
		XIEditor editor(argv[1]);
		editor.start();
	}
	else {
		XIEditor editor;
		editor.start();
	}
		//cout << "Error: No file specified." << endl;
}