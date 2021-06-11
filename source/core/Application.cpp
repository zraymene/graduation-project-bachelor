#include "Application.h"

#include "dal/mysql/MYSQLUnitOfWork.h"

Application::Application()
{
	this->users_manager = new Users(this);
	this->persons_manager = new PersonsManager(this);
	this->groups_manager = new GroupsManager(this);
	this->transactions_manager = new TransactionsManager(this);
	this->statistics_manager = new StatisticsManager(this);
}

Application::Application(UnitOfWork* uow)
{
	this->unit_of_work = uow;
}

Application::~Application()
{
	delete this->unit_of_work,
		this->users_manager,
		this->persons_manager,
		this->groups_manager,
		this->transactions_manager,
		this->statistics_manager;

}

void Application::SelectUnitOfWork(int db_type)
{
	switch (db_type)
	{
		case DB_TYPE_MYSQL:
			this->SetUnitOfWork(new MYSQLUnitOfWork());
			break;
	}
}

UnitOfWork* Application::GetUnitOfWork()
{
	return this->unit_of_work;
}

Users* Application::GetUsersManage()
{
	return this->users_manager;
}

PersonsManager* Application::GetPersonsManager()
{
	return this->persons_manager;
}

GroupsManager* Application::GetGroupsManager()
{
	return this->groups_manager;
}

TransactionsManager* Application::GetTransactionsManager()
{
	return this->transactions_manager;
}

StatisticsManager* Application::GetStatisticsManager()
{
	return this->statistics_manager;
}

void Application::SetUnitOfWork(UnitOfWork* uow)
{
	if (!this->unit_of_work)
		delete this->unit_of_work;

	this->unit_of_work = uow;
}

