#include <fstream>
#include "json/json.hpp"

using json = nlohmann::json_abi_v3_11_2::json;

class VSReader {
	public:

	std::string raw;
	//std::string fname;
	std::string host;
	std::ifstream swp;

	VSReader();
	std::string build_path(std::string);
	bool read_metadata(std::string);
	bool read_raw(std::string);
	bool get_final(std::string);
	json load_config();

};
