#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h>


#include "client.hpp"
#include "json/json.hpp"

using json = nlohmann::json_abi_v3_11_2::json;

static size_t write_callback(void *raw, size_t size, size_t nmemb, void *d) {
	((std::string*)d)->append((char*)raw, size * nmemb);
	return size * nmemb;

}

Client::Client() {
	this->client = curl_easy_init();

	if(this->client) {
		curl_easy_setopt(this->client, CURLOPT_URL, this->url.c_str());
		curl_easy_setopt(this->client, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(this->client, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(this->client, CURLOPT_WRITEDATA, &this->buf);

	}

}


void Client::update(std::string key, std::string data, std::string name) {
	this->url = "http://localhost:8000/update/";
	json fields;
	struct curl_slist *hd = NULL;
	fields["key"] = key;
	fields["data"] = data;
	fields["doc_name"] = name;
	fields["username"] = "test";
	std::string fstr = fields.dump(4);
	hd = curl_slist_append(hd, "Content-Type: application/json");
	curl_easy_setopt(this->client, CURLOPT_HTTPHEADER, hd);

	curl_easy_setopt(this->client, CURLOPT_URL, this->url.c_str());
	curl_easy_setopt(this->client, CURLOPT_POST, 1L);
	curl_easy_setopt(this->client, CURLOPT_POSTFIELDSIZE, -1L);
	curl_easy_setopt(this->client, CURLOPT_POSTFIELDS, fstr.c_str());
	this->res = curl_easy_perform(this->client);
	curl_easy_cleanup(this->client);
	std::cout << this->buf << std::endl;

}
