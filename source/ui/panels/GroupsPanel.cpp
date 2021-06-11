#include "../MainPage.h"

#include <wx\xrc\xmlres.h>
#include <wx\numformatter.h>

#define GROUPS_MANAGER this->main->app->GetGroupsManager()
#define PERSONS_MANAGER this->main->app->GetPersonsManager()

GroupsPanel::GroupsPanel(MainPage* main)
{
	this->main = main;

	this->group_name_ctr = XRCCTRL(*(this->main->frame), "group_name_ctr", wxTextCtrl);
	this->group_teacher_per_ctr = XRCCTRL(*(this->main->frame), "group_teacher_per_ctr", wxTextCtrl);
	this->group_teacher_per_ctr->SetValidator(*this->main->prices_validator);
	this->group_school_per_ctr = XRCCTRL(*(this->main->frame), "group_school_per_ctr", wxTextCtrl);
	this->group_school_per_ctr->SetValidator(*this->main->prices_validator);
	this->group_price_ctr = XRCCTRL(*(this->main->frame), "group_price_ctr", wxTextCtrl);
	this->group_price_ctr->SetValidator(*this->main->prices_validator);
	this->group_lessonsperweek_ctr = XRCCTRL(*(this->main->frame), "group_lessonsperweek_ctr", wxTextCtrl);
	this->group_lessonsperweek_ctr->SetValidator(*this->main->prices_validator);

	this->group_teacher_combo = XRCCTRL(*(this->main->frame), "group_teacher_combo", wxComboBox);
	this->group_enroll_student_combo = XRCCTRL(*(this->main->frame), "group_enroll_student_combo", wxComboBox);
	this->group_members_combo = XRCCTRL(*(this->main->frame), "group_members_combo", wxComboBox);

	for (int i = 0; i < 13; i+=2)
	{
		std::string str1 = "group_time_";
		str1 += std::to_string(i);

		this->group_lessons_time[i] = XRCCTRL(*(this->main->frame), str1.c_str(), wxTimePickerCtrl);

		str1 = "group_time_";
		str1 += std::to_string(i+1);
		this->group_lessons_time[i+1] = XRCCTRL(*(this->main->frame), str1.c_str(), wxTimePickerCtrl);

		this->group_lessons_time[i]->SetTime(1, 0, 0);
		this->group_lessons_time[i + 1]->SetTime(1, 0, 0);
	}

	this->groups_grid = XRCCTRL(*(this->main->frame), "groups_grid", wxGrid);
	this->group_members_grid = XRCCTRL(*(this->main->frame), "group_members_grid", wxGrid);

	this->add_btn = XRCCTRL(*(this->main->frame), "add_group_button", wxButton);
	this->edit_btn = XRCCTRL(*(this->main->frame), "edit_group_button", wxButton);
	this->delete_btn = XRCCTRL(*(this->main->frame), "delete_group_button", wxButton);
	this->enroll_btn = XRCCTRL(*(this->main->frame), "group_enroll_student_button", wxButton);
	this->disenroll_btn = XRCCTRL(*(this->main->frame), "group_disenroll_member_button", wxButton);
	this->pay_btn = XRCCTRL(*(this->main->frame), "pay_group_button", wxButton);
	this->collect_btn = XRCCTRL(*(this->main->frame), "collect_group_button", wxButton);

	// Event binding
	this->add_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&GroupsPanel::AddButtonOnClick, this);
	this->edit_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&GroupsPanel::EditButtonOnClick, this);
	this->delete_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&GroupsPanel::DeleteButtonOnClick, this);
	this->enroll_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&GroupsPanel::EnrollButtonOnClick, this);
	this->disenroll_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&GroupsPanel::DisenrollButtonOnClick, this);
	this->pay_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&GroupsPanel::PayButtonOnClick, this);
	this->collect_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&GroupsPanel::CollectButtonOnClick, this);

	this->groups_grid->Bind(wxEVT_GRID_SELECT_CELL,
		&GroupsPanel::OnGroupSelected, this);

	this->group_teacher_combo->Bind(wxEVT_COMBOBOX_DROPDOWN,
		&GroupsPanel::OnTeacherComboBoxDropDown, this);
	this->group_enroll_student_combo->Bind(wxEVT_COMBOBOX_DROPDOWN,
		&GroupsPanel::OnStudentComboBoxDropDown, this);
	this->group_members_combo->Bind(wxEVT_COMBOBOX_DROPDOWN,
		&GroupsPanel::OnMembersComboBoxDropDown, this);
}

