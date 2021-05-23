#include "MYSQLUnitOfWork.h"

#include <list>

#include "MYSQLUserRepository.h"
#include "MYSQLPersonsRepository.h"

#define CREATE_DATABASE_QUERY "CREATE DATABASE IF NOT EXISTS school_app"

// Lazy to write it more than 10 times
#define CON this->mysql_connection
#define USER_REPO ((MYSQLUserRepository*)user_rep)

bool MYSQLUnitOfWork::Begin()
{
	if (mysql_query(&CON, "START TRANSACTION"))
	{
		wxLogDebug("#Failed to start transaction, Error : %s!",
			mysql_error(&CON));
		return false;
	}

	return true;
}

// Returns false on failure
bool MYSQLUnitOfWork::Commit()
{

	//##################################
	// COMMIT TRANSACTION 
	if (mysql_query(&CON, "COMMIT"))
	{
		wxLogDebug("#Failed to commit transaction, Error : %s!",
			mysql_error(&CON));
		return false;
	}

	wxLogDebug("MYSQL Work commited successfuly !");

	return true;
}

// Reset all repositories's queries
void MYSQLUnitOfWork::Abort()
{

	// Rollback transaction 
	if (mysql_query(&CON, "ROLLBACK"))
	{
		wxLogDebug("#Failed to rollback transaction, Error : %s!",
			mysql_error(&CON));
	}
}


bool MYSQLUnitOfWork::Connect(Database* database)
{
	this->database = database;

	mysql_library_init(0, nullptr, nullptr);

	if (!mysql_init(&CON))
	{
		wxLogDebug("#Error while creating MYSQL Object !");
		return false;
	}

	if (!mysql_real_connect(&CON,
		this->database->hostname.c_str(),
		this->database->username.c_str(),
		this->database->password.c_str(),
		this->database->database.c_str(),
		0, nullptr, 0))
	{
		wxLogDebug("#Failed to connect to MYSQL database, Error : %s!",
			mysql_error(&CON));
		return false;
	}
	
	wxLogDebug("Connected to MYSQL databse !");

	return true;
}

bool MYSQLUnitOfWork::CheckIfDatabaseEmpty()
{
	// Create database if it doesn't exists
	if (mysql_query(&CON, CREATE_DATABASE_QUERY))
	{
		wxLogDebug("#Failed creating database, Error : %s!",
			mysql_error(&CON));
		return false;
	}

	// Disable transactions auto commit
	mysql_autocommit(&CON, 0);

	// Create table if it doesn't exists
	std::list<std::string> table_queries {
		"CREATE TABLE IF NOT EXISTS `school_app`.`student` ( `id` INT NOT NULL AUTO_INCREMENT , `first_name` VARCHAR(255) NOT NULL , `last_name` VARCHAR(255) NOT NULL , `phone` VARCHAR(255) NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`teacher` ( `id` INT NOT NULL AUTO_INCREMENT , `first_name` VARCHAR(255) NOT NULL , `last_name` VARCHAR(255) NOT NULL , `phone` VARCHAR(255) NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`transaction` ( `id` INT NOT NULL AUTO_INCREMENT , `name` VARCHAR(255) NOT NULL , `amount` DOUBLE NOT NULL , `date` TIMESTAMP NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`absence` ( `student_id` INT NOT NULL , `group_id` INT NOT NULL , `date` TIMESTAMP NOT NULL , `type` BOOLEAN NOT NULL ) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`group` ( `id` INT NOT NULL AUTO_INCREMENT , `name` VARCHAR(255) NOT NULL , `teacher_id` INT NOT NULL , `price` DOUBLE NOT NULL , `school_percentage` FLOAT NOT NULL , `teacher_percentage` FLOAT NOT NULL , `lessons_per_week` TINYINT NOT NULL , `teacher_payed_times` INT NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`lesson` ( `group_id` INT NOT NULL , `saturday` VARCHAR(20) NOT NULL , `sunday` VARCHAR(20) NOT NULL , `monday` VARCHAR(20) NOT NULL , `tudesday` VARCHAR(20) NOT NULL , `wednesday` VARCHAR(20) NOT NULL , `thrusday` VARCHAR(20) NOT NULL , `friday` VARCHAR(20) NOT NULL ) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`user` ( `username` VARCHAR(255) NOT NULL , `password` VARCHAR(255) NOT NULL ) ENGINE = InnoDB;"
	};

	for (auto const& query : table_queries)
	{
		if (mysql_query(&CON, query.c_str()))
		{
			wxLogDebug("#Failed to create table, Error : %s!",
				mysql_error(&CON));
			return false;
		}
	}

	// Get rows count from user table
	if (mysql_query(&CON,
		"SELECT COUNT(*) FROM `school_app`.`user`"))
	{
		wxLogDebug("#Failed to retrive user table count, Error : %s!",
			mysql_error(&CON));
		return false;
	}

	MYSQL_RES* result = mysql_store_result(&CON);
	MYSQL_ROW row = mysql_fetch_row(result);

	mysql_free_result(result);

	// Count != 0, database isn't empty
	if (row[0][0] != '0')
		return false;

	wxLogDebug("MYSQL database is empty !");

	return true;
}

MYSQLUnitOfWork::MYSQLUnitOfWork()
{
	this->user_rep = new MYSQLUserRepository(&CON);
	this->persons_rep = new MYSQLPersonsRepository(&CON);
}

MYSQLUnitOfWork::~MYSQLUnitOfWork()
{
	// Close connection and free mysql library
	mysql_close(&CON);
	mysql_library_end();
}

void MYSQLUnitOfWork::test()
{
	MYSQLUnitOfWork uow;
	
	Database* db = Database::GetDatabaseFromFile();

	if (!db)
		db = new Database("localhost", "root", "", "");

	db->WriteDataBaseToFile();

	uow.Connect(db);

	uow.CheckIfDatabaseEmpty();

	/*
	################## UserRepository TEST ####################
	uow.Begin();
	uow.GetUserRepository()->AddUser({ "root", "root" });
	uow.GetUserRepository()->EditUser({ "root", "toor" });
	uow.Commit();*/
	//uow.GetUserRepository()->GetUser("root", "toor");
	
	/*
	 ################## PersonsRepository TEST ####################
	Person person;
	person.first_name = "zeroual";
	person.last_name = "aymene";
	person.phone = "+2139999999999";
	uow.Begin();
	uow.GetPersonsRepository()->AddPerson(person, false);

	wxLogDebug("New id %d", person.id);
	person.last_name = "----------";
	person.phone = "0000000111";
	uow.GetPersonsRepository()->EditPerson(person, false);
	person.id = 1;
	uow.GetPersonsRepository()->DeletePerson(person, false);	
	uow.Commit();

	std::vector<Person> persons = uow.GetPersonsRepository()->GetPersons(false);
	wxLogDebug("Total teachers: %d!",
		(int)persons.size());
	*/
}


