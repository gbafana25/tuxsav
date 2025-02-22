#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <map>
#include <fstream>

#include "conf.hpp"
#include "json/json.hpp"

using json = nlohmann::json_abi_v3_11_2::json;

/*

Setup dialog that populates JSON config file.


*/
void run_setup() {
	std::cout << "Warning: this will wipe out current settings" << std::endl;
	std::cout << "To cancel, enter Ctrl+C" << std::endl << std::endl;

	std::string n;
	std::string k;
	std::string serv;
	std::string local;
	std::string url;

	std::ofstream conf("config.json");
	json s;
	std::cout << "Username: ";
	std::cin >> n;
	s["username"] = n;

	std::cout << "Key: ";
	std::cin >>  k;
	s["key"] = k;

	std::cout << "Document Name (on server): ";
	std::cin >> serv;

	std::cout << "Local file path: ";
	std::cin >> local;

	std::cout << "Server url: ";		
	std::cin >> url;
	// remove trailing slash, if it exists
	if(url[url.size()-1] == '/') {
		url.erase(url.size()-1);
	}


	s["local_files"] = {local,};
	s["remote_files"] = {serv,};
	s["url"] = url;
		
	conf << s.dump(4) << std::endl;
	conf.close();
}

/*

Appends to list of local and remote files
in config.json.

*/

void add_doc_obj(std::string serv_name, std::string local_name) {
	std::ifstream conf("config.json");
	std::ostringstream buf;
	std::string sb;
	buf << conf.rdbuf();
	sb = buf.str();
	conf.close();
	json in = json::parse(sb);
	auto init_loc = in["local_files"];
	auto init_rem = in["remote_files"];
	
	init_loc.push_back(local_name);
	init_rem.push_back(serv_name);
	in["local_files"] = init_loc;
	in["remote_files"] = init_rem;
	
	std::ofstream c("config.json");
	c << in.dump(4) << std::endl; 
	c.close();	

}

/*

Makes sure that a document's name is not more 50 characters long

*/

bool isTitleValid(std::string t) {
	if(t.length() > 50) {
		std::cout << "Title name must be <= 50 characters" << std::endl;
		return false;
	}
	return true;
}
