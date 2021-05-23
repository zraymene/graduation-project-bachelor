#pragma once

#include <string>
#include <vector>

class User {
public:
	std::string username;
	std::string password;

	User(std::string username, std::string password);
};

class UserRepository
{
public:
	virtual void AddUser(User user) = 0;
	virtual void EditUser(User user) = 0;
	virtual User* GetUser(std::string username, std::string password) = 0;

};

