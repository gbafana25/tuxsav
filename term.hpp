#include <termios.h>

class TermSettings {
	public:

	struct termios def_options;
	struct termios mod_options;

	void setupTerminal();
	void resetTerminal();
};
