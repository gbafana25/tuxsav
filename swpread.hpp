#include <fstream>

class VSReader {
	public:

	std::string raw;
	std::ifstream swp;

	VSReader();
	bool read_raw();
	void get_final();

};
