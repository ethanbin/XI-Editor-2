#ifndef OSFUNCS
#define OSFUNCS

//Windows 32-bit and 64-bit
#ifdef _WIN32
	#include <Windows.h>
	inline void clrscrn() {
		system("cls");
	}
	//hex color is in order of intensity, red, green, blue
	//higher 4 bits are background, lower 4 are text
	inline void consoleBlackOnWhite() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF0);
	}

	inline void consoleWhiteOnBlack() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	}

	inline void consoleWhiteOnRed() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xCF);
	}

	inline void consoleBlueOnWhite() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF9);
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

	inline void wait(int milliSeconds) {
		Sleep(milliSeconds);
	}
	;
#elif __APPLE__
#include "TargetConditionals.h"
	//mac OS
	#if TARGET_OS_MAC
	inline void clrscrn() {}

	inline void consoleBlackOnWhite() {}

	inline void consoleWhiteOnBlack() {}

	inline void consoleWhiteOnRed() {}

	inline void consoleBlueOnWhite() {}

	//takes int x and y for coordinates to move cursor to.
	inline void cursor(int x, int y) {}

	inline void wait(int milSeconds) {}
	#else
		#error "Unknown Apple platform"
	#endif

//linux
#elif __linux__
	inline void clrscrn() {}

	inline void consoleBlackOnWhite() {}

	inline void consoleWhiteOnBlack() {}
	
	inline void consoleWhiteOnRed() {}

	inline void consoleBlueOnWhite() {}

	//takes int x and y for coordinates to move cursor to.
	inline void cursor(int x, int y) {}

	inline void wait(int milSeconds) {}
//unix
#elif __unix__
	inline void clrscrn() {}

	inline void consoleBlackOnWhite() {}

	inline void consoleWhiteOnBlack() {}

	inline void consoleWhiteOnRed() {}

	inline void consoleBlueOnWhite() {}
	
	//takes int x and y for coordinates to move cursor to.
	inline void cursor(int x, int y) {}

	inline void wait(int milliSeconds) {
		//this function uses microseconds, so we *1000 to get milliseconds
		usleep(milliSeconds * 1000);
	}
#endif


#endif //!OSFUNCS