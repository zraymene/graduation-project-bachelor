#pragma once

#include "mysql.h"

#include "../UnitOfWork.h"

class MYSQLUnitOfWork : public UnitOfWork
{
public:
	
	void Commit();
	void Abort();
	bool Connect(Database* database);
	bool CheckIfDatabaseEmpty();

	MYSQLUnitOfWork();
	~MYSQLUnitOfWork();

	static void test();
private:
	MYSQL mysql_connection;
};

