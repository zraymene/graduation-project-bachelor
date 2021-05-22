#include "MYSQLUnitOfWork.h"

#include <list>

#define CREATE_DATABASE_QUERY "CREATE DATABASE IF NOT EXISTS school_app"

void MYSQLUnitOfWork::Commit()
{
}

void MYSQLUnitOfWork::Abort()
{
}

bool MYSQLUnitOfWork::Connect(Database* database)
{
	/*this->database = Database::GetDatabaseFromFile(DB_SAVE_FILE);

	// Choose database
	if (!this->database)
	{
		// Temporary, for testing
		this->database = new Database("localhost", "root", "pass", "db");
		this->database->WriteDataBaseToFile(DB_SAVE_FILE);
	}*/

	this->database = database;

	mysql_library_init(0, nullptr, nullptr);

	if (!mysql_init(&this->mysql_connection))
	{
		wxLogDebug("#Error while creating MYSQL Object !");
		return false;
	}

	if (!mysql_real_connect(&this->mysql_connection,
		this->database->hostname.c_str(),
		this->database->username.c_str(),
		this->database->password.c_str(),
		this->database->database.c_str(),
		0, nullptr, 0))
	{
		wxLogDebug("#Failed to connect to MYSQL database, Error : %s!",
			mysql_error(&this->mysql_connection));
		return false;
	}
	
	wxLogDebug("Connected to MYSQL databse !");

	return true;
}

bool MYSQLUnitOfWork::CheckIfDatabaseEmpty()
{
	// Create database if it doesn't exists
	if (mysql_query(&this->mysql_connection, CREATE_DATABASE_QUERY))
	{
		wxLogDebug("#Failed creating database, Error : %s!",
			mysql_error(&this->mysql_connection));
	}

	// Create table if it doesn't exists
	std::list<std::string> table_queries {
		"CREATE TABLE IF NOT EXISTS `school_app`.`student` ( `id` INT NOT NULL AUTO_INCREMENT , `first_name` VARCHAR(255) NOT NULL , `last_name` VARCHAR(255) NOT NULL , `phone` VARCHAR(255) NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`teacher` ( `id` INT NOT NULL AUTO_INCREMENT , `first_name` VARCHAR(255) NOT NULL , `last_name` VARCHAR(255) NOT NULL , `phone` VARCHAR(255) NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`transaction` ( `id` INT NOT NULL AUTO_INCREMENT , `name` VARCHAR(255) NOT NULL , `amount` DOUBLE NOT NULL , `date` TIMESTAMP NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`absence` ( `student_id` INT NOT NULL , `group_id` INT NOT NULL , `date` TIMESTAMP NOT NULL , `type` BOOLEAN NOT NULL ) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`group` ( `id` INT NOT NULL AUTO_INCREMENT , `name` VARCHAR(255) NOT NULL , `teacher_id` INT NOT NULL , `price` DOUBLE NOT NULL , `school_percentage` FLOAT NOT NULL , `teacher_percentage` FLOAT NOT NULL , `lessons_per_week` TINYINT NOT NULL , `teacher_payed_times` INT NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`lesson` ( `group_id` INT NOT NULL , `saturday` VARCHAR(20) NOT NULL , `sunday` VARCHAR(20) NOT NULL , `monday` VARCHAR(20) NOT NULL , `tudesday` VARCHAR(20) NOT NULL , `wednesday` VARCHAR(20) NOT NULL , `thrusday` VARCHAR(20) NOT NULL , `friday` VARCHAR(20) NOT NULL ) ENGINE = InnoDB;"
	};

	for (auto const& query : table_queries)
	{
		if (mysql_query(&this->mysql_connection, query.c_str()))
		{
			wxLogDebug("#Failed to create table, Error : %s!",
				mysql_error(&this->mysql_connection));
		}
	}

	return true;
}

MYSQLUnitOfWork::MYSQLUnitOfWork()
{

}

MYSQLUnitOfWork::~MYSQLUnitOfWork()
{
	// Close connection and free mysql library
	mysql_close(&this->mysql_connection);
	mysql_library_end();
}

void MYSQLUnitOfWork::test()
{
	MYSQLUnitOfWork uow;
	
	Database* db = Database::GetDatabaseFromFile();

	if (!db)
		db = new Database("localhost", "root", "", "");

	uow.Connect(db);

	uow.CheckIfDatabaseEmpty();
}


