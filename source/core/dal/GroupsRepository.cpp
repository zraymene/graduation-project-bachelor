#include "GroupsRepository.h"

Lesson::Lesson()
{
};

Lesson::Lesson(std::string begining, std::string end)
{
	this->beginning = beginning;
	this->end = end;

}
std::string Lesson::FormateDate(Lesson lesson)
{
	std::string formated_date;
	if (lesson.beginning.empty() && lesson.end.empty())
		return formated_date;

	formated_date = lesson.beginning;
	formated_date += '|';
	formated_date += lesson.end;
	return formated_date;
}
void Lesson::DeformateDate(Lesson& lesson, std::string full_date)
{
	if (full_date.empty())
		return;

	size_t pos = full_date.find('|');
	lesson.beginning = full_date.substr(0, pos);
	lesson.end = full_date.substr(pos + 1, full_date.length());
}

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

Absence::Absence()
{
}

Absence::Absence(int student_id, int group_id, std::string date, bool type)
{
	this->student_id = student_id;
	this->group_id = group_id;
	this->date = date;
	this->type = type;
}
