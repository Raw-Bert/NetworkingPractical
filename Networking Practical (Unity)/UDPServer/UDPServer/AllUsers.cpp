#include "AllUsers.h"

void AllUsers::addUser(std::string name, sockaddr_in adress, Status userStatus)
{
	listUsers.push_back(User(name, adress, userStatus));
}

bool AllUsers::checkExist(sockaddr_in adr)
{
	std::list <User> ::iterator it;
	for (it = listUsers.begin(); it != listUsers.end(); it++)
	{
		if ((*it)._adress.sin_addr.s_addr == adr.sin_addr.s_addr) {
			return true;
		}
	}
	return false;
}

std::string AllUsers::findName(sockaddr_in adr)
{
	std::list <User> ::iterator it;
	for (it = listUsers.begin(); it != listUsers.end(); it++)
	{
		if ((*it)._adress.sin_addr.s_addr == adr.sin_addr.s_addr) {
			return (*it)._name;
		}
	}
	printf("Adress of user is not exit!!!!!Name cannot be found!!!\n");
	return std::string();
}

sockaddr_in AllUsers::findAdress(std::string nam)
{
	std::list <User> ::iterator it;
	for (it = listUsers.begin(); it != listUsers.end(); it++)
	{
		if ((*it)._name == nam) {
			return (*it)._adress;
		}
	}
	printf("Name of user is not exit!!!!! Adress cannot be found!!!\n");
	return sockaddr_in();
}

void AllUsers::changeStatus(std::string nam, Status userSta)
{
	std::list <User> ::iterator it;
	for (it = listUsers.begin(); it != listUsers.end(); it++)
	{
		if ((*it)._name == nam) {
			(*it)._userStatus = userSta;
			return;
		}
	}
	printf("Name of user is not exit!!!!! Status cannot be changed!!!\n");
}

void AllUsers::changeStatus(sockaddr_in adr, Status userSta)
{
	std::list <User> ::iterator it;
	for (it = listUsers.begin(); it != listUsers.end(); it++)
	{
		if ((*it)._adress.sin_addr.s_addr == adr.sin_addr.s_addr) {
			(*it)._userStatus = userSta;
			return;
		}
	}
	printf("Adress of user is not exit!!!!! Status cannot be changed!!!\n");
}

std::string AllUsers::getStatus(User user)
{
	if (user._userStatus == 1) {
		return "Available";
	}
	else if (user._userStatus == 2) {
		return "Busy Playing Game";
	}
	else {
		return "Busy Chatting";
	}
}

std::string AllUsers::returnAllUsers()
{
	std::string line;
	line += "Online User: ";
	line += std::to_string(listUsers.size());
	line += "\n";
	int i = 1;
	if (listUsers.size() != 0)
	{
		std::list <User> ::iterator it;
		for (it = listUsers.begin(); it != listUsers.end(); it++)
		{
			line += std::to_string(i);
			line += ". ";
			line += (*it)._name;
			line += "-";
			line += getStatus(*it);
			line += "\n";
			i++;
		}
		line += "Please enter user's number to select who you want to interact, and follow with G for game C for chat.\n";
	}

	return line;
}
