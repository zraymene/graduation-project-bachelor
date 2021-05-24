#include "MYSQLGroupsRepository.h"

#define INSERT_GROUP_STMT "INSERT INTO `school_app`.`group`\
	(`name`, `teacher_id`, `price`, `school_percentage`,\
	`teacher_percentage`, `lessons_per_week`, `teacher_payed_times`)\
	 VALUES (?,?,?,?,?,?,0)"

#define DELETE_GROUP_STMT "DELETE FROM `school_app`.`group` WHERE `id`=?"

#define UPDATE_GROUP_STMT "UPDATE `school_app`.`group` SET\
	 `name`=?,`teacher_id`=?,`price`=?,\
	`school_percentage`=?,`teacher_percentage`=?,\
	`lessons_per_week`=?,`teacher_payed_times`=?\
	WHERE `id`=?"

#define SELECT_ALL_GROUP_STMT "SELECT * FROM `school_app`.`group`"

#define SELECT_LESSONS_STMT "SELECT * FROM `school_app`.`lesson`\
				WHERE `group_id` = ?"

#define UPDATE_LESSONS_STMT "UPDATE `school_app`.`lesson` SET\
	`saturday`=?,`sunday`=?,`monday`=?,`tudesday`=?,`wednesday`=?,`thrusday`=?,\
	`friday`=? WHERE `group_id`=?"

#define DELETE_LESSONS_STMT "DELETE FROM `school_app`.`lesson` WHERE `group_id`=?"
#define INSERT_LESSONS_STMT "INSERT INTO `school_app`.`lesson`\
(`group_id`, `saturday`, `sunday`, `monday`, `tudesday`, `wednesday`,\
 `thrusday`, `friday`) VALUES (?,?,?,?,?,?,?,?)"

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

