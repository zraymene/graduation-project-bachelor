#include "MYSQLTransactionsRepository.h"

#define INSERT_TRANSACTION_STMT "INSERT INTO `school_app`.`transaction`\
	(`name`, `amount`, `date`)\
	 VALUES (?,?, CURRENT_TIMESTAMP())"

#define UPDATE_TRANSACTION_STMT "UPDATE `school_app`.`transaction` SET \
	`name`=?,`amount`=?,`date`= CURRENT_TIMESTAMP()\
	WHERE `id`=?"

#define DELETE_TRANSACTION_STMT "DELETE FROM `school_app`.`transaction` WHERE id = ?"
#define SELECT_ALL_TRANSACTION_STMT "SELECT * FROM `school_app`.`transaction`"

void MYSQLTransactionsRepository::AddTransaction(Transaction& transaction)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[2];
	
	if (mysql_stmt_prepare(stmt, INSERT_TRANSACTION_STMT,
		sizeof(INSERT_TRANSACTION_STMT)))
	{
		wxLogDebug("#Failed to prepare 'INSERT TRANSACTION' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::exception("Failed to prepare 'INSERT TRANSACTION' statement");
	}

	memset(bind, 0, sizeof(bind));

	unsigned long name_length = transaction.name.length();

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (char*)transaction.name.c_str();
	bind[0].length = &name_length;

	bind[1].buffer_type = MYSQL_TYPE_DOUBLE;
	bind[1].buffer = &transaction.amount;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'INSERT TRANSACTION' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::exception("Failed to bind 'INSERT TRANSACTION' statement");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'INSERT TRANSACTION' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::exception("Failed to submit 'INSERT TRANSACTION' statement");
	}

	// Get newly AUTO_INCREMENT  id
	transaction.id = mysql_insert_id(this->mysql_connection);

	wxLogDebug("Transaction added!, Amount = %.2f", (float)transaction.amount);

	mysql_stmt_close(stmt);
}

void MYSQLTransactionsRepository::DeleteTransaction(int id)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[1];

	if (mysql_stmt_prepare(stmt, DELETE_TRANSACTION_STMT,
		sizeof(DELETE_TRANSACTION_STMT)))
	{
		wxLogDebug("#Failed to prepare 'DELETE TRANSACTION' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::exception("Failed to prepare 'DELETE TRANSACTION' statement");
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'DELETE TRANSACTION' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::exception("Failed to bind 'DELETE TRANSACTION' statement");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'DELETE TRANSACTION' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::exception("Failed to submit 'DELETE TRANSACTION' statement");
	}

	wxLogDebug("Transactions deleted: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

void MYSQLTransactionsRepository::EditTransaction(Transaction transaction)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[3];

	if (mysql_stmt_prepare(stmt, UPDATE_TRANSACTION_STMT,
		sizeof(UPDATE_TRANSACTION_STMT)))
	{
		wxLogDebug("#Failed to prepare 'UPDATE TRANSACTION' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::exception("Failed to prepare 'UPDATE TRANSACTION' statement");
	}

	memset(bind, 0, sizeof(bind));

	unsigned long name_length = transaction.name.length();

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (char*)transaction.name.c_str();
	bind[0].length = &name_length;

	bind[1].buffer_type = MYSQL_TYPE_DOUBLE;
	bind[1].buffer = &transaction.amount;

	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer = &transaction.id;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to bind 'UPDATE TRANSACTION' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::exception("Failed to bind 'UPDATE TRANSACTION' statement");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed to submit 'UPDATE TRANSACTION' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::exception("Failed to submit 'UPDATE TRANSACTION' statement");
	}

	wxLogDebug("Transactions updated: %d.", (int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

std::vector<Transaction> MYSQLTransactionsRepository::GetTransactions()
{
	std::vector<Transaction> transactions;

	if (mysql_query(this->mysql_connection, SELECT_ALL_TRANSACTION_STMT))
	{
		wxLogDebug("#Failed to retrive transactions list, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::exception("Failed to retrive transactions list");
		//return transactions;
	}

	MYSQL_RES* result = mysql_store_result(this->mysql_connection);
	MYSQL_ROW row;

	while ((row = mysql_fetch_row(result)) != nullptr)
	{
		Transaction transaction(atoi(row[0]), row[1], atof(row[2]), row[3]);
		transactions.push_back(transaction);
	}

	mysql_free_result(result);

	return transactions;
}

MYSQLTransactionsRepository::MYSQLTransactionsRepository(MYSQL* mysql_connection)
{
	this->mysql_connection = mysql_connection;
}
