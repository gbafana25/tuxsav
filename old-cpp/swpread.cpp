#include <fstream>
#include <iostream>
#include <ostream>
#include <cstring>
#include <nlohmann/json.hpp>

#include "swpread.hpp"
//#include "json/json.hpp"


using json = nlohmann::json_abi_v3_11_3::json;

VSReader::VSReader() {}

/*

Generate .swp file name from regular file name

*/

std::string VSReader::build_path(std::string base) {
	std::string n;
	std::string full;
	full.append(base);
	full.append(".swp");

	/*

	If the current file is not in the current folder, then the period that normally precedes .swp filenames can't be added to the very beginning.

	*/
	size_t last_slash = full.find_last_of("/");
	// if there is a slash, insert the period right after it
	if(last_slash != std::string::npos) {
		full.insert(last_slash+1, ".");
	} else {
		// put the period at the beginning	
		full.insert(0, ".");
	}
	return full;
	
}

/*

Read metadata from .swp file 

*/

bool VSReader::read_metadata(std::string base) {
	std::string full = this->build_path(base);
	std::ifstream met;
	std::string n;
	met.open(full);
	if(!met.is_open()) {
		return false;
	}

	std::ostringstream rdr;
	rdr << met.rdbuf();
	n = rdr.str();

	this->host = n.substr(68, 40);	
	
	return true;

}

/*

Read from Vim .swp file

*/
bool VSReader::read_raw(std::string base) {
	std::string full = this->build_path(base);
	std::string n;
	int start = 0;
	
	this->swp.open(full);
	if(!this->swp.is_open()) {
		return false;
	}
	this->raw.clear();
	std::ostringstream rdr;
	rdr << this->swp.rdbuf();
	n = rdr.str();

	/*

	The Vim .swp file has metadata in the very beginning of it that isn't used at the moment, and should be excluded from the data sent to the server.
	According to one program (see README), the file data should start at the 900th index.  However, for some larger files some data does get cut off (as seen when using the README as a test file)

	*/
	
	std::string sub = n.substr(n.size()-900, n.size());
	sub.erase(0, full.size()-1);
	
	// find starting value for the for loop below, where the lines are reversed into the proper order (first character that isn't null)
	for(long unsigned int i = 0; i < sub.size(); i++) {
		if(sub[i] != '\0') {
			break;
		}
		start += 1;
	}

	// reverse order of lines, not each character
	for(long unsigned int i = start; i < sub.size(); i++) {
		std::string line;
		while(sub[i] != '\0') {
			line += sub[i];
			i++;
		}

		line += "\n";
		this->raw.insert(0, line);
	}
	
	
	this->swp.close();
	return true;

}

/*

Read from actual source file.
Usually done when .swp version isn't available, or when file is closed during the single-backup loop.

*/
bool VSReader::get_final(std::string path) {
	std::ifstream fin;
	this->raw.clear();
	std::string plain;
	std::string l;
	fin.open(path);
	if(!fin.is_open()) {
		return false;
	}
	while(getline(fin, l)) {
		plain.append(l);
		plain += "\n";
	}
	this->raw = plain;

	return true;	

}

/*

load config file

*/
json VSReader::load_config() {
	std::ifstream con("/home/gareth/tuxsav/config.json");
	json j;
	std::cout << "Reading Config file" << std::endl;
	j = json::parse(con);
	return j;


}
