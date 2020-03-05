#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

enum Status
{
	Availiable = 1,
	Gaming = 2,
	Chatting = 3
};

struct User
{
	std::string _name;
	struct sockaddr_in _adress;
	Status _userStatus;
	User(std::string name, struct sockaddr_in adress, Status userStatus)
	{
		_name = name;
		_adress = adress;
		_userStatus = userStatus;
	}
};

class AllUsers
{
public:
	std::list <User> listUsers;
	void addUser(std::string name, struct sockaddr_in adress, Status userStatus);
	bool checkExist(struct sockaddr_in adr);
	std::string findName(struct sockaddr_in adr);
	struct sockaddr_in findAdress(std::string nam);
	void changeStatus(std::string nam, Status userSta);
	void changeStatus(struct sockaddr_in adr, Status userSta);
	std::string getStatus(User user);
	std::string returnAllUsers();
};