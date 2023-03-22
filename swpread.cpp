#include <fstream>
#include <iostream>
#include <ostream>
#include <cstring>
#include <algorithm>
#include <cstddef>
#include <bitset>
#include <vector>

#include "swpread.hpp"
#include "json/json.hpp"

using json = nlohmann::json_abi_v3_11_2::json;

VSReader::VSReader() {}

bool VSReader::read_raw(std::string base) {
	std::string n;
	std::string full = ".";
	char c;
	int start = 0;
	full.append(base);
	full.append(".swp");
	this->swp.open(full);
	if(!this->swp.is_open()) {
		return false;
	}
	this->raw.clear();
	std::ostringstream rdr;
	rdr << this->swp.rdbuf();
	n = rdr.str();
	
	// some data still gets cut off for large files?
	// test w/ README.md
	std::string sub = n.substr(n.size()-900, n.size());
	sub.erase(0, full.size()-1);
	// reverse order of lines, not each character
	for(int i = 0; i < sub.size(); i++) {
		if(sub[i] != '\0') {
			break;
		}
		start += 1;
	}
	for(int i = start; i < sub.size(); i++) {
		std::string line;
		while(sub[i] != '\0') {
			line += sub[i];
			i++;
		}

		//std::cout << line << std::endl;
		line += "\n";
		this->raw.insert(0, line);
	}
	
	
	//b64encode(this->raw);
	this->swp.close();
	return true;

}

void VSReader::get_final(std::string path) {
	std::ifstream fin;
	this->raw.clear();
	std::string plain;
	std::string l;
	fin.open(path);
	while(getline(fin, l)) {
		plain.append(l);
		plain += "\n";
	}
	this->raw = plain;

	// convert 
	//b64encode(this->raw);
	

}

json VSReader::load_config() {
	std::ifstream con("config.json");
	json j;
	j = json::parse(con);
	//std::cout << j << std::endl;
	return j;


}
