#include "AllUsers.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

//#define PORT "8888" 
#define BUF_LEN 512

class Server {
public:
	Server();
	~Server();

	void CreateServer();
	void UpdateRecv();
	void UpdateSend();
	void HandleSending(std::string _message, struct sockaddr_in _adress);
	void BroadcastMessageToAll(std::string _message);
	void CloseServer();

	bool isServerRunning;

private:
	AllUsers users;
	SOCKET server_socket;
	struct addrinfo hints, *ptr;
	char recv_buf[BUF_LEN];
};

