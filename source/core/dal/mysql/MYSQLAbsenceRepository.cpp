#include "MYSQLGroupsRepository.h"

#define INSERT_ABSENCE_STMT "INSERT INTO `school_app`.`absence`\
	(`student_id`, `group_id`, `date`, `type`)\
	 VALUES (?,?,?,?)"

#define UPDATE_ABSENCE_STMT "UPDATE `school_app`.`absence` SET `type`=? \
	WHERE `student_id`=?,`group_id`=?,`date`=?"
#define DELETE_ABSENCE_STMT "DELETE FROM `school_app`.`absence`\
	 WHERE `student_id`=? AND `group_id`=? AND `date`=?"
#define DELETE_ABSENCES_GROUP_STMT "DELETE FROM `school_app`.`absence`\
	 WHERE `group_id`=?"
#define DELETE_ABSENCES_STUDENT_STMT "DELETE FROM `school_app`.`absence`\
	 WHERE `student_id`=?"
#define SELECT_GROUP_ABSENCE_STMT "SELECT * FROM `school_app`.`absence`\
	 WHERE `group_id`=?"
#define SELECT_STUDENT_ABSENCE_STMT "SELECT * FROM `school_app`.`absence`\
 WHERE `student_id`=?"

void MYSQLGroupsRepository::AddAbsence(Absence absence)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[4];

	if (mysql_stmt_prepare(stmt, INSERT_ABSENCE_STMT,
		sizeof(INSERT_ABSENCE_STMT)))
	{
		wxLogDebug("#Failed to prepare 'INSERT ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'INSERT ABSENCE' statement!");
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &absence.student_id;

	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &absence.group_id;

	unsigned long date_length = absence.date.length();

	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer = (char*)absence.date.c_str();
	bind[2].length = &date_length;

	bind[3].buffer_type = MYSQL_TYPE_TINY;
	bind[3].buffer = &absence.type;


	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'INSERT ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind 'INSERT ABSENCE' statement!");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'INSERT ABSENCE' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submit 'INSERT ABSENCE' statement!");
	}

	wxLogDebug("Absence added!, Date = %s.", absence.date);

	mysql_stmt_close(stmt);
}

void MYSQLGroupsRepository::EditAbsence(Absence absence)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[4];

	if (mysql_stmt_prepare(stmt, UPDATE_ABSENCE_STMT,
		sizeof(UPDATE_ABSENCE_STMT)))
	{
		wxLogDebug("#Failed to prepare 'UPDATE ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'UPDATE ABSENCE' statement!");
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_TINY;
	bind[0].buffer = &absence.type;

	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &absence.student_id;

	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer = &absence.group_id;

	unsigned long date_length = absence.date.length();
	bind[3].buffer_type = MYSQL_TYPE_STRING;
	bind[3].buffer = (char*)absence.date.c_str();
	bind[3].length = &date_length;


	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'UPDATE ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind 'UPDATE ABSENCE' statement!");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'UPDATE ABSENCE' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submit 'UPDATE ABSENCE' statement!");
	}

	wxLogDebug("Absences updated: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

void MYSQLGroupsRepository::DeleteAbsence(Absence absence)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[3];

	if (mysql_stmt_prepare(stmt, DELETE_ABSENCE_STMT,
		sizeof(DELETE_ABSENCE_STMT)))
	{
		wxLogDebug("#Failed to prepare 'DELETE ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'DELETE ABSENCE' statement!");
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &absence.student_id;

	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &absence.group_id;

	unsigned long date_length = absence.date.length();
	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer = (char*)absence.date.c_str();
	bind[2].length = &date_length;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'DELETE ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind 'DELETE ABSENCE' statement!");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'DELETE ABSENCE' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submit 'DELETE ABSENCE' statement!");
	}

	wxLogDebug("Absences deleted: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

void MYSQLGroupsRepository::DeleteAbsences(int id, bool student_absence)
{

	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[1];
	int x;

	if (student_absence)
		x = mysql_stmt_prepare(stmt, DELETE_ABSENCES_STUDENT_STMT,
			sizeof(DELETE_ABSENCES_STUDENT_STMT));
	else
		x = mysql_stmt_prepare(stmt, DELETE_ABSENCES_GROUP_STMT,
			sizeof(DELETE_ABSENCES_GROUP_STMT));

	if (x)
	{
		wxLogDebug("#Failed to prepare 'DELETE ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'DELETE ABSENCE' statement!");
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'DELETE ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind 'DELETE ABSENCE' statement!");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'DELETE ABSENCE' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submit 'DELETE ABSENCE' statement!");
	}

	wxLogDebug("Absences deleted: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

std::vector<Absence> MYSQLGroupsRepository::GetAbsence(Group* group, Person* person)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[1];
	int x;
	int id;
	std::vector<Absence> absences;

	if (group) {
		x = mysql_stmt_prepare(stmt, SELECT_GROUP_ABSENCE_STMT,
			sizeof(SELECT_GROUP_ABSENCE_STMT));
		id = group->id;
	}
	else {
		x = mysql_stmt_prepare(stmt, SELECT_STUDENT_ABSENCE_STMT,
			sizeof(SELECT_STUDENT_ABSENCE_STMT));
		id = person->id;
	}

	if (x)
	{
		wxLogDebug("#Failed to prepare 'SELECT ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'SELECT ABSENCE' statement");
		//return absences;
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'SELECT ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind 'SELECT ABSENCE' statement");
		//return absences;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'SELECT ABSENCE' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submit 'SELECT ABSENCE' statement");
		//return absences;
	}
	// ------------------------------

	unsigned long length;
	MYSQL_BIND result[4];
	// Result buffers
	int student_id, group_id;
	bool type;
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

	result[3].buffer_type = MYSQL_TYPE_TINY;
	result[3].buffer = &type;


	if (mysql_stmt_bind_result(stmt, result))
	{
		wxLogDebug("#Failed to bind result 'SELECT ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind result 'SELECT ABSENCE' statement");
		//return absences;
	}

	if (mysql_stmt_store_result(stmt))
	{
		wxLogDebug("#Failed to store result 'SELECT ABSENCE', Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to store result 'SELECT ABSENCE' statement");
		//return absences;
	}

	int rows_num = mysql_stmt_num_rows(stmt);
	//wxLogDebug("Num of rows:%d", rows_num);

	date[254] = '\0';

	while (!mysql_stmt_fetch(stmt)) {

		Absence absence(student_id, group_id, date, type);

		absences.push_back(absence);
	}

	wxLogDebug("[%s] ID = %d, ROWS : %d",
		__FUNCTION__, id, rows_num);

	mysql_stmt_close(stmt);

	return absences;
}
