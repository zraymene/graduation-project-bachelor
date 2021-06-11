#include "MYSQLUserRepository.h"

#define INSERT_USER_STMT "INSERT INTO `school_app`.`user`(username, password)\
										VALUES(?,?)"
#define SELECT_USER_STMT "SELECT * FROM `school_app`.`user` WHERE `username` = ? AND\
								`password` = ?"
#define UPDATE_USER_STMT "UPDATE `school_app`.`user` SET `password`=?\
									 WHERE `username`=?"

void MYSQLUserRepository::AddUser(User user)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[2];

	if (mysql_stmt_prepare(stmt, INSERT_USER_STMT, strlen(INSERT_USER_STMT)))
	{
		wxLogDebug("#Failed to prepare 'INSERT USER' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'INSERT USER' statement!");
	}
	 
	memset(bind, 0, sizeof(bind));

	unsigned long username_length = user.username.length();
	unsigned long password_length = user.password.length();

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (char*) user.username.c_str();
	bind[0].length = &username_length;

	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (char*) user.password.c_str();
	bind[1].length = &password_length;
	
	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'INSERT USER' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind 'INSERT USER' statement!");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'INSERT USER' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submit 'INSERT USER' statement!");
	}

	wxLogDebug("User added.");

	mysql_stmt_close(stmt);
}

void MYSQLUserRepository::EditUser(User user)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[2];

	if (mysql_stmt_prepare(stmt, UPDATE_USER_STMT, strlen(UPDATE_USER_STMT)))
	{
		wxLogDebug("#Failed to prepare 'UPDATE USER' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'UPDATE USER' statement!");
	}
	
	memset(bind, 0, sizeof(bind));

	unsigned long username_length = user.username.length();
	unsigned long password_length = user.password.length();

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (char*)user.password.c_str();
	bind[0].length = &password_length;

	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (char*)user.username.c_str();
	bind[1].length = &username_length;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'UPDATE USER' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to bind 'UPDATE USER' statement!");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'UPDATE USER' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submit 'UPDATE USER' statement!");
	}

	wxLogDebug("Users updated: %d.",(int)mysql_stmt_affected_rows(stmt));

	mysql_stmt_close(stmt);
}

User *MYSQLUserRepository::GetUser(std::string username, std::string password)
{
	MYSQL_STMT* stmt = mysql_stmt_init(this->mysql_connection);
	MYSQL_BIND bind[2];

	if (mysql_stmt_prepare(stmt, SELECT_USER_STMT, strlen(SELECT_USER_STMT)))
	{
		wxLogDebug("#Failed to prepare 'SELECT USER' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'SELECT USER' statement!");
	}
	
	memset(bind, 0, sizeof(bind));

	unsigned long username_length = username.length();
	unsigned long password_length = password.length();

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (char*)username.c_str();
	bind[0].length = &username_length;

	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (char*)password.c_str();
	bind[1].length = &password_length;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		wxLogDebug("#Failed to binding 'SELECT USER' statement, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to prepare 'SELECT USER' statement!");
	}

	if (mysql_stmt_execute(stmt))
	{
		wxLogDebug("#Failed submitting 'SELECT USER' queries, Error : %s!",
			mysql_error(this->mysql_connection));
		throw std::runtime_error("Failed to submitting 'SELECT USER' statement!");
	}

	if (mysql_stmt_fetch(stmt) == MYSQL_NO_DATA)
	{
		wxLogDebug("#User not found !");
		throw std::runtime_error("Authentication failed!");
	}

	wxLogDebug("Found user!");

	mysql_stmt_close(stmt);

	return new User(username, password);
}


MYSQLUserRepository::MYSQLUserRepository(MYSQL* mysql_connection)
{
	this->mysql_connection = mysql_connection;
}
