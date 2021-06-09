#pragma once
#include "../Application.h"

class Application; 

struct StudentAbsense
{
	Group group;
	Absence absense;
};

class PersonsManager
{
public:

	PersonsManager(Application* app);
	~PersonsManager();

	std::vector<Person> GetStudentsList();
	std::vector<Group> GetStudentGroupsEnrolledIn(int id);
	std::vector<StudentAbsense> GetStudentAbsense(int id);


	void RegisterStudent(Person* student);
	void RegisterTeacher(Person* teacher);
	void UpdateStudent(Person student);
	void UpdateTeacher(Person teacher);
	void DeleteStudent(int id);
	void DeleteTeacher(int id);

private:
	Application* app;
};

