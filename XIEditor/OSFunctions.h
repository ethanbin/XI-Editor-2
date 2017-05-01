#ifndef OSFUNC_
#define OSFUNC_

#pragma once
//Windows 32-bit and 64-bit
#ifdef _WIN32
	#include <Windows.h>
	void clrscrn() {
		system("cls");
	}

	void consoleBlackOnWhite() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF0);
	}

	void consoleWhiteOnBlack() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	}

	//takes int x and y for coordinates to move cursor to.
	void moveCursorTo(int x, int y) {
		COORD coord;
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(
			GetStdHandle(STD_OUTPUT_HANDLE),
			coord);
	}
#elif __APPLE__
#include "TargetConditionals.h"
	//mac OS
	#if TARGET_OS_MAC
	void clrscrns() {}

	void consoleBlackOnWhite() {}

	void consoleWhiteOnBlack() {}

	//takes int x and y for coordinates to move cursor to.
		void cursor(int x, int y) {}
	#else
		#error "Unknown Apple platform"
	#endif

//linux
#elif __linux__
	void clrscrn() {}

	void consoleBlackOnWhite() {}

	void consoleWhiteOnBlack() {}
	
	//takes int x and y for coordinates to move cursor to.
	void cursor(int x, int y) {}
//unix
#elif __unix__
	void clrscrn() {}

	void consoleBlackOnWhite() {}

	void consoleWhiteOnBlack() {}	
	
	//takes int x and y for coordinates to move cursor to.
	void cursor(int x, int y) {}
#endif
#endif