GroupsPanel::~GroupsPanel()
{

}

void GroupsPanel::PrepareGrids()
{
	try
	{
		this->groups_list = GROUPS_MANAGER->GetGroups();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Loading groups table",
			wxICON_ERROR | wxOK);
	}

	this->PopulateGroupsTable();
	this->groups_grid->SetColLabelValue(0, "Name");
	this->groups_grid->SetColLabelValue(1, "Teacher");
	this->groups_grid->SetColLabelValue(2, "Price");
	this->groups_grid->SetColLabelValue(3, "School Per");
	MainPage::StyleGrid(this->groups_grid);

	this->group_members_grid->CreateGrid(0, 3);
	this->group_members_grid->SetColLabelValue(0, "First name");
	this->group_members_grid->SetColLabelValue(1, "First name");
	this->group_members_grid->SetColLabelValue(2, "Phone");
	MainPage::StyleGrid(this->group_members_grid);
}

void getIDFromSelectionString(std::string str, unsigned int* id)
{
	size_t pos = str.find('-');
	*id = std::atoi(str.substr(0, pos).c_str());
}

void GroupsPanel::AddButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Add group?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		if (this->group_teacher_combo->GetValue().empty())
			throw std::exception("You must set a teacher !");

		Group g;
		g.name = this->group_name_ctr->GetValue().ToStdString();
		g.price = std::atof(this->group_price_ctr->GetValue());
		g.teacher_percentage = std::atof(this->group_teacher_per_ctr->GetValue());
		g.school_percentage = std::atof(this->group_school_per_ctr->GetValue());
		g.lessons_per_week = std::atoi(this->group_lessonsperweek_ctr->GetValue());
		getIDFromSelectionString(
			this->group_teacher_combo->GetValue().ToStdString(),
			&g.teacher_id
		);
		g.teacher_payed_times = 0;

		for (int i = 0, j = 0; i < 13; i += 2, j++)
		{
			int hour, minute, seconds;
			this->group_lessons_time[i]->GetTime(&hour, &minute, &seconds);

			if (hour == 1)
				continue;

			std::string str = std::to_string(hour);
			str += ":";
			str += std::to_string(minute);
			//selected_group.lessons[j].beginning = str;
			g.lessons[j].beginning = str;

			this->group_lessons_time[i + 1]->GetTime(&hour, &minute, &seconds);
			str = std::to_string(hour);
			str += ":";
			str += std::to_string(minute);
			//selected_group.lessons[j].end = str;
			g.lessons[j].end = str;
		}

		GROUPS_MANAGER->RegisterGroup(&g);

		this->groups_list.push_back(g);
		this->groups_grid->AppendRows(1, false);
		this->SetGroupRow(g, this->groups_grid->GetNumberRows() - 1);

		this->ResetControls();

		wxMessageBox("Group registred!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Group Registering Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void GroupsPanel::EditButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Edit group?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		if (this->group_teacher_combo->GetValue().empty())
			throw std::exception("You must set a teacher !");

		selected_group.name = this->group_name_ctr->GetValue().ToStdString();
		selected_group.price = std::atof(this->group_price_ctr->GetValue());
		selected_group.teacher_percentage = std::atof(this->group_teacher_per_ctr->GetValue());
		selected_group.school_percentage = std::atof(this->group_school_per_ctr->GetValue());
		selected_group.lessons_per_week = std::atoi(this->group_lessonsperweek_ctr->GetValue());
		getIDFromSelectionString(
			this->group_teacher_combo->GetValue().ToStdString(),
			&selected_group.teacher_id
		);

		for (int i = 0, j = 0; i < 13; i += 2, j++)
		{
			int hour, minute, seconds;
			this->group_lessons_time[i]->GetTime(&hour, &minute, &seconds);

			if (hour == 1)
				continue;

			std::string str = std::to_string(hour);
			str += ":";
			str += std::to_string(minute);
			selected_group.lessons[j].beginning = str;

			this->group_lessons_time[i + 1]->GetTime(&hour, &minute, &seconds);
			str = std::to_string(hour);
			str += ":";
			str += std::to_string(minute);
			selected_group.lessons[j].end = str;
		}

		GROUPS_MANAGER->UpdateGroup(this->selected_group);

		// Update teacher in student list
		this->groups_list.at(selected_row) = this->selected_group;
		this->SetGroupRow(this->selected_group, selected_row);

		this->ResetControls();

		wxMessageBox("Group edited!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Group Editing Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void GroupsPanel::DeleteButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Delete group?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		GROUPS_MANAGER->DeleteGroup(
			this->selected_group.id
		);

		this->groups_list.erase(this->groups_list.begin() + (selected_row));
		this->groups_grid->DeleteRows(selected_row);

		this->ResetControls();

		wxMessageBox("Group deleted!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Group Deleting Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void GroupsPanel::EnrollButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Enroll student?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		if (this->group_enroll_student_combo->GetValue().empty())
			throw std::exception("You must select a student !");

		unsigned int person_id;

		getIDFromSelectionString(
			this->group_enroll_student_combo->GetValue().ToStdString(),
			&person_id
		);
		
		GROUPS_MANAGER->EnrollStudent(person_id, this->selected_group.id);

		// Lazy to update the grid 
		this->ResetControls();

		wxMessageBox("Student enrolled!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Student enrolling Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void GroupsPanel::DisenrollButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Disenroll student?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		if (this->group_members_combo->GetValue().empty())
			throw std::exception("You must select a student !");

		unsigned int person_id;

		getIDFromSelectionString(
			this->group_members_combo->GetValue().ToStdString(),
			&person_id
		);

		GROUPS_MANAGER->DisenrollStudent(person_id, this->selected_group.id);

		// Lazy to update the grid 
		this->ResetControls();

		wxMessageBox("Student disenroll!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Student disenrolling Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void GroupsPanel::PayButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Pay the teacher?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;
	try
	{
		GROUPS_MANAGER->PayTeacher(this->selected_group,1);
		this->ResetControls();

		wxMessageBox("Teacher payed!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Teacher paying Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void GroupsPanel::CollectButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Collect from students?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;
	try
	{
		GROUPS_MANAGER->CollectStudentFees(this->selected_group);
		this->ResetControls();

		wxMessageBox("Student's fees collected!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Student fees collecting Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void GroupsPanel::OnGroupSelected(wxGridEvent& e)
{
	this->ResetControls();

	this->selected_group = this->groups_list.at(e.GetRow());
	this->selected_row = e.GetRow();

	std::string str = std::to_string(selected_group.teacher_id);
	str += '-';
	str += groups_grid->GetCellValue({ e.GetRow(), 1 });

	this->group_teacher_combo->SetValue(str);

	this->group_name_ctr->SetValue(selected_group.name);
	this->group_teacher_per_ctr->SetValue(
		wxNumberFormatter::ToString(selected_group.teacher_percentage,3)
	);
	this->group_school_per_ctr->SetValue(
		wxNumberFormatter::ToString(selected_group.school_percentage, 3)
	);
	this->group_price_ctr->SetValue(
		wxNumberFormatter::ToString(selected_group.price, 3)
	);
	this->group_lessonsperweek_ctr->SetValue(
		wxNumberFormatter::ToString((long) selected_group.lessons_per_week)
	);

	for (int i = 0, j = 0; i < 13; i += 2, j++)
	{
		int hour, minute;

		if (selected_group.lessons[j].beginning.empty())
			continue;

		Lesson::getTime(selected_group.lessons[j].beginning, &hour, &minute);
		this->group_lessons_time[i]->SetTime(hour, minute, 0);

		Lesson::getTime(selected_group.lessons[j].end, &hour, &minute);
		this->group_lessons_time[i + 1]->SetTime(hour, minute, 0);
	}

	this->edit_btn->Enable();
	this->delete_btn->Enable();
	this->enroll_btn->Enable();
	this->disenroll_btn->Enable();
	this->pay_btn->Enable();
	this->collect_btn->Enable();

	this->PopulateGroupMembersTable();
}

void GroupsPanel::OnTeacherComboBoxDropDown(wxCommandEvent& e)
{
	this->group_teacher_combo->Clear();

	std::vector<Person> teachers = PERSONS_MANAGER->GetTeachersList();
	std::vector<Person>::iterator i;
	std::vector<std::string> items;
	std::string str;

	for (i = teachers.begin(); i < teachers.end(); i++)
	{
		str = std::to_string((*i).id);
		str += '-';
		str += (*i).first_name;
		str += ' ';
		str += (*i).last_name;

	
		this->group_teacher_combo->Append(str);
	}
}

void GroupsPanel::OnStudentComboBoxDropDown(wxCommandEvent& e)
{
	this->group_enroll_student_combo->Clear();

	std::vector<Person> students = PERSONS_MANAGER->GetStudentsList();
	std::vector<Person>::iterator i;
	std::vector<Person>::iterator m;
	std::vector<std::string> items;
	std::string str;

	for (i = students.begin(); i < students.end(); i++)
	{
		for (m = this->memebers.begin(); m < this->memebers.end(); m++)
		{
			if ((*m).id == (*i).id)
				goto end; // Dirty hack 
		}
		str = std::to_string((*i).id);
		str += '-';
		str += (*i).first_name;
		str += ' ';
		str += (*i).last_name;

		this->group_enroll_student_combo->Append(str);
		
	end:
		continue;
	}
}

void GroupsPanel::OnMembersComboBoxDropDown(wxCommandEvent& e)
{
	this->group_members_combo->Clear();

	std::vector<Person>::iterator m;

	for (m = this->memebers.begin(); m < this->memebers.end(); m++)
	{
		std::string str = std::to_string((*m).id);
		str += '-';
		str += (*m).first_name;
		str += ' ';
		str += (*m).last_name;
		this->group_members_combo->Append(str);
	}
}

void GroupsPanel::SetGroupRow(Group p, int i)
{
	Person pr = GROUPS_MANAGER->GetGroupTeacher(p);

	this->groups_grid->SetCellValue(wxGridCellCoords{ i,0 },
		p.name);
	this->groups_grid->SetCellValue(wxGridCellCoords{ i,1 },
		pr.first_name + " " + pr.last_name);
	this->groups_grid->SetCellValue(wxGridCellCoords{ i,2 },
		wxNumberFormatter::ToString(p.price, 3));
	this->groups_grid->SetCellValue(wxGridCellCoords{ i,3 },
		wxNumberFormatter::ToString(p.school_percentage, 3));
}

void GroupsPanel::SetMemberRow(Person p, int i)
{
	//this->group_members_grid->SetRowLabelValue(i, std::to_string(p.id));
	this->group_members_grid->SetCellValue(wxGridCellCoords{ i,0 },
		p.first_name);
	this->group_members_grid->SetCellValue(wxGridCellCoords{ i,1 },
		p.last_name);
	this->group_members_grid->SetCellValue(wxGridCellCoords{ i,2 },
		p.phone);
}

void GroupsPanel::PopulateGroupsTable()
{
	this->groups_grid->CreateGrid(this->groups_list.size(), 4);

	for (int i = 0; i < this->groups_list.size(); i++)
	{
		this->SetGroupRow(this->groups_list.at(i), i);
	}
}

void GroupsPanel::PopulateGroupMembersTable()
{
	try {
		std::vector<Person> members
			= GROUPS_MANAGER->GetGroupMembers(selected_group.id);
		std::vector<Person>::iterator i;

		int j = 0;

		for (i = members.begin(); i < members.end(); i++, j++)
		{
			this->group_members_grid->AppendRows(1, false);
			this->SetMemberRow((*i), j);
			this->memebers.push_back((*i));
		}
	}
	catch (std::exception e)
	{
		wxMessageBox(e.what(),
			"Loading group's members table",
			wxICON_ERROR | wxOK);
	}
}

void GroupsPanel::ResetControls()
{
	// Reset input
	this->group_name_ctr->SetValue("");
	this->group_teacher_per_ctr->SetValue("");
	this->group_school_per_ctr->SetValue("");
	this->group_price_ctr->SetValue("");
	this->group_lessonsperweek_ctr->SetValue("");

	this->group_teacher_combo->SetSelection(-1);
	this->group_enroll_student_combo->SetSelection(-1);
	this->group_members_combo->SetSelection(-1);
	
	// Reset lessons time to 00:00 AM
	for (int i = 0, j = 0; i < 13; i += 2, j++)
	{
		this->group_lessons_time[i]->SetTime(1, 0, 0);
		this->group_lessons_time[i + 1]->SetTime(1, 0, 0);
	}

	// Disable buttons
	this->edit_btn->Enable(0);
	this->delete_btn->Enable(0);
	this->enroll_btn->Enable(0);
	this->disenroll_btn->Enable(0);
	this->pay_btn->Enable(0);
	this->collect_btn->Enable(0);

	// Reset secondary grids
	if (this->group_members_grid->GetNumberRows() > 0)
	{
		this->group_members_grid->DeleteRows(0, this->group_members_grid->GetNumberRows());
		this->group_members_grid->ForceRefresh();
	}

	// Clear members id vector too
	this->memebers.clear();
}
