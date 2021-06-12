#include "StatisticsManager.h"

#define GROUPS_REP this->app->GetUnitOfWork()->GetGroupsRepository()
#define PERSON_REP this->app->GetUnitOfWork()->GetPersonsRepository()
#define TRANS_REP this->app->GetUnitOfWork()->GetTransactionsRepository()

// Memory cache of transactions
// Updated on StatisticsManager::CalculateExpenses()
std::vector<Transaction> transactions_cache;

// Expenses total and Total profit are used in multiple
// functions, So cache them too !
double total_expenses;
double total_profit;

StatisticsManager::StatisticsManager(Application* app)
{
	this->app = app;
}

StatisticsManager::~StatisticsManager()
{
}

int StatisticsManager::GetStudentCount()
{
	return PERSON_REP->GetPersons().size();
}

int StatisticsManager::GetTeachersCount()
{
	return PERSON_REP->GetPersons(false).size();
}

int StatisticsManager::GetGroupsCount()
{
	return GROUPS_REP->GetGroups().size();
}

double StatisticsManager::CalculateProfit()
{
	std::vector<Transaction>::iterator iter;
	total_profit = 0;

	for (iter = transactions_cache.begin(); iter < transactions_cache.end(); iter++)
	{
		if ((*iter).amount > 0)
			total_profit += (*iter).amount;
	}

	return total_profit;
}

double StatisticsManager::CalculateNetProfits()
{
	// Simply, like this
	return total_profit + total_expenses;
}

double StatisticsManager::CalculateExpenses()
{
	// Update Transactions cache
	transactions_cache = TRANS_REP->GetTransactions();
	this->PopulateDataset();

	std::vector<Transaction>::iterator iter;
	total_expenses = 0;

	for (iter = transactions_cache.begin(); iter < transactions_cache.end(); iter++)
	{
		if ((*iter).amount < 0)
			total_expenses += (*iter).amount;
	}

	return total_expenses;
}

void extractDate(std::string str, int* year, int* month)
{
	int d, h, m, s;
	//2021-06-11 13:31:34
	std::sscanf(str.c_str(), "%4d-%2d-%2d %2d:%2d:%2d",
		year, month, &d, &h, &m, &s);
}

std::vector<double> StatisticsManager::GetMonthlyProfits()
{
	return this->profits_dataset;
}

std::vector<double> StatisticsManager::GetMonthlyExpenses()
{
	return this->expenses_dataset;
}

std::vector<std::string> StatisticsManager::GetMonthsList()
{
	std::vector<std::string> months_list;
	std::vector<Transaction>::iterator iter;
	total_expenses = 0;

	int lm = 0, ly = 0, y, m;

	for (iter = transactions_cache.begin(); iter < transactions_cache.end(); iter++)
	{

		extractDate((*iter).date, &y, &m);

		if (y != ly && m != lm)
		{
			//ex: 2016/01
			months_list.push_back((*iter).date.substr(0, 7));
			lm = m;
			ly = y;
		}

	}
	return months_list;
}

void StatisticsManager::PopulateDataset()
{
	std::vector<Transaction>::iterator iter;
	int year, month, lyear = 0, lmonth = 0;

	double monthly_profit;
	double monthly_expenses;

	for (iter = transactions_cache.begin(); iter < transactions_cache.end(); iter++)
	{
		extractDate((*iter).date, &year, &month);

		if (lyear != year || lmonth != month)
		{
			// It means it's the first time so don't push value 0
			if (lyear != 0 && lmonth != 0)
			{
				profits_dataset.push_back(monthly_profit);
				expenses_dataset.push_back(monthly_expenses);
			}

			monthly_profit = 0;
			monthly_expenses = 0;

			lyear = year;
			lmonth = month;
		}
		
		if ((*iter).amount > 0)
			monthly_profit += (*iter).amount;
		else
			monthly_expenses += (*iter).amount * -1;
	}

	profits_dataset.push_back(monthly_profit);
	expenses_dataset.push_back(monthly_expenses);
}
