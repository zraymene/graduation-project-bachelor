#pragma once
#include "../Application.h"

class Application;

class PersonsManager
{
public:

	PersonsManager(Application* app);
	~PersonsManager();

	std::vector<Person> GetStudentsList();

	void RegisterStudent(Person* student);
	void RegisterTeacher(Person* teacher);
	void UpdateStudent(Person student);
	void UpdateTeacher(Person teacher);
	void DeleteStudent(int id);
	void DeleteTeacher(int id);

private:
	Application* app;
};

