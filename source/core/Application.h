#pragma once

#include "dal/UnitOfWork.h"

class Application
{
public:
	Application();
	Application(UnitOfWork* uow);
	~Application();

	void SelectUnitOfWork(int db_type);

	UnitOfWork* GetUnitOfWork();

private:
	UnitOfWork* unit_of_work;
	std::string title;
	std::string version;

	void SetUnitOfWork(UnitOfWork* uow);
};

