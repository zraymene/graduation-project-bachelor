#pragma once

#include "wx/wx.h"
#include "wx/grid.h"
#include "../core/Application.h"
#include "wx/notebook.h"
#include "wx/timectrl.h"
#include "wx/charts/wxcolumnchartctrl.h"
#include "wx/gbsizer.h"

class MainPage;

class StudentsPanel{
public:
	StudentsPanel(MainPage* main);
	~StudentsPanel();

	void PrepareGrids();

private:

	void AddButtonOnClick(wxCommandEvent& event);
	void EditButtonOnClick(wxCommandEvent& event);
	void DeleteButtonOnClick(wxCommandEvent& event);
	
	void OnStudentSelected(wxGridEvent& e);

	void SetStudentRow(Person p, int i);
	void PopulateStudentsTable();
	void PopulateStudentGroups();
	void PopulateStudentAbsence();
	void ResetControls();

	wxTextCtrl* student_fname_ctr;
	wxTextCtrl* student_lname_ctr;
	wxTextCtrl* student_phone_ctr;

	wxGrid* students_grid;
	wxGrid* student_group_grid;
	wxGrid* student_absence_grid;

	wxButton* add_btn;
	wxButton* edit_btn;
	wxButton* delete_btn;

	std::vector<Person> student_list;
	Person selected_student;
	int selected_row;

	MainPage* main;
};

class TeachersPanel {
public:
	TeachersPanel(MainPage* main);
	~TeachersPanel();

	void PrepareGrids();

private:

	void AddButtonOnClick(wxCommandEvent& event);
	void EditButtonOnClick(wxCommandEvent& event);
	void DeleteButtonOnClick(wxCommandEvent& event);
	void OnTeacherSelected(wxGridEvent& e);

	void SetTeacherRow(Person p, int i);
	void PopulateTeachersTable();
	void PopulateTeacherGroups();
	void ResetControls();

	wxTextCtrl* teacher_fname_ctr;
	wxTextCtrl* teacher_lname_ctr;
	wxTextCtrl* teacher_phone_ctr;

	wxGrid* teachers_grid;
	wxGrid* teacher_groups_grid;

	wxButton* add_btn;
	wxButton* edit_btn;
	wxButton* delete_btn;

	std::vector<Person> teachers_list;
	Person selected_teacher;
	int selected_row;

	MainPage* main;
};

class GroupsPanel {
public:
	GroupsPanel(MainPage* main);
	~GroupsPanel();

	void PrepareGrids();

private:

	void AddButtonOnClick(wxCommandEvent& event);
	void EditButtonOnClick(wxCommandEvent& event);
	void DeleteButtonOnClick(wxCommandEvent& event);
	void EnrollButtonOnClick(wxCommandEvent& event);
	void DisenrollButtonOnClick(wxCommandEvent& event);
	void PayButtonOnClick(wxCommandEvent& event);
	void CollectButtonOnClick(wxCommandEvent& event);
	void OnGroupSelected(wxGridEvent& e);
	void OnTeacherComboBoxDropDown(wxCommandEvent& e);
	void OnStudentComboBoxDropDown(wxCommandEvent& e);
	void OnMembersComboBoxDropDown(wxCommandEvent& e);

	void SetGroupRow(Group p, int i);
	void SetMemberRow(Person p, int i);
	void PopulateGroupsTable();
	void PopulateGroupMembersTable();
	void ResetControls();

	wxTextCtrl* group_name_ctr;
	wxTextCtrl* group_teacher_per_ctr;
	wxTextCtrl* group_school_per_ctr;
	wxTextCtrl* group_price_ctr;
	wxTextCtrl* group_lessonsperweek_ctr;

	wxComboBox* group_teacher_combo;
	wxComboBox* group_enroll_student_combo;
	wxComboBox* group_members_combo;
	
	// 7/Week
	// First : start time of the lesson
	// Second: end time of the lesson
	wxTimePickerCtrl* group_lessons_time[14];

	wxGrid* groups_grid;
	wxGrid* group_members_grid;

	wxButton* add_btn;
	wxButton* edit_btn;
	wxButton* delete_btn;
	wxButton* pay_btn;
	wxButton* collect_btn;
	wxButton* enroll_btn;
	wxButton* disenroll_btn;

	std::vector<Person> memebers;
	std::vector<Group> groups_list;
	Group selected_group;
	int selected_row;

	MainPage* main;
};

class TransactionsPanel {
public:
	TransactionsPanel(MainPage* main);
	~TransactionsPanel();

	void PrepareGrids();
	void PopulateTransactionsTable();

private:

	void AddButtonOnClick(wxCommandEvent& event);
	void EditButtonOnClick(wxCommandEvent& event);
	void DeleteButtonOnClick(wxCommandEvent& event);

	void OnTransactionSelected(wxGridEvent& e);

	void SetTransactionRow(Transaction p, int i);
	void ResetControls();

	wxTextCtrl* transaction_name_ctr;
	wxTextCtrl* transaction_amount_ctr;

	wxGrid* transactions_grid;

	wxButton* add_btn;
	wxButton* edit_btn;
	wxButton* delete_btn;

	std::vector<Transaction> transaction_list;
	Transaction selected_transaction;
	int selected_row;

	MainPage* main;
};

class DashboardPanel {
public:
	DashboardPanel(MainPage* main);
	~DashboardPanel();

	void UpdateDashboard();

private:

	void PopulateCharts();

	wxTextCtrl* stat_teachers_count_ctr;
	wxTextCtrl* stat_students_count_ctr;
	wxTextCtrl* stat_groups_count_ctr;
	wxTextCtrl* stat_total_expenses_ctr;
	wxTextCtrl* stat_total_profits_ctr;
	wxTextCtrl* stat_net_profit_ctr;

	wxStaticText* dash_tmp_label;
	wxStaticText* dash_parent_tmp;

	wxColumnChartCtrl* stat_transactions_chart;
	wxStaticBoxSizer* stat_trans_chart_sizer;

	wxPanel* panel;

	MainPage* main;
};

class MainPage
{
public:

	MainPage(Application* app);
	~MainPage();

	void Show();

	Application* app;
	wxFrame* frame = nullptr;
	wxNotebook* notebook;
	wxTextValidator *prices_validator;

	static void StyleGrid(wxGrid* grid, int height = 0);

private:
	
	void OnNotePageChanged(wxBookCtrlEvent& e);
	void PrepareNotebookHeader();

	StudentsPanel* students_panel = nullptr;
	TeachersPanel* teachers_panel = nullptr;
	GroupsPanel* groups_panel = nullptr;
	TransactionsPanel* transactions_panel = nullptr;
	DashboardPanel* dashboard_panel = nullptr;
};

