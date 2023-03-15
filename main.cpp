#include <iostream>
#include <unistd.h>
#include <string>

#include "client.hpp"
#include "term.hpp"
#include "editor.hpp"


int main() {
	std::cout << "Starting savebox.." << std::endl;
	Client cl;
	cl.update("test string");
	//TermSettings term;
	//Editor ed;
	//term.setupTerminal();
	//ed.inputLoop();	
	//std::cout << ed.buffer << std::endl;
	//term.resetTerminal();
	

}
