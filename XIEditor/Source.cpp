//Source.cpp - Brian Keegan & Ethan Binyaminov CSC 326 
#include <iostream>
#include <string>
#include "XIEditor.h"

void main(int argc, char *argv[]) {
	XIEditor editor(argv[1]);

	system("cls");

	editor.start();
}