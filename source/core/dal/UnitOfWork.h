#pragma once

#include "wx/wx.h"
#include <string>

#include "UserRepository.h"
#include "PersonsRepository.h"

#define DB_SAVE_FILE "data"

class Database {
	public:
		std::string hostname = "";
		std::string username = "";
		std::string password = "";
		std::string database = "";

		Database() {};
		Database(std::string hostname, std::string username,
			std::string password, std::string database);

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

	UnitOfWork();
	~UnitOfWork();

protected:
	Database* database = nullptr;

	UserRepository* user_rep;
	PersonsRepository* persons_rep;
};
