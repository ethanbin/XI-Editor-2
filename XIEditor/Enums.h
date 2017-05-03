#ifndef ENUMS_
#define ENUMS_

//Windows 32-bit and 64-bit
#ifdef _WIN32
	#ifndef ENUMS
	#define ENUMS
	namespace KeyCode
	{//for comparing input
	 //commands using 2 different keys will be named with a 1 and 2, 1 being required first.
		enum KeyCode {
			UP = 'k',
			DOWN = 'j',
			RIGHT = 'l',
			LEFT = 'h',
			DEL_CHAR = 'x',
			DEL_LINE = 'd',
			UNDO = 'u',
			UNDO_ALL = 'U',
			INSERT_BELOW = 'o',
			INSERT_ABOVE = 'O',
			INSERT_HERE = 'i',
			INSERT_START = 'I',
			LAST_LINE = ':',

			ESC = '\x1b',
			BACKSPACE = '\b',
			ENTER = '\r',
			RETURN = '\r',
			FUNC_KEY = 'à',
			FUNC_ARROW_LEFT = 'K',
			FUNC_ARROW_RIGHT = 'M',
			FUNC_ARROW_UP = 'H',
			FUNC_ARROW_DOWN = 'P',
			FUNC_DELETE = 'S'
		};
	}
	#endif
#elif __APPLE__
#include "TargetConditionals.h"
	//mac OS
	#if TARGET_OS_MAC
		#ifndef ENUMS
		#define ENUMS
		namespace KeyCode
		{//for comparing input
		 //commands using 2 different keys will be named with a 1 and 2, 1 being required first.
			enum KeyCode {
				UP = 'k',
				DOWN = 'j',
				RIGHT = 'l',
				LEFT = 'h',
				DEL_CHAR = 'x',
				DEL_LINE = 'd',
				QUIT_1 = ':',
				QUIT_2 = 'q',
				UNDO = 'u',
				INSERT_BELOW = 'o',
				INSERT_ABOVE = 'O',
				INSERT_HERE = 'i',
				INSERT_START = 'I',
				};
			}
		#endif
	#else
		#error "Unknown Apple platform"
	#endif

//linux
#elif __linux__
	#ifndef ENUMS
	#define ENUMS
	namespace KeyCode
	{//for comparing input
	 //commands using 2 different keys will be named with a 1 and 2, 1 being required first.
		enum KeyCode {
			UP = 'k',
			DOWN = 'j',
			RIGHT = 'l',
			LEFT = 'h',
			DEL_CHAR = 'x',
			DEL_LINE = 'd',
			QUIT_1 = ':',
			QUIT_2 = 'q',
			UNDO = 'u',
			INSERT_BELOW = 'o',
			INSERT_ABOVE = 'O',
			INSERT_HERE = 'i',
			INSERT_START = 'I',
		};
	}
	#endif
//unix
#elif __unix__
	#ifndef ENUMS
	#define ENUMS
	namespace KeyCode
	{//for comparing input
		//commands using 2 different keys will be named with a 1 and 2, 1 being required first.
		enum KeyCode {
			UP = 'k',
			DOWN = 'j',
			RIGHT = 'l',
			LEFT = 'h',
			DEL_CHAR = 'x',
			DEL_LINE = 'd',
			QUIT_1 = ':',
			QUIT_2 = 'q',
			UNDO = 'u',
			INSERT_BELOW = 'o',
			INSERT_ABOVE = 'O',
			INSERT_HERE = 'i',
			INSERT_START = 'I',
		};
	}
	#endif
#endif
#endif