void MYSQLGroupsRepository::AddGroup(Group& group)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[6];

	if (mysql_stmt_prepare(stmt, INSERT_GROUP_STMT,
		sizeof(INSERT_GROUP_STMT)))
	{
		wxLogDebug("#Failed to prepare 'INSERT GROUP' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	memset(bind, 0, sizeof(bind));

	unsigned long name_length = group.name.length();

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (char*)group.name.c_str();
	bind[0].length = &name_length;

	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &group.teacher_id;

	bind[2].buffer_type = MYSQL_TYPE_DOUBLE;
	bind[2].buffer = &group.price;

	bind[3].buffer_type = MYSQL_TYPE_FLOAT;
	bind[3].buffer = &group.school_percentage;

	bind[4].buffer_type = MYSQL_TYPE_FLOAT;
	bind[4].buffer = &group.teacher_percentage;

	bind[5].buffer_type = MYSQL_TYPE_LONG;
	bind[5].buffer = &group.lessons_per_week;


	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'INSERT GROUP' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'INSERT GROUP' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	// Get newly AUTO_INCREMENT  id
	group.id = mysql_insert_id(this->mysql_connection);

	wxLogDebug("Group added!, Name = %s, School Per = %.2f, Teacher Per = %.2f",
			group.name.c_str(),
			(float)group.school_percentage,
			(float)group.teacher_percentage);

	mysql_stmt_close(stmt);

	// Add group lessons whene we add a new group
	// Might change in the future
	this->AddGroupLessons(group);
}

void MYSQLGroupsRepository::DeleteGroup(int id)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[1];

	if (mysql_stmt_prepare(stmt, DELETE_GROUP_STMT,
		sizeof(DELETE_GROUP_STMT)))
	{
		wxLogDebug("#Failed to prepare 'DELETE GROUP' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'DELETE GROUP' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'DELETE GROUP' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	wxLogDebug("Group deleted: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);

	// Add group lessons whene we add a new group
	// Might change in the future
	this->DeleteGroupLessons(id);
}

void MYSQLGroupsRepository::EditGroup(Group group)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[8];

	if (mysql_stmt_prepare(stmt, UPDATE_GROUP_STMT,
		sizeof(UPDATE_GROUP_STMT)))
	{
		wxLogDebug("#Failed to prepare 'UPDATE GROUP' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	memset(bind, 0, sizeof(bind));

	unsigned long name_length = group.name.length();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (char*)group.name.c_str();
	bind[0].length = &name_length;

	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &group.teacher_id;

	bind[2].buffer_type = MYSQL_TYPE_DOUBLE;
	bind[2].buffer = &group.price;

	bind[3].buffer_type = MYSQL_TYPE_FLOAT;
	bind[3].buffer = &group.school_percentage;

	bind[4].buffer_type = MYSQL_TYPE_FLOAT;
	bind[4].buffer = &group.teacher_percentage;

	bind[5].buffer_type = MYSQL_TYPE_LONG;
	bind[5].buffer = &group.lessons_per_week;

	bind[6].buffer_type = MYSQL_TYPE_LONG;
	bind[6].buffer = &group.teacher_payed_times;

	bind[7].buffer_type = MYSQL_TYPE_LONG;
	bind[7].buffer = &group.id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'UPDATE GROUP' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'UPDATE GROUP' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	wxLogDebug("Groups updated: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

void MYSQLGroupsRepository::AddGroupLessons(Group group)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[8];

	if (mysql_stmt_prepare(stmt, INSERT_LESSONS_STMT,
		sizeof(INSERT_LESSONS_STMT)))
	{
		wxLogDebug("#Failed to prepare 'INSERT LESSON' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &group.id;

	unsigned long date_length[7];
	std::string fromated_dates[7];

	for (int i = 0; i < 7; i++)
	{
		fromated_dates[i] = Lesson::FormateDate(group.lessons[i]);

		date_length[i] = fromated_dates[i].length();
		bind[i+1].buffer_type = MYSQL_TYPE_STRING;
		bind[i+1].buffer = (char*)fromated_dates[i].c_str();
		bind[i+1].length = &date_length[i];
	}

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'INSERT LESSON' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'INSERT LESSON' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	mysql_stmt_close(stmt);
}

void MYSQLGroupsRepository::DeleteGroupLessons(int id)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[1];

	if (mysql_stmt_prepare(stmt, DELETE_LESSONS_STMT,
		sizeof(DELETE_LESSONS_STMT)))
	{
		wxLogDebug("#Failed to prepare 'DELETE LESSONS' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'DELETE LESSONS' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'DELETE LESSONS' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	wxLogDebug("Lessons deleted: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

void MYSQLGroupsRepository::EditGroupLessons(Group group)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[8];

	if (mysql_stmt_prepare(stmt, UPDATE_LESSONS_STMT,
		sizeof(UPDATE_LESSONS_STMT)))
	{
		wxLogDebug("#Failed to prepare 'UPDATE LESSONS' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	memset(bind, 0, sizeof(bind));

	unsigned long date_length[7];
	std::string fromated_dates[7];

	for (int i = 0; i < 7; i++)
	{
		fromated_dates[i] = Lesson::FormateDate(group.lessons[i]);

		date_length[i] = fromated_dates[i].length();
		bind[i].buffer_type = MYSQL_TYPE_STRING;
		bind[i].buffer = (char*)fromated_dates[i].c_str();
		bind[i].length = &date_length[i];
	}

	bind[7].buffer_type = MYSQL_TYPE_LONG;
	bind[7].buffer = &group.id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'UPDATE LESSONS' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'UPDATE LESSONS' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	wxLogDebug("Lessons updated: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

void MYSQLGroupsRepository::AddAbsence(Absence absence)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[4];

	if (mysql_stmt_prepare(stmt, INSERT_ABSENCE_STMT,
		sizeof(INSERT_ABSENCE_STMT)))
	{
		wxLogDebug("#Failed to prepare 'INSERT ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
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
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'INSERT ABSENCE' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
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
		return;
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
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'UPDATE ABSENCE' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
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
		return;
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
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'DELETE ABSENCE' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
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
		return;
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'DELETE ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'DELETE ABSENCE' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	wxLogDebug("Absences deleted: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

std::vector<Group> MYSQLGroupsRepository::GetGroups()
{
	std::vector<Group> groups;

	if (mysql_query(this->mysql_connection, SELECT_ALL_GROUP_STMT))
	{
		wxLogDebug("#Failed to retrive groups list, Error : %s!",
			mysql_error(this->mysql_connection));
		return groups;
	}

	MYSQL_RES* result = mysql_store_result(this->mysql_connection);
	MYSQL_ROW row;

	while ((row = mysql_fetch_row(result)) != nullptr)
	{
		Group group(atoi(row[0]),
					row[1],
					atoi(row[2]),
					atof(row[3]),
					atof(row[4]),
					atof(row[5]),
					atoi(row[6]),
					atoi(row[0])
		);

		groups.push_back(group);
	}

	mysql_free_result(result);

	return groups;
}

std::vector<Absence> MYSQLGroupsRepository::GetAbsence(Group* group ,Person* person )
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
		return absences;
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'SELECT ABSENCE' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return absences;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'SELECT ABSENCE' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return absences;
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
		return absences;
	}

	if (mysql_stmt_store_result(stmt))
	{
		wxLogDebug("#Failed to store result 'SELECT ABSENCE', Error : %s!",
			mysql_error(this->mysql_connection));
		return absences;
	}

	int rows_num = mysql_stmt_num_rows(stmt);
	//wxLogDebug("Num of rows:%d", rows_num);

	date[254] = '\0';

	// Looks like mysql_stmt_fetch() doesn't get all row
	for(int i = 0; i < rows_num; i++)
	{
		mysql_stmt_data_seek(stmt, i);

		Absence absence(student_id, group_id, date, type);

		absences.push_back(absence);
	}

	wxLogDebug("[%s] ID = %d, ROWS : %d",
		__FUNCTION__, id,rows_num);
	
	mysql_stmt_close(stmt);

	return absences;
}

void MYSQLGroupsRepository::GetGroupLessons(Group& group)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[1];
	MYSQL_BIND result[8];
	char formated_dates[7][255]; // Result buffer
	unsigned long length[7];
	int x = 0; 

	if (mysql_stmt_prepare(stmt, SELECT_LESSONS_STMT,
		sizeof(SELECT_LESSONS_STMT)))
	{
		wxLogDebug("#Failed to prepare 'SELECT LESSONS' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &group.id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'SELECT LESSONS' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'SELECT LESSONS' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}
	// ------------------------------

	memset(result, 0, sizeof(result));
	memset(formated_dates, 0, sizeof(formated_dates));

	result[0].buffer_type = MYSQL_TYPE_LONG;
	result[0].buffer = &x;

	for (int i = 1; i < 8; i++)
	{
		result[i].buffer_type = MYSQL_TYPE_STRING;
		result[i].buffer = (char*)formated_dates[i-1];
		result[i].buffer_length = 255;
		result[i].length = &length[i-1];
	}

	if (mysql_stmt_bind_result(stmt, result))
	{
		wxLogDebug("#Failed to bind result 'SELECT LESSONS' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}

	if (mysql_stmt_store_result(stmt))
	{
		wxLogDebug("#Failed to store result 'SELECT LESSONS', Error : %s!",
			mysql_error(this->mysql_connection));
		return;
	}
	
	wxLogDebug("[%s] Statement Fetch return %d ", 
		__FUNCTION__,
		mysql_stmt_fetch(stmt));

	for (int i = 0; i < 7; i++)
	{
		Lesson::DeformateDate(group.lessons[i], formated_dates[i]);
		//wxLogDebug("Day %d, Time: %s to %s",
			//i, group.lessons[i].beginning, group.lessons[i].end);
	}

	mysql_stmt_close(stmt);
}

MYSQLGroupsRepository::MYSQLGroupsRepository(MYSQL* mysql_connection)
{
	this->mysql_connection = mysql_connection;
}
