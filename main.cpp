#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <typeinfo>

#include "client.hpp"
#include "swpread.hpp"
#include "conf.hpp"
#include "json/json.hpp"

using json = nlohmann::json_abi_v3_11_2::json;


void start_dialog() {
	std::cout << "-s : run setup dialog" << std::endl;
	std::cout << "-i [document] [/path/to/file] : backup one file" << std::endl;
	std::cout << "-a [document] [/path/to/file] : add file to backup list" << std::endl;
	
	std::cout << "-c [document] : create document on server" << std::endl;
	std::cout << "-r : automatically backup all files in config" << std::endl;
	std::cout << "-h : display this help menu" << std::endl;	
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
		} else if(strcmp(argv[1],"-i") == 0 && argc == 4) {
		
			VSReader vr;
			json co = vr.load_config();
			while(vr.read_raw(argv[3])) {
				// keep updating, w/ delay
				Client cl;
				cl.username = co["username"];
				cl.update(co["key"], vr.raw, argv[2], co["url"]);
				sleep(4);
			}

			// file has been closed, submit final copy to server
			Client f;
			f.username = co["username"];
			vr.get_final(argv[3]);
			//std::cout << vr.raw << std::endl;
			f.update(co["key"], vr.raw, argv[2], co["url"]);


			return 0;
		} else if(strcmp(argv[1],"-c") == 0 && argc == 3) {
			Client c;
			VSReader vr;
			json co = vr.load_config();
			c.username = co["username"];
			c.create(co["key"], argv[2], co["url"]);
			return 0;
		} else if(strcmp(argv[1],"-a") == 0 && argc == 4) {
			add_doc_obj(argv[2], argv[3]);	
			return 0;
		} else if(strcmp(argv[1],"-r") == 0 && argc == 2) {
			VSReader vr;
			json co = vr.load_config();

			int num_files = co["remote_files"].size();
			/*

			manually kill loop w/ Ctrl+C
			reads from original file if swap doesn't exist
			
			*/
			while(true) {
				for(int i = 0; i < co["remote_files"].size(); i++) {
					bool swap_is_good = vr.read_raw(co["local_files"][i]);
					if(swap_is_good) { 
						// keep updating, w/ delay
						Client cl;
						cl.username = co["username"];
						cl.update(co["key"], vr.raw, co["remote_files"][i], co["url"]);
					} else {
						bool final_is_good = vr.get_final(co["local_files"][i]);
						if(final_is_good) {
							Client cl;
							cl.username = co["username"];
							cl.update(co["key"], vr.raw, co["remote_files"][i], co["url"]);

						}
					}

				}
				sleep(4);
			}


		}
	}
	
	

}
