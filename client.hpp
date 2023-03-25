#include <curl/curl.h>
#include "json/json.hpp"

using json = nlohmann::json_abi_v3_11_2::json;

class Client {
	private:
	std::string username;

	public:
	
	CURL *client;
	CURLcode res;
	struct curl_slist *hd;
	std::string url;
	std::string buf;


	Client();
	void set_username(std::string);
	std::string get_username();
	json execute_api_request(std::string);
	void update(std::string, std::string, std::string, std::string, std::string, std::string);
	void create(std::string, std::string, std::string);
	void delete_document(std::string, std::string, std::string);
	void multi_update(std::string key, std::vector<std::string> data, std::vector<std::string> name, std::string addr, std::string host, std::vector<std::string> local);
	std::string fetch(std::string key, std::string name, std::string addr);
	void write_response_to_file(std::string, std::string);
	
};
