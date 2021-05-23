#include "PersonsRepository.h"

Person::Person()
{
}

Person::Person(int id, std::string first_name, std::string last_name, std::string phone)
{
	this->id = id;
	this->first_name = first_name;
	this->last_name = last_name;
	this->phone = phone;
}
