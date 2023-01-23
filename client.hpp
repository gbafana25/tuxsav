#include <sys/socket.h>
#include <netinet/in.h>

class Client {
	public:

	int soc;
	struct sockaddr_in cli;
	socklen_t len;
	int acc;
	char b[128]; // needed?

	Client();
	void connectToServer();
	void sendData(std::string); 
};
