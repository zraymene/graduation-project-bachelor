#include "PersonsManager.h"

#define PERSON_REP this->app->GetUnitOfWork()->GetPersonsRepository()
#define TRANSACTIONS_REP this->app->GetUnitOfWork()->GetTransactionsRepository()


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
	this->app->GetUnitOfWork()->Begin();
	PERSON_REP->EditPerson(student);
	this->app->GetUnitOfWork()->Commit();
}

void PersonsManager::UpdateTeacher(Person teacher)
{
	this->app->GetUnitOfWork()->Begin();
	PERSON_REP->EditPerson(teacher, false);
	this->app->GetUnitOfWork()->Commit();
}

void PersonsManager::DeleteStudent(int id)
{
	this->app->GetUnitOfWork()->Begin();
	PERSON_REP->DeletePerson(id);
	this->app->GetUnitOfWork()->Commit();
}

void PersonsManager::DeleteTeacher(int id)
{
	this->app->GetUnitOfWork()->Begin();
	PERSON_REP->DeletePerson(id, false);
	this->app->GetUnitOfWork()->Commit();
}

