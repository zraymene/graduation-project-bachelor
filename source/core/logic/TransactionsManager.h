#pragma once

#include "../Application.h"

class Application;


class TransactionsManager
{
public:
	TransactionsManager(Application* app);
	~TransactionsManager();

	void AddTransaction(Transaction *transaction);
	void EditTransaction(Transaction transaction);
	void DeleteTransaction(int transaction_id);
	std::vector<Transaction> GetTransactions();

	double CalculateProfit();
	double CalculateNetProfit();
	double CalculateExpenses();

private: 
	Application* app;
};

