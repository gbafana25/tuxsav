#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>

#include "client.hpp"
#include "term.hpp"
#include "editor.hpp"
#include "swpread.hpp"
#include "json/json.hpp"

using json = nlohmann::json_abi_v3_11_2::json;


void start_dialog() {
	std::cout << "-s : run setup dialog" << std::endl;
	std::cout << "-i [document]: initialize backup daemon" << std::endl;
	std::cout << "-h : display this help menu" << std::endl;	
}

void run_setup() {
	std::string n;
	std::string k;
	std::ofstream conf("config.json");
	json s;
	std::cout << "Username: ";
	std::cin >> n;
	s["username"] = n;
	std::cout << "Key: ";
	std::cin >>  k;
	s["key"] = k;
		
	conf << s.dump(4) << std::endl;
	conf.close();
}


int main(int argc, char **argv) {
	std::cout << "Starting savebox.." << std::endl;
	if(argc < 2) {
		start_dialog();
		return 0;
	} else {
		if(strcmp(argv[1],"-s") == 0) {
			run_setup();
			return 0;
		} else if(strcmp(argv[1],"-i") == 0 && argc == 3) {
		
			VSReader vr;
			json co = vr.load_config();
			while(vr.read_raw()) {
				// keep updating, w/ delay
				Client cl;
				cl.username = co["username"];
				cl.update(co["key"], vr.raw, argv[2]);
				sleep(4);
			}

			// file has been closed, submit final copy to server
			Client f;
			f.username = co["username"];
			vr.get_final();
			std::cout << vr.raw << std::endl;
			f.update(co["key"], vr.raw, argv[2]);


			return 0;
		}
	}
	
	

}
