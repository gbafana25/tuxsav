#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>

#include "client.hpp"

Client::Client() {
	int o = 1;
	this->soc = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(this->soc, SOL_SOCKET, SO_REUSEADDR, &o, sizeof(o));
	this->cli.sin_family = AF_INET;
	this->cli.sin_port = htons(8080);
	//this->cli.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, "127.0.0.1", &this->cli.sin_addr);
	this->len = sizeof(this->cli);

}

void Client::connectToServer() {
	connect(this->soc, (struct sockaddr *) &this->cli, this->len);
	
}

void Client::sendData(std::string d) {
	const char *s = d.c_str();
	send(this->soc, s, strlen(s), 0);
	
}
