#pragma once

#include "wx/wx.h"
#include <string>


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
	virtual void Commit() = 0;
	virtual void Abort() = 0;
	virtual bool Connect(Database* database) = 0;
	virtual bool CheckIfDatabaseEmpty() = 0;

	UnitOfWork();
	~UnitOfWork();

protected:
	Database* database = nullptr;
	std::vector<std::string> queries;
};

