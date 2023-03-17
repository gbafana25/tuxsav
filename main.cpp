#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

#include "client.hpp"
#include "term.hpp"
#include "editor.hpp"
#include "swpread.hpp"


int main() {
	std::cout << "Starting savebox.." << std::endl;
	/*
	cl.update("10c5b69e-0a4d-4898-bfaf-02d1b78a1718", "blink", "testdoc");
	*/

	VSReader vr;
	while(vr.read_raw()) {
		//std::cout << vr.raw << std::endl;
		// keep updating, w/ delay
		Client cl;
		cl.username = "test";
		cl.update("10c5b69e-0a4d-4898-bfaf-02d1b78a1718", vr.raw, "testdoc");
		sleep(3);
	}

	// file has been closed, submit final copy to server
	vr.get_final();
	std::cout << vr.raw << std::endl;
	

}
