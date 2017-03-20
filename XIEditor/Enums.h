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
		ESC_1 = ';',
		ESC_2 = 'q',
		UNDO = 'u',
		INSERT_BELOW = 'o',
		INSERT_ABOVE = 'O'
	};
}

namespace Action
{//for recording actions in Command
	enum Action {
		UP,
		DOWN,
		RIGHT,
		LEFT,
		DEL_CHAR,
		DEL_LINE,
		INSERT_ABOVE
	};
}
#endif