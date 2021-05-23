#pragma once

#include <string>
#include <vector>

class Lesson {
public:
	std::string beginning;
	std::string end;

	Lesson();
	Lesson(std::string beginning, std::string end);
};

class Group {
public:
	unsigned int id;
	std::string name;
	unsigned int teacher_id;
	double price;
	float school_percentage;
	float teacher_percentage;
	int lessons_per_week;
	int teacher_payed_times;
	Lesson lessons[7];

	Group();
	Group(int id, std::string name, int teacher_id, double price,
		float school_percentage, float teacher_percentage,
		int lessons_per_wekk, int teacher_payed_times);
};

class GroupsRepository
{
public:
	virtual void AddGroup(Group& group) = 0;
	virtual void DeleteGroup(int id) = 0;
	virtual void EditGroup(Group group) = 0;

	virtual void AddGroupLessons(Group group) = 0;
	virtual void DeleteGroupLessons(int id) = 0;
	virtual void EditGroupLessons(Group group) = 0;

	virtual std::vector<Group> GetGroups() = 0;

	// Return lessons array in args instead of returning it.
	virtual void GetGroupLessons(int id, Lesson lessons[7]) = 0;
};

