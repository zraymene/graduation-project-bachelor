#include "../MainPage.h"

#include <wx\xrc\xmlres.h>
#include <wx/numformatter.h>

#define PERSON_MANAGER this->main->app->GetPersonsManager()

void StudentsPanel::AddButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Add student?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {
		Person p = { 0,
			  this->student_fname_ctr->GetValue().ToStdString(),
			  this->student_lname_ctr->GetValue().ToStdString(),
			  this->student_phone_ctr->GetValue().ToStdString() };

		PERSON_MANAGER->RegisterStudent(&p
		);

		this->student_list.push_back(p);
		this->students_grid->AppendRows(1, false);
		this->SetStudentRow(p, this->students_grid->GetNumberRows()-1);

		this->ResetControls();

		wxMessageBox("Student registred!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Student Registering Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void StudentsPanel::EditButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Edit student?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		this->selected_student.first_name =
			this->student_fname_ctr->GetValue().ToStdString();
			this->selected_student.last_name =
			this->student_lname_ctr->GetValue().ToStdString();
			this->selected_student.phone =
			this->student_phone_ctr->GetValue().ToStdString();

		PERSON_MANAGER->UpdateStudent(
			this->selected_student
		);

		// Update student in student list
		this->student_list.at(selected_row) = this->selected_student;
		this->SetStudentRow(this->selected_student, selected_row);
		
		this->ResetControls();

		wxMessageBox("Student edited!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Student Editing Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void StudentsPanel::DeleteButtonOnClick(wxCommandEvent& event)
{

	int answer = wxMessageBox("Delete student?", "Confirm",
		                           wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		PERSON_MANAGER->DeleteStudent(
			this->selected_student.id
		);

		this->student_list.erase(this->student_list.begin() + (selected_row));
		this->students_grid->DeleteRows(selected_row);
		
		this->ResetControls();

		wxMessageBox("Student deleted!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Student Deleting Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void StudentsPanel::OnStudentSelected(wxGridEvent& e)
{
	this->selected_student = this->student_list.at(e.GetRow());
	this->selected_row = e.GetRow();

	this->student_fname_ctr->SetValue(selected_student.first_name);
	this->student_lname_ctr->SetValue(selected_student.last_name);
	this->student_phone_ctr->SetValue(selected_student.phone);

	this->edit_btn->Enable();
	this->delete_btn->Enable();

	this->PopulateStudentGroups();
	this->PopulateStudentAbsence();
}

StudentsPanel::StudentsPanel(MainPage* main)
{
	this->main = main;

	this->student_fname_ctr = XRCCTRL(*(this->main->frame), "student_fname_ctr", wxTextCtrl);
	this->student_lname_ctr = XRCCTRL(*(this->main->frame), "student_lname_ctr", wxTextCtrl);
	this->student_phone_ctr = XRCCTRL(*(this->main->frame), "student_phone_ctr", wxTextCtrl);

	this->students_grid = XRCCTRL(*(this->main->frame), "students_grid", wxGrid);
	this->student_group_grid = XRCCTRL(*(this->main->frame), "student_group_grid", wxGrid);
	this->student_absence_grid = XRCCTRL(*(this->main->frame), "student_absence_grid", wxGrid);

	this->add_btn = XRCCTRL(*(this->main->frame), "add_student_button", wxButton);
	this->edit_btn = XRCCTRL(*(this->main->frame), "edit_student_button", wxButton);
	this->delete_btn = XRCCTRL(*(this->main->frame), "delete_student_button", wxButton);

	// Event binding
	this->add_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&StudentsPanel::AddButtonOnClick, this);
	this->edit_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&StudentsPanel::EditButtonOnClick, this);
	this->delete_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&StudentsPanel::DeleteButtonOnClick , this);

	this->students_grid->Bind(wxEVT_GRID_SELECT_CELL,
		&StudentsPanel::OnStudentSelected, this);
}

StudentsPanel::~StudentsPanel()
{
}

void StudentsPanel::SetStudentRow(Person p, int i)
{
	this->students_grid->SetCellValue(wxGridCellCoords{ i,0 },
		p.first_name);
	this->students_grid->SetCellValue(wxGridCellCoords{ i,1 },
		p.last_name);
	this->students_grid->SetCellValue(wxGridCellCoords{ i,2 },
		p.phone);
}

void StudentsPanel::PopulateStudentsTable()
{
	this->students_grid->CreateGrid(this->student_list.size(), 3);

	for (int i = 0; i < this->student_list.size(); i++)
	{
		this->SetStudentRow(this->student_list.at(i), i);
	}
}

void StudentsPanel::PopulateStudentGroups()
{
	try {
		std::vector<Group> groups
			= PERSON_MANAGER->GetStudentGroupsEnrolledIn(selected_student.id);

		for (int i = 0; i < groups.size(); i++)
		{
			this->student_group_grid->AppendRows(1, false);
			this->student_group_grid->SetCellValue(wxGridCellCoords{ i,0 },
				groups.at(i).name);
			this->student_group_grid->SetCellValue(wxGridCellCoords{ i,1 },
				wxNumberFormatter::ToString(groups.at(i).price, 2));
		}
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Loading student's groups table",
			wxICON_ERROR | wxOK);
	}

}

void StudentsPanel::PopulateStudentAbsence()
{
	try {
		std::vector<StudentAbsense> groups
			= PERSON_MANAGER->GetStudentAbsense(selected_student.id);

		for (int i = 0; i < groups.size(); i++)
		{
			this->student_absence_grid->AppendRows(1, false);
			this->student_absence_grid->SetCellValue(wxGridCellCoords{ i,0 },
				groups.at(i).group.name);
			this->student_absence_grid->SetCellValue(wxGridCellCoords{ i,1 },
				groups.at(i).absense.date);
		}
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Loading student's absence table",
			wxICON_ERROR | wxOK);
	}
}

void StudentsPanel::ResetControls()
{
	// Reset input
	this->student_fname_ctr->SetValue("");
	this->student_lname_ctr->SetValue("");
	this->student_phone_ctr->SetValue("");

	// Disable buttons
	this->edit_btn->Enable(0);
	this->delete_btn->Enable(0);

	// Reset secondary grids
	if (this->student_group_grid->GetNumberRows() > 1)
	{
		this->student_group_grid->DeleteRows(0, this->student_group_grid->GetNumberRows());
		this->student_group_grid->ForceRefresh();
	}

	if (this->student_absence_grid->GetNumberRows() > 1)
	{
		this->student_absence_grid->DeleteRows(0, this->student_absence_grid->GetNumberRows());
		this->student_absence_grid->ForceRefresh();
	}
}

void StudentsPanel::PrepareGrids()
{
	try
	{
		this->student_list = PERSON_MANAGER->GetStudentsList();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Loading students table",
			wxICON_ERROR | wxOK);
	}

	this->PopulateStudentsTable();
	this->students_grid->SetColLabelValue(0, "Last name");
	this->students_grid->SetColLabelValue(1, "First name");
	this->students_grid->SetColLabelValue(2, "Phone");
	MainPage::StyleGrid(this->students_grid);
	

	this->student_group_grid->CreateGrid(0, 2);
	this->student_group_grid->SetColLabelValue(0, "Group's Name");
	this->student_group_grid->SetColLabelValue(1, "Teacher");
	MainPage::StyleGrid(this->student_group_grid);

	this->student_absence_grid->CreateGrid(0, 2);
	this->student_absence_grid->SetColLabelValue(0, "Group's Name");
	this->student_absence_grid->SetColLabelValue(1, "Date");
	MainPage::StyleGrid(this->student_absence_grid);


}
