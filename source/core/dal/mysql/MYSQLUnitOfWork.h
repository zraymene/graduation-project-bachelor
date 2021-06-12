#pragma once

#include "mysql.h"

#include "../UnitOfWork.h"

class MYSQLUnitOfWork : public UnitOfWork
{
public:
	
	bool Begin();
	bool Commit();
	void Abort();
	bool Connect(Database* database);
	bool CheckIfDatabaseEmpty();

	MYSQLUnitOfWork();
	~MYSQLUnitOfWork();

private:
	MYSQL mysql_connection;

};

