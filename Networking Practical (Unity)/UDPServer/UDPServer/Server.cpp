//Based on in-class code.
#include "Server.h"

Server::Server()
{
	isServerRunning = true;
}

Server::~Server()
{
}

void Server::CreateServer()
{
	//Initialize winsock
	WSADATA wsa;

	int error;
	error = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (error != 0) {
		printf("Failed to initialize %d\n", error);
		return;
	}

	//Create a Server socket
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, "8888", &hints, &ptr) != 0) {
		printf("Getaddrinfo failed!! %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (server_socket == INVALID_SOCKET) {
		printf("Failed creating a socket %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	// Bind socket

	if (bind(server_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
		printf("Bind failed: %d\n", WSAGetLastError());
		closesocket(server_socket);
		freeaddrinfo(ptr);
		WSACleanup();
		return;
	}

	printf("Server is now running!\n");
}

void Server::UpdateRecv()
{
	while (isServerRunning) {
		struct sockaddr_in fromAddr;
		int fromlen;
		fromlen = sizeof(fromAddr);
		memset(recv_buf, 0, BUF_LEN);
		if (recvfrom(server_socket, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*) & fromAddr, &fromlen) == SOCKET_ERROR) {
			printf("recvfrom() failed...%d\n", WSAGetLastError());
			return;
		}

		//Mennage Messages
		std::string message = std::string(recv_buf);
		char code = message[0];
		if (code == '@') { //client is logging on
			message.erase(0, 1);
			std::string newMassage = "Client " + message + " joined.\n";

			// send new client info to all client
			BroadcastMessageToAll(newMassage);

			// send online info back new client
			std::string line = users.returnAllUsers();
			HandleSending(line, fromAddr);

			users.addUser(message, fromAddr, Status::Availiable);
		}
		
		else
		{
			printf("Received: %s\n", recv_buf);
		}
	}
}

void Server::UpdateSend()
{
	while (isServerRunning) {
	std::string line;
	std::getline(std::cin, line);
	char* message = (char*)line.c_str();
	if(line.size() > 0)
		BroadcastMessageToAll(line);
	}
}

void Server::HandleSending(std::string _message, sockaddr_in _adress)
{
	int fromlen;
	fromlen = sizeof(_adress);
	char* temp = (char*)_message.c_str();
	if (sendto(server_socket, temp, BUF_LEN, 0, (struct sockaddr*) & _adress, fromlen) == SOCKET_ERROR)
	{
		printf("sendto() failed %d\n", WSAGetLastError());
	}
}

void Server::BroadcastMessageToAll(std::string _message)
{
	std::cout << _message;
	if (users.listUsers.size() != 0)
	{
		std::list <User> ::iterator it;
		for (it = users.listUsers.begin(); it != users.listUsers.end(); it++)
		{
			HandleSending(_message, (*it)._adress);
		};
	}
}

void Server::CloseServer()
{
	closesocket(server_socket);
	freeaddrinfo(ptr);
	WSACleanup();
}
