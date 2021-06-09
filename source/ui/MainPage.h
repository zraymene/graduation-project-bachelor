#pragma once

#include "wx/wx.h"
#include "wx/grid.h"
#include "../core/Application.h"

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

	wxPanel* panel;

	std::vector<Person> student_list;
	Person selected_student;
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

	static void StyleGrid(wxGrid* grid, int height = 0);

private:
	StudentsPanel* student_panel = nullptr;
};

