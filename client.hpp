#include <curl/curl.h>

class Client {
	public:
	
	CURL *client;
	CURLcode res;
	std::string url;
	std::string buf;


	Client();
	void update(std::string, std::string, std::string);
};
