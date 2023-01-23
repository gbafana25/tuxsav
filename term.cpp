#include <termios.h>
#include <unistd.h>

#include "term.hpp"

void TermSettings::setupTerminal() {
	tcgetattr(STDIN_FILENO, &this->def_options);
	this->mod_options = this->def_options;
	this->mod_options.c_lflag &= ~(ICANON);
	this->mod_options.c_lflag &= ~(ECHO);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &this->mod_options);
}

void TermSettings::resetTerminal() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &this->def_options);

}
