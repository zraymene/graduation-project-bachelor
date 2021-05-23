#pragma once

#include "../PersonsRepository.h"
#include "MYSQLUnitOfWork.h"

class MYSQLPersonsRepository : public PersonsRepository
{
public:
    void AddPerson(Person& person, bool is_student = true);
    void EditPerson(Person person, bool is_student = true);
    void DeletePerson(Person person, bool is_student = true);

    std::vector<Person> GetPersons(bool is_student = true);

    MYSQLPersonsRepository(MYSQL* mysql_connection);

private:
    MYSQL* mysql_connection;
};

