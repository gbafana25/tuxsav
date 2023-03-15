#include <fstream>
#include <iostream>

#include "swpread.hpp"

VSReader::VSReader() {}

void VSReader::read_raw() {
	std::string n;
	std::ifstream swp;
	swp.open(".testfile.swp");
	//swp >> this->raw;
	while(getline(swp, n)) {
		//std::cout << n << std::endl;
		this->raw.append(n);
	}
	swp.close();

}
