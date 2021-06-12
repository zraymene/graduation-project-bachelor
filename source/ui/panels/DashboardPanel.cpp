#include "../MainPage.h"

#include "wx/wx.h"
#include <wx/numformatter.h>

#define STAT_MANAGER this->main->app->GetStatisticsManager()


/*
	TODO:
	• Make the chart update when changing pages

*/

DashboardPanel::DashboardPanel(MainPage* main)
{
	this->main = main;

	//this->panel = wxXmlResource::Get()->LoadPanel(nullptr, "dashboard_tab");

	this->panel = XRCCTRL(*(this->main->frame), "dashboard_tab", wxPanel);
	
	this->stat_teachers_count_ctr = XRCCTRL(*(this->main->frame), "stat_teachers_count_ctr", wxTextCtrl);
	this->stat_students_count_ctr = XRCCTRL(*(this->main->frame), "stat_students_count_ctr", wxTextCtrl);
	this->stat_groups_count_ctr = XRCCTRL(*(this->main->frame), "stat_groups_count_ctr", wxTextCtrl);
	this->stat_total_expenses_ctr = XRCCTRL(*(this->main->frame), "stat_total_expenses_ctr", wxTextCtrl);
	this->stat_total_profits_ctr = XRCCTRL(*(this->main->frame), "stat_total_profits_ctr", wxTextCtrl);
	this->stat_net_profit_ctr = XRCCTRL(*(this->main->frame), "stat_net_profit_ctr", wxTextCtrl);

	this->UpdateDashboard();

	this->PopulateCharts();
}

DashboardPanel::~DashboardPanel()
{

}

void DashboardPanel::UpdateDashboard()
{
	this->stat_teachers_count_ctr->SetValue(
		std::to_string(STAT_MANAGER->GetTeachersCount())
	);
	this->stat_students_count_ctr->SetValue(
		std::to_string(STAT_MANAGER->GetStudentCount())
	);
	this->stat_groups_count_ctr->SetValue(
		std::to_string(STAT_MANAGER->GetGroupsCount())
	);

	this->stat_total_expenses_ctr->SetValue(
		wxNumberFormatter::ToString(STAT_MANAGER->CalculateExpenses(),3)
	);
	this->stat_total_profits_ctr->SetValue(
		wxNumberFormatter::ToString(STAT_MANAGER->CalculateProfit(),3)
	);

	if (STAT_MANAGER->CalculateNetProfits() > 0)
		this->stat_net_profit_ctr->SetForegroundColour(*wxGREEN);
	else
		this->stat_net_profit_ctr->SetForegroundColour(*wxRED);

	this->stat_net_profit_ctr->SetValue(
		wxNumberFormatter::ToString(STAT_MANAGER->CalculateNetProfits(),3)
	);

}

void DashboardPanel::PopulateCharts()
{
	// This utterly stupid
	// Froced to use wxVector<wxString>
	// So forced to converte std::vector<std::string>
	wxVector<wxString> labels;
	std::vector<std::string> str = STAT_MANAGER->GetMonthsList();
	std::vector<std::string>::iterator iter;
	for (iter = str.begin(); iter < str.end(); iter++)
	{
		labels.push_back((*iter).c_str());
	}
	wxChartsCategoricalData::ptr chartData = wxChartsCategoricalData::make_shared(labels);

	wxVector<wxDouble> points1;
	std::vector<double> expenses = STAT_MANAGER->GetMonthlyExpenses();
	std::vector<double>::iterator iter1;
	for (iter1 = expenses.begin(); iter1 < expenses.end(); iter1++)
	{
		points1.push_back((*iter1));
	}
	wxChartsDoubleDataset::ptr dataset1(new wxChartsDoubleDataset("Expenses", points1));
	chartData->AddDataset(dataset1);

	wxVector<wxDouble> points2;
	std::vector<double> profits = STAT_MANAGER->GetMonthlyProfits();
	std::vector<double>::iterator iter2;
	for (iter2 = profits.begin(); iter2 < profits.end(); iter2++)
	{
		points2.push_back((*iter2));
	}
	wxChartsDoubleDataset::ptr dataset2(new wxChartsDoubleDataset("Profits", points2));
	chartData->AddDataset(dataset2);

	this->stat_transactions_chart = new wxColumnChartCtrl(this->panel, wxID_ANY, chartData);
	this->stat_transactions_chart->SetMinSize(wxSize(650, 300));
	wxStaticBoxSizer* sbz = new wxStaticBoxSizer(wxHORIZONTAL, this->panel, "Transactions chart:");

	sbz->Add(stat_transactions_chart, 0, wxALL | wxEXPAND, 5);
	wxGridBagSizer* gbs = (wxGridBagSizer*)this->panel->GetSizer();

	gbs->Add(sbz, wxGBPosition(0, 1), wxGBSpan(2, 2), wxALL | wxEXPAND, 5);
	gbs->Layout();
}
