#include "MYSQLGroupsRepository.h"

#define INSERT_GROUP_STMT ""
#define DELETE_GROUP_STMT ""
#define UPDATE_GROUP_STMT ""
#define SELECT_GROUP_STMT ""

#define SELECT_LESSONS_STMT ""
#define UPDATE_LESSONS_STMT ""
#define DELETE_LESSONS_STMT ""
#define INSERT_GROUP_STMT ""

void MYSQLGroupsRepository::AddGroup(Group& group)
{
}

void MYSQLGroupsRepository::DeleteGroup(int id)
{
}

void MYSQLGroupsRepository::EditGroup(Group group)
{
}

void MYSQLGroupsRepository::AddGroupLessons(Group group)
{
}

void MYSQLGroupsRepository::DeleteGroupLessons(int id)
{
}

void MYSQLGroupsRepository::EditGroupLessons(Group group)
{
}

std::vector<Group> MYSQLGroupsRepository::GetGroups()
{
	return std::vector<Group>();
}

void MYSQLGroupsRepository::GetGroupLessons(int id, Lesson lessons[7])
{
}

MYSQLGroupsRepository::MYSQLGroupsRepository(MYSQL* mysql_connection)
{
	this->mysql_connection = mysql_connection;
}
