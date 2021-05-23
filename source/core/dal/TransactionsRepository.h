#pragma once
#include <string>
#include <vector>

class Transaction
{
public:
	unsigned int id;
	std::string name;
	double amount;
	std::string date;

	Transaction();
	Transaction(int id, std::string name,
		double amount, std::string date);
};

class TransactionsRepository
{
public:
	virtual void AddTransaction(Transaction& transaction) = 0;
	virtual void DeleteTransaction(int id) = 0;
	virtual void EditTransaction(Transaction transaction) = 0;

	virtual std::vector<Transaction> GetTransactions() = 0;
};

