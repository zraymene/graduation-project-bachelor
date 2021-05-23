#include "MYSQLPersonsRepository.h"

#define INSERT_TEACHER_STMT "INSERT INTO\
		`school_app`.`teacher`(`first_name`,`last_name`, `phone`)\
		 VALUES (?,?,?)"
#define INSERT_STUDENT_STMT "INSERT INTO\
		`school_app`.`student`(`first_name`,`last_name`, `phone`)\
		 VALUES (?,?,?)"

#define UPDATE_STUDENT_STMT "UPDATE `school_app`.`student`\
		SET `first_name`=?,`last_name`=?,`phone`=?\
		WHERE `id`=?"
#define UPDATE_TEACHER_STMT "UPDATE `school_app`.`teacher`\
		SET `first_name`=?,`last_name`=?,`phone`=?\
		WHERE `id`=?"

#define DELETE_TEACHER_STMT "DELETE FROM `school_app`.`teacher` WHERE id = ?"
#define DELETE_STUDENT_STMT "DELETE FROM `school_app`.`student` WHERE id = ?"

#define SELECT_TEACHER_STMT "SELECT * FROM `school_app`.`teacher`"
#define SELECT_STUDENT_STMT "SELECT * FROM `school_app`.`student`"

void MYSQLPersonsRepository::AddPerson(Person& person, bool is_student)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[3];
	int x;
	
	if (is_student)
		x = mysql_stmt_prepare(stmt, INSERT_STUDENT_STMT,
			sizeof(INSERT_STUDENT_STMT));
	else 
		x = mysql_stmt_prepare(stmt, INSERT_TEACHER_STMT,
			sizeof(INSERT_TEACHER_STMT));

	if (x)
	{
		wxLogDebug("#Failed to prepare 'INSERT PERSON' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	memset(bind, 0, sizeof(bind));

	unsigned long fname_length = person.first_name.length();
	unsigned long lname_length = person.last_name.length();
	unsigned long phone_length = person.phone.length();

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (char*)person.first_name.c_str();
	bind[0].length = &fname_length;

	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (char*)person.last_name.c_str();
	bind[1].length = &lname_length;

	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer = (char*)person.phone.c_str();
	bind[2].length = &phone_length;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'INSERT PERSON' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'INSERT PERSON' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	// Get newly AUTO_INCREMENT  id
	person.id = mysql_insert_id(this->mysql_connection);

	wxLogDebug("Person added!, Is a teacher ? : %d.", !is_student);

	mysql_stmt_close(stmt);
}

void MYSQLPersonsRepository::EditPerson(Person person, bool is_student)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[4];
	int x;

	if (is_student)
		x = mysql_stmt_prepare(stmt, UPDATE_STUDENT_STMT,
			sizeof(UPDATE_STUDENT_STMT));
	else
		x = mysql_stmt_prepare(stmt, UPDATE_TEACHER_STMT,
			sizeof(UPDATE_TEACHER_STMT));

	if (x)
	{
		wxLogDebug("#Failed to prepare 'INSERT PERSON' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	memset(bind, 0, sizeof(bind));

	unsigned long fname_length = person.first_name.length();
	unsigned long lname_length = person.last_name.length();
	unsigned long phone_length = person.phone.length();

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (char*)person.first_name.c_str();
	bind[0].length = &fname_length;

	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (char*)person.last_name.c_str();
	bind[1].length = &lname_length;

	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer = (char*)person.phone.c_str();
	bind[2].length = &phone_length;

	bind[3].buffer_type = MYSQL_TYPE_LONG;
	bind[3].buffer = &person.id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'UPDATE PERSON' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'UPDATE PERSON' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	wxLogDebug("Persons updated: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

void MYSQLPersonsRepository::DeletePerson(Person person, bool is_student)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[1];
	int x;

	if (is_student)
		x = mysql_stmt_prepare(stmt, DELETE_STUDENT_STMT,
			sizeof(DELETE_STUDENT_STMT));
	else
		x = mysql_stmt_prepare(stmt, DELETE_TEACHER_STMT,
			sizeof(DELETE_TEACHER_STMT));

	if (x)
	{
		wxLogDebug("#Failed to prepare 'DELETE PERSON' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &person.id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'DELETE PERSON' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'DELETE PERSON' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	wxLogDebug("Persons deleted: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

std::vector<Person> MYSQLPersonsRepository::GetPersons(bool is_student)
{
	std::vector<Person> persons;
	
	int x;

	if (is_student)
		x = mysql_query(this->mysql_connection, SELECT_STUDENT_STMT);
	else
		x = mysql_query(this->mysql_connection, SELECT_TEACHER_STMT);

	if(x)
	{
		wxLogDebug("#Failed to retrive persons list, Error : %s!",
			mysql_error(this->mysql_connection));
		return persons;
	}

	MYSQL_RES* result = mysql_store_result(this->mysql_connection);
	MYSQL_ROW row;

	while ((row = mysql_fetch_row(result)) != nullptr)
	{
		Person person(atoi(row[0]), row[1], row[2], row[3]);
		persons.push_back(person);
	}

	mysql_free_result(result);

	return persons;
}

MYSQLPersonsRepository::MYSQLPersonsRepository(MYSQL* mysql_connection)
{
	this->mysql_connection = mysql_connection;
}
