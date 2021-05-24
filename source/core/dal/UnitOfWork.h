#pragma once

#include "wx/wx.h"
#include <string>

#include "UserRepository.h"
#include "PersonsRepository.h"
#include "TransactionsRepository.h"
#include "GroupsRepository.h"

#define DB_SAVE_FILE "data"

#define DB_TYPE_MYSQL 0

class Database {
	public:
		int type = DB_TYPE_MYSQL; // mysql
		std::string hostname = "";
		std::string username = "";
		std::string password = "";
		std::string database = "";

		Database() {};
		Database(int type, std::string hostname, std::string username,
			std::string password, std::string database = "");

		void WriteDataBaseToFile();
		
		static Database* GetDatabaseFromFile();

		static void test();
};

class UnitOfWork
{
public:
	virtual bool Begin() = 0;
	virtual bool Commit() = 0;
	virtual void Abort() = 0;
	virtual bool Connect(Database* database) = 0;
	virtual bool CheckIfDatabaseEmpty() = 0;

	UserRepository* GetUserRepository();
	PersonsRepository* GetPersonsRepository();
	TransactionsRepository* GetTransactionsRepository();
	GroupsRepository* GetGroupsRepository();

	UnitOfWork();
	~UnitOfWork();

protected:
	Database* database = nullptr;

	UserRepository* user_rep;
	PersonsRepository* persons_rep;
	TransactionsRepository* transactions_rep;
	GroupsRepository* groups_rep;
};

