#include "../MainPage.h"
#include <wx\xrc\xmlres.h>

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

	/*this->stat_trans_chart_sizer = (wxStaticBoxSizer*) XRCCTRL(*(this->main->frame),
		"stat_trans_chart_sizer", wxBoxSizer);
		*/

	wxVector<wxString> labels;
	labels.push_back("January");
	labels.push_back("February");
	labels.push_back("March");
	labels.push_back("April");
	labels.push_back("May");
	labels.push_back("June");
	labels.push_back("July");
	wxChartsCategoricalData::ptr chartData = wxChartsCategoricalData::make_shared(labels);

	wxVector<wxDouble> points1;
	points1.push_back(3);
	points1.push_back(2.5);
	points1.push_back(1.2);
	points1.push_back(3);
	points1.push_back(6);
	points1.push_back(5);
	points1.push_back(1);
	wxChartsDoubleDataset::ptr dataset1(new wxChartsDoubleDataset("Dataset 1", points1));
	chartData->AddDataset(dataset1);

	this->stat_transactions_chart = new wxColumnChartCtrl(this->panel, wxID_ANY, chartData);
	this->stat_transactions_chart->SetMinSize(wxSize(650, 300));

	wxLogDebug("Panel childs: %d", this->panel->GetChildren().size());

	wxWindowList::iterator first;;
	
//	wxGridBagSizer* gbs = (wxGridBagSizer*)(*first);

	wxStaticBoxSizer* sbz = new wxStaticBoxSizer(wxHORIZONTAL, this->panel, "Transactions chart:");

	sbz->Add(stat_transactions_chart, 0, wxALL | wxEXPAND, 5);
	wxGridBagSizer* gbs = (wxGridBagSizer*) this->panel->GetSizer();

	//gbs->Add(sbz, 0, wxALL | wxEXPAND, 5);
	gbs->Add(sbz, wxGBPosition(0, 1), wxGBSpan(2, 2), wxALL | wxEXPAND, 5);
	gbs->Layout();
}

DashboardPanel::~DashboardPanel()
{

}

void DashboardPanel::UpdateDashboard()
{
	this->PopulateCharts();
}

void DashboardPanel::PopulateCharts()
{

}
