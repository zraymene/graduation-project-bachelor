#include "UnitOfWork.h"

#include <iostream>
#include <fstream>

Database::Database(int type,std::string hostname, std::string username,
	std::string password, std::string database)
{
	this->type = type;
	this->hostname = hostname;
	this->username = username;
	this->password = password;
	this->database = database;
}

void Database::WriteDataBaseToFile()
{
	std::ofstream file;

	// Open file to write and replace content if the file already exists
	file.open(DB_SAVE_FILE, std::ios::out | std::ios::trunc | std::ios::binary );
	file << this->type << this->hostname << ' ' << this->username << ' ' 
		<< this->password << ' ' << this->database;
	file.close();
}

Database* Database::GetDatabaseFromFile()
{
	std::ifstream file;

	file.open(DB_SAVE_FILE, std::ios::in | std::ios::binary);

	if (!file.is_open())
		return nullptr;

	Database* db = new Database();

	file >> db->type >> db->hostname >>
		db->username >> db->password >> db->database;

	file.close();
	return db;
}

void Database::test()
{

	Database db(DB_TYPE_MYSQL, "localhost", "root", "pass", "db");

	db.WriteDataBaseToFile();

	Database* _db = Database::GetDatabaseFromFile();

	std::cout << "Hostname:" << _db->username
		<< "Username :" << _db->username;
	
	wxLogDebug("Hostname: %s\nUsername: %s\nPassword: %s\nDatabase: %s\n",
		_db->hostname, _db->username, _db->password, _db->database);

	delete _db;
}

UserRepository *UnitOfWork::GetUserRepository()
{
	return this->user_rep;
}

PersonsRepository* UnitOfWork::GetPersonsRepository()
{
	return this->persons_rep;
}

TransactionsRepository* UnitOfWork::GetTransactionsRepository()
{
	return this->transactions_rep;
}

GroupsRepository* UnitOfWork::GetGroupsRepository()
{
	return this->groups_rep;
}

UnitOfWork::UnitOfWork()
{
	
}

UnitOfWork::~UnitOfWork()
{
	delete this->database;
}
