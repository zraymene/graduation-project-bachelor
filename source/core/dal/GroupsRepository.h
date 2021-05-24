#pragma once

#include <string>
#include <vector>

#include "PersonsRepository.h"

class Absence {
public:
	unsigned int student_id;
	unsigned int group_id;
	std::string date; // Format ex : 2013-08-05 18:19:03
	bool type;  // True absent, false present

	Absence();
	Absence(int student_id, int group_id, std::string date, bool type);
};

class Lesson {
public:
	std::string beginning;
	std::string end;

	Lesson();
	Lesson(std::string beginning, std::string end);

	static std::string FormateDate(Lesson lesson);
	static void DeformateDate(Lesson& lesson, std::string full_date);

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
	int teacher_payed_times = 0;
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

	virtual void EditGroupLessons(Group group) = 0;

	virtual void AddAbsence(Absence absence) = 0;
	virtual void EditAbsence(Absence absence) = 0;
	virtual void DeleteAbsence(Absence absence) = 0;
	// Delete all group's or student's absence
	virtual void DeleteAbsences(int id, bool student_absence = false) = 0;

	virtual std::vector<Group> GetGroups() = 0;
	virtual std::vector<Absence> GetAbsence(Group* group = nullptr,
		Person* person = nullptr) = 0;

	// Return lessons array in args instead of returning it.
	virtual void GetGroupLessons(Group& group) = 0;
protected:
	virtual void AddGroupLessons(Group group) = 0;
	virtual void DeleteGroupLessons(int id) = 0;
};

