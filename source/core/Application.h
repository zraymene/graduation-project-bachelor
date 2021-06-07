#pragma once

#include "dal/UnitOfWork.h"
#include "logic/Users.h"
#include "logic/PersonsManager.h"

class Users;
class PersonsManager;

class Application
{
public:
	Application();
	Application(UnitOfWork* uow);
	~Application();

	void SelectUnitOfWork(int db_type);

	UnitOfWork* GetUnitOfWork();

	Users* GetUsersManage();
	PersonsManager* GetPersonsManager();

private:
	UnitOfWork* unit_of_work = nullptr;

	Users* users_manager = nullptr;
	PersonsManager* persons_manager = nullptr;

	std::string title;
	std::string version;

	void SetUnitOfWork(UnitOfWork* uow);
};

