#pragma once

#include "../Application.h"
#include "../dal/UserRepository.h"

class Application;

class Users
{
public:

	Users(Application* app);
	~Users();
	
	void Login(User user);
	void RegisterUser(User user);
	void ModifyUser(User user);

	User* user = nullptr;

private :
	Application* app;
};

