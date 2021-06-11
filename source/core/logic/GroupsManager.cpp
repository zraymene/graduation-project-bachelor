#include "GroupsManager.h"

#define GROUPS_REP this->app->GetUnitOfWork()->GetGroupsRepository()
#define PERSON_REP this->app->GetUnitOfWork()->GetPersonsRepository()
#define TRANS_REP this->app->GetUnitOfWork()->GetTransactionsRepository()

GroupsManager::GroupsManager(Application* app)
{
	this->app = app;
}

GroupsManager::~GroupsManager()
{
}

void GroupsManager::RegisterGroup(Group* group)
{
	if ((*group).name.empty())
		throw std::runtime_error("Group's name is empty!");
	if ((*group).price <= 0)
		throw std::runtime_error("Group's price must be set !");
	if ((*group).school_percentage <= 0)
		throw std::runtime_error("School's percentage must be set !");
	if ((*group).teacher_percentage < 0)
		throw std::runtime_error("Teachers's percentage is invalid!");
	if ((*group).lessons_per_week <= 0)
		throw std::runtime_error("Group's lessons per week must be set !");

	this->app->GetUnitOfWork()->Begin();
	GROUPS_REP->AddGroup(*group);
	
	this->app->GetUnitOfWork()->Commit();
}

void GroupsManager::UpdateGroup(Group group)
{
	this->app->GetUnitOfWork()->Begin();
	GROUPS_REP->EditGroup(group);
	this->app->GetUnitOfWork()->Commit();
}

void GroupsManager::DeleteGroup(int group_id)
{
	this->app->GetUnitOfWork()->Begin();
	GROUPS_REP->DeleteGroup(group_id);
	GROUPS_REP->DeleteAllEnrollments(group_id);
	GROUPS_REP->DeleteAbsences(group_id);
	this->app->GetUnitOfWork()->Commit();
}

void GroupsManager::PayTeacher(Group group, int num_months)
{
	Transaction t;
	t.amount = ((group.price * group.teacher_percentage)/100) * -1 * num_months;
	t.name = group.name;
	t.name += "-TEACHERx";
	t.name += std::to_string(num_months);

	this->app->GetUnitOfWork()->Begin();
	TRANS_REP->AddTransaction(t);
	this->app->GetUnitOfWork()->Commit();
}

void GroupsManager::EnrollStudent(int student_id, int group_id)
{
	this->app->GetUnitOfWork()->Begin();
	GROUPS_REP->AddEnrollment(group_id, student_id);
	this->app->GetUnitOfWork()->Commit();
}

void GroupsManager::DisenrollStudent(int student_id, int group_id)
{
	this->app->GetUnitOfWork()->Begin();
	GROUPS_REP->DeleteEnrollment(group_id, student_id);
	this->app->GetUnitOfWork()->Commit();
}

void GroupsManager::CollectStudentFees(Group group)
{
	std::vector<Person> members = this->GetGroupMembers(group.id);

	if (members.size() == 0)
		return;

	Transaction t;
	t.amount = group.price * members.size();
	t.name = group.name;
	t.name += "-STUDENTSx";
	t.name += std::to_string(members.size());

	this->app->GetUnitOfWork()->Begin();
	TRANS_REP->AddTransaction(t);
	this->app->GetUnitOfWork()->Commit();
}

Person GroupsManager::GetGroupTeacher(Group group)
{
	Person teacher;

	std::vector<Person> teachers = PERSON_REP->GetPersons(false);
	std::vector<Person>::iterator teacher_iter;

	for (teacher_iter = teachers.begin(); teacher_iter < teachers.end(); teacher_iter++)
	{
		if (group.teacher_id == (*teacher_iter).id)
		{
			teacher = (*teacher_iter);
			break;
		}
	}

	return teacher;
}

std::vector<Group> GroupsManager::GetGroups()
{
	std::vector<Group> g = GROUPS_REP->GetGroups();
	std::vector<Group>::iterator group_iter;
	
	// Load group's lessons too
	for (group_iter = g.begin(); group_iter < g.end(); group_iter++)
	{
		GROUPS_REP->GetGroupLessons(*group_iter);
	}

	return g;
}

std::vector<Person> GroupsManager::GetGroupMembers(int group_id)
{
	std::vector<Enrollment> enroll_tmp = GROUPS_REP->GetEnrollments(group_id);
	std::vector<Enrollment>::iterator enroll_iter;
	std::vector<Person> students_tmp = PERSON_REP->GetPersons();
	std::vector<Person>::iterator student_iter;
	std::vector<Person> memebers;

	for (enroll_iter = enroll_tmp.begin(); enroll_iter < enroll_tmp.end(); enroll_iter++)
	{
		for (student_iter = students_tmp.begin(); student_iter < students_tmp.end(); student_iter++)
		{
			if ((*enroll_iter).student_id == (*student_iter).id)
			{
				memebers.push_back((*student_iter));
			}
		}
	}

	return memebers;
}
