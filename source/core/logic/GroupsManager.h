#pragma once

#include "../Application.h"

class Application;

class GroupsManager
{
public:
	GroupsManager(Application* app);
	~GroupsManager();

	void RegisterGroup(Group* group);
	void UpdateGroup(Group group);
	void DeleteGroup(int group_id);
	void PayTeacher(Group group, int num_months);
	void EnrollStudent(int student_id, int group_id);
	void DisenrollStudent(int student_id, int group_id);
	void CollectStudentFees(Group group);

	Person GetGroupTeacher(Group);

	std::vector<Group> GetGroups();
	std::vector<Person> GetGroupMembers(int group_id);
	
private:
	Application* app;
};

