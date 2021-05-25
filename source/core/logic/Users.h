#pragma once

#include "../Application.h"
#include "../dal/UserRepository.h"

class Application;

class Users
{
public:
	Users();
	Users(Application* app);
	~Users();
	
	bool Login(User user);
	bool RegisterUser(User user);
	bool ModifyUser(User user);

	User* user = nullptr;

private :
	Application* app;
};

