#include <iostream>
#include <unistd.h>
#include <string>

#include "client.hpp"
#include "term.hpp"
#include "editor.hpp"
#include "swpread.hpp"


int main() {
	std::cout << "Starting savebox.." << std::endl;
	/*
	Client cl;
	cl.username = "test";
	cl.update("10c5b69e-0a4d-4898-bfaf-02d1b78a1718", "this is a different one", "testdoc");
	*/

	/*
	TermSettings term;
	Editor ed;
	term.setupTerminal();
	ed.inputLoop();	
	std::cout << ed.buffer[0] << std::endl;
	term.resetTerminal();
	*/
	

	VSReader vr;
	while(vr.read_raw()) {
		//std::cout << vr.raw << std::endl;
		// keep updating, w/ delay
	}

	// file has been closed, submit final copy to server
	vr.get_final();
	std::cout << vr.raw << std::endl;
	

}
