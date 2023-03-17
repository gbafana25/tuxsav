#include <fstream>
#include <iostream>
#include <ostream>

#include "swpread.hpp"

VSReader::VSReader() {}

bool VSReader::read_raw() {
	std::string n;
	char c;
	int start = 0;
	this->swp.open(".testfile.swp");
	if(!this->swp.is_open()) {
		return false;
	}
	this->raw.clear();
	// check if valid letter, num space till hits offset
	getline(this->swp, n);
	// swp file indices range in 1000's, try different starting sizes 
	
	std::string sub = n.substr(n.size()-100, n.size());
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

void VSReader::get_final() {
	std::ifstream fin;
	this->raw.clear();
	std::string l;
	fin.open("testfile");
	while(getline(fin, l)) {
		this->raw.append(l);
		this->raw += "\n";
	}


}
