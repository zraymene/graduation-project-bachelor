#include "../MainPage.h"

#include <wx\xrc\xmlres.h>

void StudentsPanel::AddButtonOnClick(wxCommandEvent& event)
{
}

void StudentsPanel::EditButtonOnClick(wxCommandEvent& event)
{
}

void StudentsPanel::DeleteButtonOnClick(wxCommandEvent& event)
{
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

}

StudentsPanel::~StudentsPanel()
{
}


void StudentsPanel::PopulateStudentsTable()
{
	for (int i = 0; i < this->student_list.size(); i++)
	{
		this->students_grid->SetCellValue(wxGridCellCoords{ i,0 },
			this->student_list.at(i).first_name);
		this->students_grid->SetCellValue(wxGridCellCoords{ i,1 },
			this->student_list.at(i).last_name);
		this->students_grid->SetCellValue(wxGridCellCoords{ i,2 },
			this->student_list.at(i).phone);
	}
}

void StudentsPanel::PrepareGrids()
{
	this->student_list = this->main->app->GetPersonsManager()->GetStudentsList();
	this->students_grid->CreateGrid(this->student_list.size(), 3);
	this->students_grid->SetColLabelValue(0, "Last name");
	this->students_grid->SetColLabelValue(1, "First name");
	this->students_grid->SetColLabelValue(2, "Phone");
	MainPage::StyleGrid(this->students_grid);
	this->PopulateStudentsTable();


	this->student_group_grid->CreateGrid(0, 2);
	this->student_group_grid->SetColLabelValue(0, "Group ID");
	this->student_group_grid->SetColLabelValue(1, "Group Name");
	MainPage::StyleGrid(this->student_group_grid);

	this->student_absence_grid->CreateGrid(0, 3);
	this->student_absence_grid->SetColLabelValue(0, "Group ID");
	this->student_absence_grid->SetColLabelValue(1, "Group Name");
	this->student_absence_grid->SetColLabelValue(2, "Date");
	MainPage::StyleGrid(this->student_absence_grid);


}
