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

void Users::Login(User user)
{
	this->user = USER_REP->GetUser(user.username, user.password);
}

void Users::RegisterUser(User user)
{
	app->GetUnitOfWork()->Begin();

	USER_REP->AddUser(user);

	app->GetUnitOfWork()->Commit();

}

void Users::ModifyUser(User user)
{
	app->GetUnitOfWork()->Begin();

	USER_REP->EditUser(user);

	app->GetUnitOfWork()->Commit();
}
