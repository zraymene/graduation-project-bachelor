#include "TransactionsManager.h"

#define TRANS_REP this->app->GetUnitOfWork()->GetTransactionsRepository()

TransactionsManager::TransactionsManager(Application* app)
{
	this->app = app;
}

TransactionsManager::~TransactionsManager()
{
}

void TransactionsManager::AddTransaction(Transaction* transaction)
{
	if ((*transaction).name.empty())
		throw std::runtime_error("Transaction's name is empty!");
	if ((*transaction).amount == 0)
		throw std::runtime_error("Transaction's amount mustn't be null!");

	this->app->GetUnitOfWork()->Begin();
	TRANS_REP->AddTransaction(*transaction);
	this->app->GetUnitOfWork()->Commit();
}

void TransactionsManager::EditTransaction(Transaction transaction)
{
	if (transaction.name.empty())
		throw std::runtime_error("Transaction's name is empty!");
	if (transaction.amount == 0)
		throw std::runtime_error("Transaction's amount mustn't be null!");

	this->app->GetUnitOfWork()->Begin();
	TRANS_REP->EditTransaction(transaction);
	this->app->GetUnitOfWork()->Commit();
}

void TransactionsManager::DeleteTransaction(int transaction_id)
{
	this->app->GetUnitOfWork()->Begin();
	TRANS_REP->DeleteTransaction(transaction_id);
	this->app->GetUnitOfWork()->Commit();
}

std::vector<Transaction> TransactionsManager::GetTransactions()
{
	return TRANS_REP->GetTransactions();
}
