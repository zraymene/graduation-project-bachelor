#include "MYSQLUnitOfWork.h"

#include <list>

#include "MYSQLUserRepository.h"
#include "MYSQLPersonsRepository.h"
#include "MYSQLTransactionsRepository.h"
#include "MYSQLGroupsRepository.h"

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
		throw std::runtime_error("Error starting transaction !");
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
		throw std::runtime_error("Failed to commit transaction!");
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
		throw std::runtime_error("Failed to rollback transaction!");
	}
}


bool MYSQLUnitOfWork::Connect(Database* database)
{
	this->database = database;

	mysql_library_init(0, nullptr, nullptr);

	if (!mysql_init(&CON))
	{
		wxLogDebug("#Error while creating MYSQL Object !");
		throw std::runtime_error("Cannot create MYSQL Object !");
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

		throw std::runtime_error("Error establishing a MYSQL database connection! !");
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
		throw std::runtime_error("Failed to create a database!");
	}

	// Create table if it doesn't exists
	std::list<std::string> table_queries {
		"CREATE TABLE IF NOT EXISTS `school_app`.`student` ( `id` INT NOT NULL AUTO_INCREMENT , `first_name` VARCHAR(255) NOT NULL , `last_name` VARCHAR(255) NOT NULL , `phone` VARCHAR(255) NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`teacher` ( `id` INT NOT NULL AUTO_INCREMENT , `first_name` VARCHAR(255) NOT NULL , `last_name` VARCHAR(255) NOT NULL , `phone` VARCHAR(255) NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`transaction` ( `id` INT NOT NULL AUTO_INCREMENT , `name` VARCHAR(255) NOT NULL , `amount` DOUBLE NOT NULL , `date` TIMESTAMP NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`absence` ( `student_id` INT NOT NULL , `group_id` INT NOT NULL , `date` TIMESTAMP NOT NULL , `type` BOOLEAN NOT NULL, FOREIGN KEY(`student_id`) REFERENCES `student`(`id`), FOREIGN KEY(`group_id`) REFERENCES `group`(`id`) ) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`group` ( `id` INT NOT NULL AUTO_INCREMENT , `name` VARCHAR(255) NOT NULL , `teacher_id` INT NOT NULL , `price` DOUBLE NOT NULL , `school_percentage` FLOAT NOT NULL , `teacher_percentage` FLOAT NOT NULL , `lessons_per_week` TINYINT NOT NULL , `teacher_payed_times` INT NOT NULL , PRIMARY KEY (`id`), FOREIGN KEY(`teacher_id`) REFERENCES `teacher`(`id`)) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`lesson` ( `group_id` INT NOT NULL , `saturday` VARCHAR(20) NOT NULL , `sunday` VARCHAR(20) NOT NULL , `monday` VARCHAR(20) NOT NULL , `tudesday` VARCHAR(20) NOT NULL , `wednesday` VARCHAR(20) NOT NULL , `thrusday` VARCHAR(20) NOT NULL , `friday` VARCHAR(20) NOT NULL, FOREIGN KEY(`group_id`) REFERENCES `group`(`id`) ) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`user` ( `username` VARCHAR(255) NOT NULL , `password` VARCHAR(255) NOT NULL ) ENGINE = InnoDB;",
		"CREATE TABLE IF NOT EXISTS `school_app`.`enrollment` ( `student_id` INT NOT NULL, `group_id` INT NOT NULL , `date` TIMESTAMP NOT NULL , FOREIGN KEY(`student_id`) REFERENCES `student`(`id`), FOREIGN KEY(`group_id`) REFERENCES `group`(`id`)) ENGINE = InnoDB;"
	};

	for (auto const& query : table_queries)
	{
		if (mysql_query(&CON, query.c_str()))
		{
			wxLogDebug("#Failed to create table, Error : %s!",
				mysql_error(&CON));
			throw std::runtime_error("Failed to create table!");
		}
	}

	// Disable transactions auto commit
	mysql_autocommit(&CON, 0);
	// Disable checking forign key
	mysql_query(&CON, "SET FOREIGN_KEY_CHECKS=0;");

	// Get rows count from user table
	if (mysql_query(&CON,
		"SELECT COUNT(*) FROM `school_app`.`user`"))
	{
		wxLogDebug("#Failed to retrive user table count, Error : %s!",
			mysql_error(&CON));
		throw std::runtime_error("Failed to retrive user table count!");
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
	this->transactions_rep = new MYSQLTransactionsRepository(&CON);
	this->groups_rep = new MYSQLGroupsRepository(&CON);
}

MYSQLUnitOfWork::~MYSQLUnitOfWork()
{
	// Delete repositories
	delete this->user_rep, this->persons_rep,
		this->transactions_rep, this->groups_rep;

	// Close connection and free mysql library
	mysql_close(&CON);
	mysql_library_end();
}

void MYSQLUnitOfWork::test()
{
	MYSQLUnitOfWork uow;
	
	Database* db = Database::GetDatabaseFromFile();

	if (!db)
		db = new Database(DB_TYPE_MYSQL, "localhost", "root", "", "");

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
	uow.GetPersonsRepository()->DeletePerson(person.id, false);	
	uow.Commit();

	std::vector<Person> persons = uow.GetPersonsRepository()->GetPersons(false);
	wxLogDebug("Total teachers: %d.",
		(int)persons.size());
	*/

	//################## TransactionsRepository TEST ####################
	/*Transaction trans;
	trans.name = "Pay teacher";
	trans.amount = 1999.05f;
	
	uow.Begin();
	uow.GetTransactionsRepository()->AddTransaction(trans);

	trans.amount = -1000.99f;
	uow.GetTransactionsRepository()->EditTransaction(trans);
	uow.GetTransactionsRepository()->DeleteTransaction(1);
	uow.Commit();
	std::vector<Transaction> transs = uow.GetTransactionsRepository()
		->GetTransactions();
	wxLogDebug("Total transactions: %d.", (int)transs.size());*/
	
	/*

	//################## Group TEST ####################


	/*std::vector<Group> transs = uow.GetGroupsRepository()
		->GetGroups();
	wxLogDebug("Total groups: %d.", (int)transs.size());
	*/

	Group group;
	group.id = 10;
	group.name = "ENGLISH";
	group.teacher_id = 5;
	group.price = 800.09f;
	group.school_percentage = 50.00f;
	group.teacher_percentage = 50.00f;
	group.lessons_per_week = 4;

	Person person;
	person.id = 10;

	Absence absence(5,10, "2013-08-05 18:19:03", false);
	
	uow.Begin();
	//uow.GetGroupsRepository()->EditGroupLessons(group);
	//uow.GetGroupsRepository()->AddGroup(group);
	//uow.GetGroupsRepository()->DeleteGroup(7);
	
	//uow.GetGroupsRepository()->DeleteAbsence(absence);
	//uow.GetGroupsRepository()->DeleteAbsences(5, true);
	
	uow.Commit();

	//uow.GetGroupsRepository()->GetGroupLessons(group);
	std::vector<Absence> absences = uow.GetGroupsRepository()->GetAbsence(nullptr, &person);
}


