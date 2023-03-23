#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
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

void Client::write_response_to_file(std::string data, std::string fname) {
	std::ofstream out(fname);
	out << data << std::endl;
	out.close();
}


void Client::set_username(std::string u) {
	this->username = u;	
}
	
std::string Client::get_username() {
	return this->username;
}

json Client::execute_api_request(std::string fstr) {
	this->hd = NULL;
	this->hd = curl_slist_append(this->hd, "Content-Type: application/json");
	
	curl_easy_setopt(this->client, CURLOPT_HTTPHEADER, this->hd);

	curl_easy_setopt(this->client, CURLOPT_URL, this->url.c_str());
	curl_easy_setopt(this->client, CURLOPT_POST, 1L);
	curl_easy_setopt(this->client, CURLOPT_POSTFIELDSIZE, -1L);
	curl_easy_setopt(this->client, CURLOPT_POSTFIELDS, fstr.c_str());
	this->res = curl_easy_perform(this->client);
	curl_easy_cleanup(this->client);

	json rp = json::parse(this->buf);
	return rp;

}


void Client::update(std::string key, std::string data, std::string name, std::string addr) {
	this->url = addr+"/update/";
	json fields;
	fields["key"] = key;
	fields["data"] = data;
	fields["doc_name"] = name;
	fields["username"] = this->username;
	std::string fstr = fields.dump(4);	

	json rp = execute_api_request(fstr);
	std::cout << rp["status"] << std::endl;

}

void Client::create(std::string key, std::string name, std::string addr) {
	this->url = addr+"/create/";
	
	json fields;
	fields["key"] = key;
	fields["doc_name"] = name;
	fields["username"] = this->username;
	std::string fstr = fields.dump(4);
	
	json rp = execute_api_request(fstr);
	std::cout << rp["status"] << std::endl;

}

std::string Client::fetch(std::string key, std::string name, std::string addr) {
	this->url = addr+"/fetch/";	
	
	json fields;
	fields["key"] = key;
	fields["doc_name"] = name;
	fields["username"] = this->username;
	std::string fstr = fields.dump(4);
	json rp = execute_api_request(fstr);
	return rp["file_data"];
}
