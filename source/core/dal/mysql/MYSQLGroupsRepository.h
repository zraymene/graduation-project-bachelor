#pragma once

#include "../GroupsRepository.h"
#include "MYSQLUnitOfWork.h"

class MYSQLGroupsRepository : public GroupsRepository
{
public:
	void AddGroup(Group& group);
	void DeleteGroup(int id);
	void EditGroup(Group group);
	
	void EditGroupLessons(Group group);

	void AddAbsence(Absence absence);
	// Editing absence using group_id AND student_id AND date
	void EditAbsence(Absence absence);
	// Deleting absence using group_id AND student_id AND date
	void DeleteAbsence(Absence absence); 
	void DeleteAbsences(int id, bool student_absence = false);

	void AddEnrollment(int group_id, int student_id);
	void DeleteEnrollment(int group_id, int student_id);
	void DeleteAllEnrollments(int id, bool is_group = true);
	std::vector<Enrollment> GetEnrollments(int id, bool is_group = true);

	std::vector<Group> GetGroups();
	std::vector<Absence> GetAbsence(Group* group = nullptr,
		Person* person = nullptr);
	// Return lessons array in args instead of returning it.
	void GetGroupLessons(Group& group);

	MYSQLGroupsRepository(MYSQL* mysql_connection);

private:
	MYSQL* mysql_connection;

	void DeleteGroupLessons(int id);
	void AddGroupLessons(Group group);
};

