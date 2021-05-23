#pragma once

#include "../TransactionsRepository.h"
#include "MYSQLUnitOfWork.h"

class MYSQLTransactionsRepository : public TransactionsRepository
{
public:
	void AddTransaction(Transaction& transaction);
	void DeleteTransaction(int id);
	void EditTransaction(Transaction transaction);

	std::vector<Transaction> GetTransactions();

	MYSQLTransactionsRepository(MYSQL* mysql_connection);

private:
	MYSQL* mysql_connection;

};

