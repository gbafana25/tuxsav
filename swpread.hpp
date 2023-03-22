#include <fstream>
#include "json/json.hpp"

using json = nlohmann::json_abi_v3_11_2::json;

class VSReader {
	public:

	std::string raw;
	std::ifstream swp;

	VSReader();
	bool read_raw(std::string);
	bool get_final(std::string);
	json load_config();

};
