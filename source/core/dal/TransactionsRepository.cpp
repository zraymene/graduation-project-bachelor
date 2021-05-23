#include "TransactionsRepository.h"

Transaction::Transaction()
{
}

Transaction::Transaction(int id, std::string name, double amount, std::string date)
{
	this->id = id;
	this->name = name;
	this->amount = amount;
	this->date = date;
}
