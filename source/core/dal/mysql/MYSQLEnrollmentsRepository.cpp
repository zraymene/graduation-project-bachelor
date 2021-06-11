#include "MYSQLGroupsRepository.h"

#define INSERT_ENROLLMENT_STMT "INSERT INTO `school_app`.`enrollment`\
	(`group_id`, `student_id`, `date`)\
	 VALUES (?,?,CURRENT_DATE())"

#define DELETE_ENROLLMENT_STMT "DELETE FROM `school_app`.`enrollment`\
		WHERE `group_id`=? AND `student_id`=?"
#define DELETE_GROUP_ENROLLMENT_STMT "DELETE FROM `school_app`.`enrollment`\
		WHERE `group_id`=? "
#define DELETE_STUDENT_ENROLLMENT_STMT "DELETE FROM `school_app`.`enrollment`\
		WHERE `student_id`=?"
#define SELECT_GROUP_ENROLLMENT_STMT "SELECT * FROM `school_app`.`enrollment`\
		WHERE `group_id`=?"
#define SELECT_STUDENT_ENROLLMENT_STMT "SELECT * FROM `school_app`.`enrollment`\
		WHERE `student_id`=?"

void MYSQLGroupsRepository::AddEnrollment(int group_id, int student_id)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[2];

	if (mysql_stmt_prepare(stmt, INSERT_ENROLLMENT_STMT,
		sizeof(INSERT_ENROLLMENT_STMT)))
	{
		wxLogDebug("#Failed to prepare 'INSERT ENROLLMENT' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'INSERT ENROLLMENT' statement!");
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &group_id;

	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &student_id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'INSERT ENROLLMENT' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind 'INSERT ENROLLMENT' statement!");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'INSERT ENROLLMENT' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submit 'INSERT ENROLLMENT' statement!");
	}

	wxLogDebug("Enrollment added!");

	mysql_stmt_close(stmt);
}

void MYSQLGroupsRepository::DeleteEnrollment(int group_id, int student_id)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[2];

	if (mysql_stmt_prepare(stmt, DELETE_ENROLLMENT_STMT,
		sizeof(DELETE_ENROLLMENT_STMT)))
	{
		wxLogDebug("#Failed to prepare 'DELETE ENROLLMENT' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'DELETE ENROLLMENT' statement!");
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &group_id;

	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &student_id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'DELETE ENROLLMENT' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind 'DELETE ENROLLMENT' statement!");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'DELETE ENROLLMENT' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submit 'DELETE ENROLLMENT' statement!");
	}

	wxLogDebug("Enrollment deleted: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

void MYSQLGroupsRepository::DeleteAllEnrollments(int id, bool is_group)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[1];
	int x;

	std::vector<Absence> absences;

	if (is_group)
		x = mysql_stmt_prepare(stmt, DELETE_GROUP_ENROLLMENT_STMT,
			sizeof(DELETE_GROUP_ENROLLMENT_STMT));
	else
		x = mysql_stmt_prepare(stmt, DELETE_GROUP_ENROLLMENT_STMT,
			sizeof(DELETE_GROUP_ENROLLMENT_STMT));

	if (x)
	{
		wxLogDebug("#Failed to prepare 'DELETE ENROLLMENT' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'DELETE ENROLLMENT' statement!");
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'DELETE ENROLLMENT' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind 'DELETE ENROLLMENT' statement!");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'DELETE ENROLLMENT' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submit 'DELETE ENROLLMENT' statement!");
	}

	wxLogDebug("Enrollment deleted: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

std::vector<Enrollment> MYSQLGroupsRepository::GetEnrollments(int id, bool is_group)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[1];
	int x;
	std::vector<Enrollment> e;

	if (is_group)
		x = mysql_stmt_prepare(stmt, SELECT_GROUP_ENROLLMENT_STMT,
			sizeof(SELECT_GROUP_ENROLLMENT_STMT));
	else
		x = mysql_stmt_prepare(stmt, SELECT_STUDENT_ENROLLMENT_STMT,
			sizeof(SELECT_STUDENT_ENROLLMENT_STMT));

	if (x)
	{
		wxLogDebug("#Failed to prepare 'SELECT ENROLLMENT' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'SELECT ENROLLMENT' statement");
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'SELECT ENROLLMENT' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind 'SELECT ENROLLMENT' statement");
		//return absences;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'SELECT ENROLLMENT' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submit 'SELECT ENROLLMENT' statement");
		//return absences;
	}
	// ------------------------------

	unsigned long length;
	MYSQL_BIND result[3];
	// Result buffers
	int student_id, group_id;
	char date[255];

	memset(result, 0, sizeof(result));
	memset(date, 0, sizeof(date));

	result[0].buffer_type = MYSQL_TYPE_LONG;
	result[0].buffer = &student_id;

	result[1].buffer_type = MYSQL_TYPE_LONG;
	result[1].buffer = &group_id;

	result[2].buffer_type = MYSQL_TYPE_STRING;
	result[2].buffer = date;
	result[2].buffer_length = 255;
	result[2].length = &length;


	if (mysql_stmt_bind_result(stmt, result))
	{
		wxLogDebug("#Failed to bind result 'SELECT ENROLLMENT' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind result 'SELECT ENROLLMENT' statement");
	}

	if (mysql_stmt_store_result(stmt))
	{
		wxLogDebug("#Failed to store result 'SELECT ENROLLMENT', Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to store result 'SELECT ENROLLMENT' statement");
	}

	int rows_num = mysql_stmt_num_rows(stmt);


	date[254] = '\0';


	while(!mysql_stmt_fetch(stmt)){

		Enrollment enrollment(student_id, group_id, date);

		e.push_back(enrollment);
	}

	wxLogDebug("[%s] ID = %d, ROWS : %d",
		__FUNCTION__, id, rows_num);

	mysql_stmt_close(stmt);

	return e;
}