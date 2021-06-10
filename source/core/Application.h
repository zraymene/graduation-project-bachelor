#pragma once

#include "dal/UnitOfWork.h"
#include "logic/Users.h"
#include "logic/PersonsManager.h"
#include "logic/GroupsManager.h"
#include "logic/TransactionsManager.h"

class Users;
class PersonsManager;
class GroupsManager;
class TransactionsManager;

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
	GroupsManager* GetGroupsManager();
	TransactionsManager* GetTransactionsManager();

private:
	UnitOfWork* unit_of_work = nullptr;

	Users* users_manager = nullptr;
	PersonsManager* persons_manager = nullptr;
	GroupsManager* groups_manager = nullptr;
	TransactionsManager* transactions_manager = nullptr;

	std::string title;
	std::string version;

	void SetUnitOfWork(UnitOfWork* uow);
};

