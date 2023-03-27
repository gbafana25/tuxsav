#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <bitset>
#include <curl/curl.h>


#include "client.hpp"
#include "json/json.hpp"

using json = nlohmann::json_abi_v3_11_2::json;


/*

Callback function for libcurl, used to write response data to buffer (stored in instance of Client).

*/
static size_t write_callback(void *raw, size_t size, size_t nmemb, void *d) {
	((std::string*)d)->append((char*)raw, size * nmemb);
	return size * nmemb;

}

Client::Client() {
	this->client = curl_easy_init();

	if(this->client) {
		// URL to follow - class variable
		curl_easy_setopt(this->client, CURLOPT_URL, this->url.c_str());
		// Point to callback
		curl_easy_setopt(this->client, CURLOPT_WRITEFUNCTION, write_callback);
		// Follow redirects
		curl_easy_setopt(this->client, CURLOPT_FOLLOWLOCATION, 1);
		// Set buffer to for response data - class variable
		curl_easy_setopt(this->client, CURLOPT_WRITEDATA, &this->buf);

	}

}

/*

Encodes communication to base64

*/

std::string Client::base64_encode(std::string w) {
	/*

	1. Shift over 2 
	2. Shift 1st over 4 (so it's in leftmost spot), or with 2nd letter shifted 2 right (mask?)
	3. Mask to isolate last 4 of 2nd letter.  Or with 3rd letter shifted 6 right

	more info: https://ecs.syr.edu/faculty/fawcett/Handouts/cse687/code/Base64Encoding/Base64.cpp

	*/

	std::string set = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string res;
	long unsigned int i;

	for(i = 0; i < w.size()-2; i+=3) {
		std::string sub = w.substr(i, 4);
		char fi = sub[0] >> 2;
		char sec = (sub[0] << 4) | (sub[1] >> 4);
		char thi = ((sub[1] & 0b001111) << 2) | ((sub[2] >> 6) & 0b000011);
		char fo = sub[2] & 0b111111;
		
		std::bitset<6> b1 = fi;
		std::bitset<6> b2 = sec;
		std::bitset<6> b3 = thi;
		std::bitset<6> b4 = fo;	

		res += set[b1.to_ulong()];
		res += set[b2.to_ulong()];
		res += set[b3.to_ulong()];
		res += set[b4.to_ulong()];

	}

	if(i < w.size()) {
		res += set[w[i] >> 2];
		if(i < (w.size()-1)) {
			res += set[(w[i] & 0b000011)<< 4];	
			res += "=";
		} else {
			res += set[(w[i] & 0b000011) << 4 | ((w[i+1] & 0b111100) >> 4)]; 
			res += "=";
		}

	}
	return res;	


}


/*
	
Writes data to file

*/
void Client::write_response_to_file(std::string data, std::string fname) {
	std::ofstream out(fname);
	out << data << std::endl;
	out.close();
}

/*

Client username setter 

*/
void Client::set_username(std::string u) {
	this->username = u;	
}

/*

Client username getter

*/	
std::string Client::get_username() {
	return this->username;
}

/*

Sends specified request body to url.
The url is pre-set in the endpoint functions themselves (fetch, update, etc.)

*/
json Client::execute_api_request(std::string fstr) {
	// set request headers
	this->hd = NULL;
	this->hd = curl_slist_append(this->hd, "Content-Type: application/json");
	curl_easy_setopt(this->client, CURLOPT_HTTPHEADER, this->hd);

	// set url
	curl_easy_setopt(this->client, CURLOPT_URL, this->url.c_str());
	// set necessary flags for POST request
	curl_easy_setopt(this->client, CURLOPT_POST, 1L);
	curl_easy_setopt(this->client, CURLOPT_POSTFIELDSIZE, -1L);
	curl_easy_setopt(this->client, CURLOPT_POSTFIELDS, fstr.c_str());
	// make request
	this->res = curl_easy_perform(this->client);
	curl_easy_cleanup(this->client);

	// return response as json type
	json rp = json::parse(this->buf);
	return rp;

}

/*

API - /multi-update 
request data:
data: string
key: string
username: string
doc_name: string

addr - base url loaded from settings

*/
void Client::multi_update(std::string key, std::vector<std::string> data, std::vector<std::string> name, std::string addr, std::string host, std::vector<std::string> local) {
	this->url = addr+"/multi-update/";
	json fields;
	fields["key"] = key;
	fields["data"] = data;
	fields["doc_name"] = name;
	fields["username"] = this->username;
	fields["host_name"] = host;
	fields["local_name"] = local;
	std::string fstr = fields.dump(4);	

	json rp = execute_api_request(fstr);
	std::cout << rp["status"] << std::endl;

}

/*

API - /update 
request data:
data: string
key: string
username: string
doc_name: string

addr - base url loaded from settings

*/
void Client::update(std::string key, std::string data, std::string name, std::string addr, std::string host, std::string local) {
	this->url = addr+"/update/";
	json fields;
	fields["key"] = key;
	fields["data"] = this->base64_encode(data);
	fields["doc_name"] = name;
	fields["username"] = this->username;
	fields["host_name"] = host;
	fields["local_name"] = local;
	std::string fstr = fields.dump(4);	

	json rp = execute_api_request(fstr);
	std::cout << rp["status"] << std::endl;

}

/*

API - /delete
request data:
key: string
doc_name: string
username: string

addr - base url loaded from settings

*/
void Client::delete_document(std::string key, std::string name, std::string addr) {
	this->url = addr+"/delete/";
	
	json fields;
	fields["key"] = key;
	fields["doc_name"] = name;
	fields["username"] = this->username;
	std::string fstr = fields.dump(4);
	
	json rp = execute_api_request(fstr);
	std::cout << rp["status"] << std::endl;

}

/*

API - /create
request data:
key: string
doc_name: string
username: string

addr - base url loaded from settings

*/
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

/*

API - /fetch
request data:
key: string
doc_name: string
username: string
doc_name: string

addr - base url loaded from settings

*/
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
