#pragma once

#include "wx/wx.h"
#include "wx/grid.h"
#include "../core/Application.h"
#include "wx/notebook.h"

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
	void OnGroupSelected(wxGridEvent& e);

	void SetGroupRow(Group p, int i);
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
	
	// 7/Week
	// First : start time of the lesson
	// Second: end time of the lesson
	wxSpinCtrl* group_lessons_time[14];

	wxGrid* groups_grid;
	wxGrid* group_members_grid;

	wxButton* add_btn;
	wxButton* edit_btn;
	wxButton* delete_btn;
	wxButton* enroll_btn;

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

private:

	void AddButtonOnClick(wxCommandEvent& event);
	void EditButtonOnClick(wxCommandEvent& event);
	void DeleteButtonOnClick(wxCommandEvent& event);

	void OnTransactionSelected(wxGridEvent& e);

	void SetTransactionRow(Transaction p, int i);
	void PopulateTransactionsTable();
	void ResetControls();

	wxTextCtrl* transaction_name_ctr;
	wxTextCtrl* transaction_amount_ctr;

	wxGrid* transactions_grid;

	wxButton* add_btn;
	wxButton* edit_btn;
	wxButton* delete_btn;

	std::vector<Transaction> transaction_list;
	Person selected_transaction;
	int selected_row;

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

	static void StyleGrid(wxGrid* grid, int height = 0);

private:
	
	void PrepareNotebookHeader();

	StudentsPanel* students_panel = nullptr;
	TeachersPanel* teachers_panel = nullptr;
};

