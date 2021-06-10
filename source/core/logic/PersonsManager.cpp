#include "PersonsManager.h"

#define PERSON_REP this->app->GetUnitOfWork()->GetPersonsRepository()
#define TRANSACTIONS_REP this->app->GetUnitOfWork()->GetTransactionsRepository()
#define GROUPS_REP this->app->GetUnitOfWork()->GetGroupsRepository()

std::vector<Group> teacher_groups;

PersonsManager::PersonsManager(Application* app)
{
	this->app = app;
}

PersonsManager::~PersonsManager()
{
}

std::vector<Person> PersonsManager::GetStudentsList()
{
	return PERSON_REP->GetPersons();
}

std::vector<Person> PersonsManager::GetTeachersList()
{
	return PERSON_REP->GetPersons(false);
}

std::vector<Group> PersonsManager::GetStudentGroupsEnrolledIn(int id)
{
	std::vector<Group> g_tmp = GROUPS_REP->GetGroups();
	std::vector<Group>::iterator g_iter;
	std::vector<Enrollment> e_tmp = GROUPS_REP->GetEnrollments(id, false);
	std::vector<Enrollment>::iterator e_iter;
	std::vector<Group> g;

	for (e_iter = e_tmp.begin(); e_iter < e_tmp.end(); e_iter++)
	{
		for (g_iter = g_tmp.begin(); g_iter < g_tmp.end(); g_iter++)
		{
			if ((*g_iter).id == (*e_iter).group_id)
			{
				g.push_back((*g_iter));
			}
		}
	}

	return g;
}

std::vector<StudentAbsense> PersonsManager::GetStudentAbsense(int id)
{
	std::vector<StudentAbsense> sa;

	Person p;
	p.id = id;

	std::vector<Absence> a_tmp = GROUPS_REP->GetAbsence(nullptr, &p);
	std::vector<Absence>::iterator a_iter;
	std::vector<Group> g_tmp = GROUPS_REP->GetGroups();
	std::vector<Group>::iterator g_iter;

	for (a_iter = a_tmp.begin(); a_iter < a_tmp.end(); a_iter++)
	{
		// It's not an absence
		if (!(*a_iter).type)
			continue;

		for (g_iter = g_tmp.begin(); g_iter < g_tmp.end(); g_iter++)
		{
			if ((*g_iter).id == (*a_iter).group_id)
			{		
				sa.push_back({ (*g_iter), (*a_iter) });
			}
		}
	}

	return sa;
}

std::vector<Group> PersonsManager::GetTeacherGroups(int id)
{
	std::vector<Group> g_tmp = GROUPS_REP->GetGroups();
	std::vector<Group>::iterator g_iter;
	std::vector<Group> g;

	for (g_iter = g_tmp.begin(); g_iter < g_tmp.end(); g_iter++)
	{
		if ((*g_iter).teacher_id == id)
		{
			g.push_back((*g_iter));
		}
	}
	teacher_groups = g;
	return g;
}

void PersonsManager::RegisterStudent(Person* student)
{
	if((*student).first_name.empty())
		throw std::exception("Student first name is empty!");
	if((*student).last_name.empty())
		throw std::exception("Student last name is empty!");


	this->app->GetUnitOfWork()->Begin();
	PERSON_REP->AddPerson(*student);
	this->app->GetUnitOfWork()->Commit();
}

void PersonsManager::RegisterTeacher(Person* teacher)
{
	if ((*teacher).first_name.empty())
		throw std::exception("Teacher first name is empty!");
	if ((*teacher).last_name.empty())
		throw std::exception("Teacher last name is empty!");


	this->app->GetUnitOfWork()->Begin();
	PERSON_REP->AddPerson(*teacher, false);
	this->app->GetUnitOfWork()->Commit();
}

void PersonsManager::UpdateStudent(Person student)
{
	if (student.first_name.empty())
		throw std::exception("Student first name is empty!");
	if (student.last_name.empty())
		throw std::exception("Student last name is empty!");

	this->app->GetUnitOfWork()->Begin();
	PERSON_REP->EditPerson(student);
	this->app->GetUnitOfWork()->Commit();
}

void PersonsManager::UpdateTeacher(Person teacher)
{
	if (teacher.first_name.empty())
		throw std::exception("Teacher first name is empty!");
	if (teacher.last_name.empty())
		throw std::exception("Teacher last name is empty!");

	this->app->GetUnitOfWork()->Begin();
	PERSON_REP->EditPerson(teacher, false);
	this->app->GetUnitOfWork()->Commit();
}

void PersonsManager::DeleteStudent(int id)
{
	this->app->GetUnitOfWork()->Begin();
	PERSON_REP->DeletePerson(id);
	GROUPS_REP->DeleteAbsences(id, true);
	GROUPS_REP->DeleteAllEnrollments(id, false);
	this->app->GetUnitOfWork()->Commit();
}

void PersonsManager::DeleteTeacher(int id)
{
	this->app->GetUnitOfWork()->Begin();
	PERSON_REP->DeletePerson(id, false);
	
	// Delete all groups associated to the teacher
	std::vector<Group>::iterator g_iter;
	for (g_iter = teacher_groups.begin(); g_iter < teacher_groups.end(); g_iter++)
	{
		if ((*g_iter).teacher_id == id)
		{
			GROUPS_REP->DeleteGroup((*g_iter).id);
		}
	}

	this->app->GetUnitOfWork()->Commit();
}

