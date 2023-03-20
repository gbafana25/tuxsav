#include <fstream>
#include <iostream>
#include <ostream>

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
	// check if valid letter, num space till hits offset
	getline(this->swp, n);
	// TODO: swp file indices range in 1000's, try different starting sizes 
	
	std::string sub = n.substr(n.size()-900, n.size());
	std::cout << sub << std::endl;
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
	
	
	this->swp.close();
	return true;

}

void VSReader::get_final(std::string path) {
	std::ifstream fin;
	this->raw.clear();
	std::string l;
	fin.open(path);
	while(getline(fin, l)) {
		this->raw.append(l);
		this->raw += "\n";
	}


}

json VSReader::load_config() {
	std::ifstream con("config.json");
	json j;
	j = json::parse(con);
	//std::cout << j << std::endl;
	return j;


}
