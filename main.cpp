#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <chrono>
#include <vector>
#include <thread>

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
	std::cout << "-d [document] : delete document object on server" << std::endl;
	std::cout << "-f [document] [/path/to/file] : fetch document from server and put it into specified file" << std::endl;
	std::cout << "-sync : automatically backup all files in config" << std::endl;
	std::cout << "-restore : restore all files in config" << std::endl;
	std::cout << "-h : display this help menu" << std::endl;	
}



int main(int argc, char **argv) {
	std::cout << "Starting savebox.." << std::endl;

	VSReader vr;
	json user_config = vr.load_config();
	if(argc < 2) {
		start_dialog();
		return 0;
	} else {
		if(strcmp(argv[1],"-s") == 0) {
			run_setup();
			return 0;
		} else if(strcmp(argv[1],"-i") == 0 && argc == 4) {
			// Check if document name is <= 50 characters
			std::string arg = argv[2];
			if(!isTitleValid(arg)) {
				return 0;
			}
		
			/*

			Continuously read from .swp file and send the file data to the server.
			If the .swp file closes, read from source file and exit. 

			*/

			while(vr.read_raw(argv[3])) {
				// keep updating, w/ delay
				vr.read_metadata(argv[3]);
				Client cl;
				cl.set_username(user_config["username"]);
				

				cl.update(user_config["key"], vr.raw, argv[2], user_config["url"], vr.host, argv[3]);

				// requires unistd.h, below version is portable
				//sleep(4);
				std::this_thread::sleep_for(std::chrono::milliseconds(4000));
			}

			// file has been closed, submit final copy to server
			Client f;
			f.set_username(user_config["username"]);
			vr.get_final(argv[3]);
			f.update(user_config["key"], vr.raw, argv[2], user_config["url"], vr.host, argv[3]);


			return 0;
		} else if(strcmp(argv[1],"-c") == 0 && argc == 3) {
			// check title length
			std::string arg = argv[2];
			if(!isTitleValid(arg)) {
				return 0;
			}

			/*

			Create a new Document object on the server.
			
			*/

			Client c;
			c.set_username(user_config["username"]);
			c.create(user_config["key"], argv[2], user_config["url"]);
			return 0;
		} else if(strcmp(argv[1],"-a") == 0 && argc == 4) {
			std::string arg = argv[2];
			if(!isTitleValid(arg)) {
				return 0;
			}
			/*

			Add a new document/local file pair to their respective arrays in config.json.
			*/
			add_doc_obj(argv[2], argv[3]);	
			return 0;
		} else if(strcmp(argv[1],"-sync") == 0 && argc == 2) {

			/*

			This function runs indefintely, in order to watch all files, regardless whether they're open or closed.

			manually kill loop w/ Ctrl+C
			reads from original file if swap doesn't exist
			
			TODO: send documents as one request, with arrays for document names and data?
			This would help reduce the number of requests sent.
			
			*/
			while(true) {
				std::vector<std::string> file_data_vec;
				for(long unsigned int i = 0; i < user_config["remote_files"].size(); i++) {
					// Will return false if .swp doesn't exist
					bool swap_is_good = vr.read_raw(user_config["local_files"][i]);
					if(swap_is_good) { 
						// keep updating file contents, w/ delay
						Client enc;
						vr.read_metadata(user_config["local_files"][i]);
						file_data_vec.push_back(enc.base64_encode(vr.raw));
						
					} else {
						bool final_is_good = vr.get_final(user_config["local_files"][i]);
						if(final_is_good) {
						
							Client enc;
							vr.read_metadata(user_config["local_files"][i]);
							file_data_vec.push_back(enc.base64_encode(vr.raw));	

						}
					}
					//std::this_thread::sleep_for(std::chrono::milliseconds(500));

				}
				
				
				Client cl;
				cl.set_username(user_config["username"]);
				cl.multi_update(user_config["key"], file_data_vec, user_config["remote_files"], user_config["url"], vr.host, user_config["local_files"]);
				
				// requires unistd.h, below version is portable
				//sleep(4); 				
				std::this_thread::sleep_for(std::chrono::milliseconds(4000));
			}



		} else if(strcmp(argv[1],"-f") == 0 && argc == 4) {
			std::string arg = argv[2];
			if(!isTitleValid(arg)) {
				return 0;
			}
			/*

			Restore document from server onto local machine

			*/

			Client c;
			c.set_username(user_config["username"]);
			std::string data = c.fetch(user_config["key"], argv[2], user_config["url"]);
			
			c.write_response_to_file(data, argv[3]);

			return 0;
		} else if(strcmp(argv[1],"-restore") == 0 && argc == 2) {
			/*

			Get all files in config.json and restore their contents from the server.

			*/

			for(long unsigned int i = 0; i < user_config["remote_files"].size(); i++) {
				Client c;
				c.set_username(user_config["username"]);
				std::string d = c.fetch(user_config["key"], user_config["remote_files"][i], user_config["url"]);
				c.write_response_to_file(d, user_config["local_files"][i]);
			}

			return 0;
		} else if(strcmp(argv[1],"-d") == 0 && argc == 3) {
			/*
			
			Delete document object on server.

			*/

			// check title length
			std::string arg = argv[2];
			if(!isTitleValid(arg)) {
				return 0;
			}

			Client c;
			c.set_username(user_config["username"]);
			c.delete_document(user_config["key"], argv[2], user_config["url"]);	

		} else {
			start_dialog();
			return -1;
		}
	}
	

}
