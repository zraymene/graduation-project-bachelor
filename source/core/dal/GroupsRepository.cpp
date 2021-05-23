#include "GroupsRepository.h"

Lesson::Lesson()
{
};

Lesson::Lesson(std::string begining, std::string end)
{
	this->beginning = beginning;
	this->end = end;
};

Group::Group()
{
};

Group::Group(int id, std::string name, int teacher_id, double price,
	float school_percentage, float teacher_percentage,
	int lessons_per_week, int teacher_payed_times)
{
	this->id = id;
	this->name = name;
	this->teacher_id = teacher_id;
	this->price = price;
	this->school_percentage = school_percentage;
	this->teacher_percentage = teacher_percentage;
	this->lessons_per_week = lessons_per_week;
	this->teacher_payed_times = teacher_payed_times;
};
