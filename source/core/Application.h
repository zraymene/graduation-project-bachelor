#pragma once

#include "dal/UnitOfWork.h"
#include "logic/Users.h"

class Users;

class Application
{
public:
	Application();
	Application(UnitOfWork* uow);
	~Application();

	void SelectUnitOfWork(int db_type);

	UnitOfWork* GetUnitOfWork();

	Users* GetUsersManage();

private:
	UnitOfWork* unit_of_work = nullptr;
	Users* users_manager = nullptr;

	std::string title;
	std::string version;

	void SetUnitOfWork(UnitOfWork* uow);
};

