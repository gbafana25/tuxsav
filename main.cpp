#include <iostream>
#include <unistd.h>
#include <string>

#include "client.hpp"
#include "term.hpp"
#include "editor.hpp"


int main() {
	// hardcode key
	std::cout << "Starting savebox.." << std::endl;
	Client cl;
	cl.update("10c5b69e-0a4d-4898-bfaf-02d1b78a1718", "test string", "test2");
	//TermSettings term;
	//Editor ed;
	//term.setupTerminal();
	//ed.inputLoop();	
	//std::cout << ed.buffer << std::endl;
	//term.resetTerminal();
	

}
