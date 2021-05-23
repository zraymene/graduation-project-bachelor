#pragma once

#include <string>
#include <vector>

class Person {
public:
	unsigned int id;
	std::string first_name;
	std::string last_name;
	std::string phone;

	Person();
	Person(int id, std::string first_name,
		std::string last_name,
		std::string phone);
};

class PersonsRepository
{
public:
	// is_student: defualt value is true
	// false will treat person as a teacher
	virtual void AddPerson(Person& person, bool is_student = true) = 0;
	virtual void EditPerson(Person person, bool is_student = true) = 0;
	virtual void DeletePerson(Person person, bool is_student = true) = 0;
	
	virtual std::vector<Person> GetPersons(bool is_student = true) = 0;
};

