#pragma once

#include "../GroupsRepository.h"
#include "MYSQLUnitOfWork.h"

class MYSQLGroupsRepository : public GroupsRepository
{
public:
	void AddGroup(Group& group);
	void DeleteGroup(int id);
	void EditGroup(Group group);
	
	void AddGroupLessons(Group group);
	void DeleteGroupLessons(int id);
	void EditGroupLessons(Group group);

	std::vector<Group> GetGroups();
	// Return lessons array in args instead of returning it.
	void GetGroupLessons(int id, Lesson lessons[7]);

	MYSQLGroupsRepository(MYSQL* mysql_connection);

private:
	MYSQL* mysql_connection;


};

