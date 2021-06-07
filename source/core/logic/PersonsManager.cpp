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

void PersonsManager::RegisterStudent(Person student)
{
	PERSON_REP->AddPerson(student);
}

void PersonsManager::RegisterTeacher(Person teacher)
{
	PERSON_REP->AddPerson(teacher, false);
}

void PersonsManager::UpdateStudent(Person student)
{
	PERSON_REP->EditPerson(student);
}

void PersonsManager::UpdateTeacher(Person teacher)
{
	PERSON_REP->EditPerson(teacher, false);
}

