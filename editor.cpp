#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "editor.hpp"

void Editor::inputLoop() {
	this->current_mode = Editor::NAV;

	write(STDOUT_FILENO, "\e[2J", 4);
	write(STDOUT_FILENO, "\e[H", 3);
	while(true) {
		read(STDIN_FILENO, &this->raw, 1); 
		if(this->raw == 'q' and this->current_mode == Editor::NAV) {
			break;
		} else if(this->raw == '\n') {
			//std::cout << this->buffer << std::endl;
			//this->buffer = "";
			write(STDOUT_FILENO, "\n", 2);
		} else if(this-> raw == 'i' && this->current_mode == Editor::NAV) {
			this->current_mode = Editor::EDIT;
		} else if(this->current_mode == Editor::NAV) {
			scanNavFunctions();
		} else if(this->raw == '\033') {
			this->current_mode = Editor::NAV;
		} else if(this->raw == 127) {
			
			write(STDOUT_FILENO, "\b", 2);
			write(STDOUT_FILENO, "\e[K", 4);
		} else {
			write(STDOUT_FILENO, &this->raw, 1);
		}

		//out += std::string((const char *)&t);
		//this->buffer += this->raw;
	}

}

void Editor::scanNavFunctions() {
	if(this->raw == 'h') {
		write(STDOUT_FILENO, "\e[1D", 5);
	} else if(this->raw == 'j') {
		write(STDOUT_FILENO, "\e[1B", 5);
	} else if(this->raw == 'k') {
		write(STDOUT_FILENO, "\e[1A", 5);
	} else if(this->raw == 'l') {
		write(STDOUT_FILENO, "\e[1C", 5);
	} else {
	}
}
