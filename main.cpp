#include <iostream>
#include <unistd.h>
#include <string>

#include "client.hpp"
#include "term.hpp"
#include "editor.hpp"


int main() {
	std::cout << "Starting savebox.." << std::endl;
	//Client cl;
	TermSettings term;
	Editor ed;
	term.setupTerminal();
	//cl.connectToServer();
	//cl.sendData("test string\n");
	ed.inputLoop();	
	//std::cout << ed.buffer << std::endl;
	term.resetTerminal();

}
