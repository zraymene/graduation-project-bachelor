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
	std::vector<std::string> GetMonthsList();

private:
	Application* app;

	void PopulateDataset();

	std::vector<double> profits_dataset;
	std::vector<double> expenses_dataset;

};

