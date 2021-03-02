#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

int main() {
	const char strMessage[] = "GET /main/ HTTP/1.1\r\nHost: www.ysu.am\r\n\r\n";

	// a structure to fill the server address
	struct sockaddr_in serverAddress;

	// create a socket of (AF_INET -> internet protocol, SOCK_STREAM -> TCP transport, 0 -> auto select a protocol)
	int mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if(mySocket == -1){ // make sure socket created
		std::cerr << "Error while creating a socket" << std::endl;
		exit(errno);
	}

	// fill serevr address with 0s
	memset(&serverAddress, 0, sizeof(serverAddress));
	// fill the server ip and port address
	serverAddress.sin_addr.s_addr = inet_addr("217.113.2.231"); // ip
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(80); // port: HTTP

	// connect socket to given address
	int connected = connect(mySocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
	if(connected == -1){
		std::cerr << "Error while connecting socket" << std::endl;
		exit(errno);
	}

	int sent = send(mySocket, strMessage, strlen(strMessage), 0);
	if(sent == -1){
		std::cerr << "Error while sending to server" << std::endl;
		exit(errno);
	}

	std::string str = "";

	while(true){
		char buf[4096];
		// read from server
		int read = recv(mySocket, buf, strlen(buf), 0);
		if(read == -1){
			std::cerr << "Error while sending to server" << std::endl;
			exit(errno);
		}

		if(read == 0){
			break;
		}

		for(int i = 0; i < strlen(buf); ++i){
			str +=  buf[i];
		}
	}
	std::cout << str << std::endl;

	std::ofstream fout;
	fout.open("a.html");
	if(fout.is_open()){
		fout << str;
	}
	fout.close();
	close(mySocket);
}

