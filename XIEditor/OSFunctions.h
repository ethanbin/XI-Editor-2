#ifndef OSFUNCS
#define OSFUNCS



//Windows 32-bit and 64-bit
#ifdef _WIN32
	#include <Windows.h>
	inline void clrscrn() {
		system("cls");
	}

	inline void consoleBlackOnWhite() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF0);
	}

	inline void consoleWhiteOnBlack() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	}

	//takes int x and y for coordinates to move cursor to.
	inline void moveCursorTo(int x, int y) {
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
	inline void clrscrn() {}

	inline void consoleBlackOnWhite() {}

	inline void consoleWhiteOnBlack() {}

	//takes int x and y for coordinates to move cursor to.
	inline void cursor(int x, int y) {}
	#else
		#error "Unknown Apple platform"
	#endif

//linux
#elif __linux__
	inline void clrscrn() {}

	inline void consoleBlackOnWhite() {}

	inline void consoleWhiteOnBlack() {}
	
	//takes int x and y for coordinates to move cursor to.
	inline void cursor(int x, int y) {}
//unix
#elif __unix__
	inline void clrscrn() {}

	inline void consoleBlackOnWhite() {}

	inline void consoleWhiteOnBlack() {}
	
	//takes int x and y for coordinates to move cursor to.
	inline void cursor(int x, int y) {}
#endif


#endif //!OSFUNCS