#include "SettingsDialog.h"
#include "LoginDialog.h"
#include "RegisterDialog.h"

Database* db;

void SettingsDialog::Init(bool auto_connect)
{
	db = Database::GetDatabaseFromFile();

	if (!db)
	{
		wxMessageBox("Cannot find saved database information !\
		\nPlease enter database's informations and then click save.",
			"Warning", wxICON_INFORMATION | wxOK);
		return;
	}
	
	this->hostname_ctr->SetValue(db->hostname);
	this->username_ctr->SetValue(db->username);
	this->password_ctr->SetValue(db->password);
	this->database_choice->SetSelection(db->type);

	if(auto_connect)
		this->TryConnect();
}

void SettingsDialog::TryConnect()
{
	this->m_button7->Enable(false);

	this->app->SelectUnitOfWork(db->type);

	// Try to connect
	if (!this->app->GetUnitOfWork()->Connect(db))
	{
		wxMessageBox("Error establishing a database connection!\
				\nPlease verify database's informations and then click save.",
			"Connection Error",
			wxICON_ERROR | wxOK);
		this->m_button7->Enable();
		delete db;
		return;
	}

	// Write database to file
	db->WriteDataBaseToFile();

	// Check if database is empty
	if (this->app->GetUnitOfWork()->CheckIfDatabaseEmpty())
	{
		RegisterDialog* register_dialog = new RegisterDialog(this->app);
		register_dialog->Show();
	}
	else {
		LoginDialog* login_dialog = new LoginDialog(this->app);
		login_dialog->Show();
	}

	this->Close();
}

void SettingsDialog::SwapToSettings(wxCommandEvent& event)
{
	db = new Database(
		this->database_choice->GetSelection(),
		this->hostname_ctr->GetValue().ToStdString(),
		this->username_ctr->GetValue().ToStdString(),
		this->password_ctr->GetValue().ToStdString()
	);

	this->TryConnect();

	event.Skip(); 
}

SettingsDialog::SettingsDialog(Application* app, bool auto_connect) 
	: wxDialog(nullptr, wxID_ANY, wxT("Settings"),
	wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{
	this->app = app;

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("")), wxVERTICAL);

	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText20 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Database"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText20->Wrap(-1);
	bSizer33->Add(m_staticText20, 0, wxALIGN_CENTER | wxALL, 5);


	bSizer33->Add(5, 0, 1, wxEXPAND, 5);

	wxString database_choiceChoices[] = { wxT("MYSQL") };
	int database_choiceNChoices = sizeof(database_choiceChoices) / sizeof(wxString);
	database_choice = new wxChoice(sbSizer3->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, database_choiceNChoices, database_choiceChoices, 0);
	database_choice->SetSelection(0);
	bSizer33->Add(database_choice, 10, wxALIGN_CENTER | wxALL, 5);


	sbSizer3->Add(bSizer33, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText5 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Hostname:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText5->Wrap(-1);
	bSizer4->Add(m_staticText5, 0, wxALIGN_CENTER | wxALL, 5);


	bSizer4->Add(0, 0, 1, wxEXPAND, 5);

	hostname_ctr = new wxTextCtrl(sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(hostname_ctr, 0, wxALIGN_CENTER | wxALL, 5);


	sbSizer3->Add(bSizer4, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText6 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Username:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText6->Wrap(-1);
	bSizer5->Add(m_staticText6, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


	bSizer5->Add(0, 0, 1, wxEXPAND, 5);

	username_ctr = new wxTextCtrl(sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(username_ctr, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


	sbSizer3->Add(bSizer5, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText61 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Password:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText61->Wrap(-1);
	bSizer51->Add(m_staticText61, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


	bSizer51->Add(0, 0, 1, wxEXPAND, 5);

	password_ctr = new wxTextCtrl(sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	bSizer51->Add(password_ctr, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


	sbSizer3->Add(bSizer51, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxHORIZONTAL);


	bSizer6->Add(0, 0, 1, wxEXPAND, 5);

	m_button7 = new wxButton(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer6->Add(m_button7, 0, wxALL, 5);


	sbSizer3->Add(bSizer6, 1, wxEXPAND, 5);


	this->SetSizer(sbSizer3);
	this->Layout();
	sbSizer3->Fit(this);

	this->Centre(wxBOTH);

	// Connect Events
	m_button7->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SettingsDialog::SwapToSettings), NULL, this);
	
	this->Init(auto_connect);
}

SettingsDialog::~SettingsDialog()
{
	// Disconnect Events
	m_button7->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SettingsDialog::SwapToSettings), NULL, this);

}