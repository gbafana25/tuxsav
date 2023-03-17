#include <fstream>
#include <iostream>

#include "swpread.hpp"

VSReader::VSReader() {}

void VSReader::read_raw() {
	std::string n;
	char c;
	int start = 0;
	std::ifstream swp;
	swp.open(".testfile.swp");
	// check if valid letter, num space till hits offset
	getline(swp, n);
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
	
	
	swp.close();

}
