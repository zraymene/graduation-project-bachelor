#pragma once

#include "../UserRepository.h"
#include "MYSQLUnitOfWork.h"
#include <map>


class MYSQLUserRepository : public UserRepository
{
public:
	void AddUser(User user);
	void EditUser(User user);
	User* GetUser(std::string username, std::string password);

	MYSQLUserRepository(MYSQL* mysql_connection);

private:

	MYSQL* mysql_connection = nullptr;
};

