#pragma once

#include "../Application.h"

class Application;


class StatisticsManager
{
public:
	StatisticsManager(Application* app);
	~StatisticsManager();

	int GetStudentCount();
	int GetTeachersCount();
	int GetGroupsCount();
	double CalculateProfit();
	double CalculateNetProfits();
	double CalculateExpenses();
	std::vector<double> GetMonthlyProfits();
	std::vector<double> GetMonthlyExpenses();

private:
	Application* app;
};

