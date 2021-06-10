#include "../MainPage.h"

#include <wx\xrc\xmlres.h>
#include <wx/numformatter.h>

#define PERSON_MANAGER this->main->app->GetPersonsManager()

TeachersPanel::TeachersPanel(MainPage* main)
{
	this->main = main;

	this->teacher_fname_ctr = XRCCTRL(*(this->main->frame), "teacher_fname_ctr", wxTextCtrl);
	this->teacher_lname_ctr = XRCCTRL(*(this->main->frame), "teacher_lname_ctr", wxTextCtrl);
	this->teacher_phone_ctr = XRCCTRL(*(this->main->frame), "teacher_phone_ctr", wxTextCtrl);

	this->teachers_grid = XRCCTRL(*(this->main->frame), "teachers_grid", wxGrid);
	this->teacher_groups_grid = XRCCTRL(*(this->main->frame), "teacher_groups_grid", wxGrid);

	this->add_btn = XRCCTRL(*(this->main->frame), "add_teacher_button", wxButton);
	this->edit_btn = XRCCTRL(*(this->main->frame), "edit_teacher_button", wxButton);
	this->delete_btn = XRCCTRL(*(this->main->frame), "delete_teacher_button", wxButton);

	// Event binding
	this->add_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&TeachersPanel::AddButtonOnClick, this);
	this->edit_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&TeachersPanel::EditButtonOnClick, this);
	this->delete_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&TeachersPanel::DeleteButtonOnClick, this);

	this->teachers_grid->Bind(wxEVT_GRID_SELECT_CELL,
		&TeachersPanel::OnTeacherSelected, this);
}

TeachersPanel::~TeachersPanel()
{

}

void TeachersPanel::PrepareGrids()
{
	try
	{
		this->teachers_list = PERSON_MANAGER->GetTeachersList();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Loading teachers table",
			wxICON_ERROR | wxOK);
	}

	this->PopulateTeachersTable();
	this->teachers_grid->SetColLabelValue(0, "Last name");
	this->teachers_grid->SetColLabelValue(1, "First name");
	this->teachers_grid->SetColLabelValue(2, "Phone");
	MainPage::StyleGrid(this->teachers_grid);


	this->teacher_groups_grid->CreateGrid(0, 3);
	this->teacher_groups_grid->SetColLabelValue(0, "Group's Name");
	this->teacher_groups_grid->SetColLabelValue(1, "Price");
	this->teacher_groups_grid->SetColLabelValue(2, "Percentage");
	MainPage::StyleGrid(this->teacher_groups_grid);

}

void TeachersPanel::AddButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Add teacher?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {
		Person p = { 0,
			  this->teacher_fname_ctr->GetValue().ToStdString(),
			  this->teacher_lname_ctr->GetValue().ToStdString(),
			  this->teacher_phone_ctr->GetValue().ToStdString() };

		PERSON_MANAGER->RegisterTeacher(&p
		);

		this->teachers_list.push_back(p);
		this->teachers_grid->AppendRows(1, false);
		this->SetTeacherRow(p, this->teachers_grid->GetNumberRows() - 1);

		this->ResetControls();

		wxMessageBox("Teacher registred!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Teacher Registering Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void TeachersPanel::EditButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Edit teacher?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		this->selected_teacher.first_name =
			this->teacher_fname_ctr->GetValue().ToStdString();
		this->selected_teacher.last_name =
			this->teacher_lname_ctr->GetValue().ToStdString();
		this->selected_teacher.phone =
			this->teacher_phone_ctr->GetValue().ToStdString();

		PERSON_MANAGER->UpdateTeacher(
			this->selected_teacher
		);

		// Update teacher in student list
		this->teachers_list.at(selected_row) = this->selected_teacher;
		this->SetTeacherRow(this->selected_teacher, selected_row);

		this->ResetControls();

		wxMessageBox("Teacher edited!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Teacher Editing Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void TeachersPanel::DeleteButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Delete teacher?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		PERSON_MANAGER->DeleteStudent(
			this->selected_teacher.id
		);

		this->teachers_list.erase(this->teachers_list.begin() + (selected_row));
		this->teachers_grid->DeleteRows(selected_row);

		this->ResetControls();

		wxMessageBox("Teacher deleted!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Teacher Deleting Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void TeachersPanel::OnTeacherSelected(wxGridEvent& e)
{
	this->ResetControls();

	this->selected_teacher = this->teachers_list.at(e.GetRow());
	this->selected_row = e.GetRow();

	this->teacher_fname_ctr->SetValue(selected_teacher.first_name);
	this->teacher_lname_ctr->SetValue(selected_teacher.last_name);
	this->teacher_phone_ctr->SetValue(selected_teacher.phone);

	this->edit_btn->Enable();
	this->delete_btn->Enable();

	this->PopulateTeacherGroups();
}

void TeachersPanel::SetTeacherRow(Person p, int i)
{
	this->teachers_grid->SetCellValue(wxGridCellCoords{ i,0 },
		p.first_name);
	this->teachers_grid->SetCellValue(wxGridCellCoords{ i,1 },
		p.last_name);
	this->teachers_grid->SetCellValue(wxGridCellCoords{ i,2 },
		p.phone);
}

void TeachersPanel::PopulateTeachersTable()
{
	this->teachers_grid->CreateGrid(this->teachers_list.size(), 3);
	std::vector<Person>::iterator iter;
	int j = 0;

	for (iter = this->teachers_list.begin(); iter < this->teachers_list.end(); iter++, j++)
	{
		this->SetTeacherRow((*iter), j);
	}
}

void TeachersPanel::PopulateTeacherGroups()
{
	try {
		std::vector<Group> groups
			= PERSON_MANAGER->GetTeacherGroups(selected_teacher.id);

		for (int i = 0; i < groups.size(); i++)
		{
			this->teacher_groups_grid->AppendRows(1, false);
			this->teacher_groups_grid->SetCellValue(wxGridCellCoords{ i,0 },
				groups.at(i).name);
			this->teacher_groups_grid->SetCellValue(wxGridCellCoords{ i,1 },
				wxNumberFormatter::ToString(groups.at(i).price, 2));
			this->teacher_groups_grid->SetCellValue(wxGridCellCoords{ i,2 },
				wxNumberFormatter::ToString(groups.at(i).teacher_percentage, 3));
		}
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Loading teacher's groups table",
			wxICON_ERROR | wxOK);
	}
}

void TeachersPanel::ResetControls()
{
	// Reset input
	this->teacher_fname_ctr->SetValue("");
	this->teacher_lname_ctr->SetValue("");
	this->teacher_phone_ctr->SetValue("");

	// Disable buttons
	this->edit_btn->Enable(0);
	this->delete_btn->Enable(0);

	// Reset secondary grids
	if (this->teacher_groups_grid->GetNumberRows() > 0)
	{
		this->teacher_groups_grid->DeleteRows(0, this->teacher_groups_grid->GetNumberRows());
		this->teacher_groups_grid->ForceRefresh();
	}
}
