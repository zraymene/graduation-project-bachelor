#include "Users.h"

#define USER_REP this->app->GetUnitOfWork()->GetUserRepository()


Users::Users(Application* app)
{
	this->app = app;
}

Users::~Users()
{
	delete this->user;
}

bool Users::Login(User user)
{
	if (!(this->user = USER_REP->GetUser(user.username, user.password)))
		return false;

	return true;
}

bool Users::RegisterUser(User user)
{
	app->GetUnitOfWork()->Begin();
	USER_REP->AddUser(user);

	if (!app->GetUnitOfWork()->Commit())
		return false;

	return true;
}

bool Users::ModifyUser(User user)
{
	this->app->GetUnitOfWork()->Begin();
	USER_REP->EditUser(user);

	if (!app->GetUnitOfWork()->Commit())
		return false;

	return true;
}
