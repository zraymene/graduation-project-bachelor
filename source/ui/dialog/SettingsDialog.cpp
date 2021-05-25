#include "../InitialPage.h"

Database* db;

class IntialPage;

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
	this->save_button->Enable(false);

	this->page->app->SelectUnitOfWork(db->type);

	// Try to connect
	if (!this->page->app->GetUnitOfWork()->Connect(db))
	{
		wxMessageBox("Error establishing a database connection!\
				\nPlease verify database's informations and then click save.",
			"Connection Error",
			wxICON_ERROR | wxOK);

		this->save_button->Enable();

		delete db;

		return;
	}

	// Write database to file
	db->WriteDataBaseToFile();

	// Check if database is empty
	if (this->page->app->GetUnitOfWork()->CheckIfDatabaseEmpty())
	{
		this->page->ShowDialog<RegisterDialog>();
	}
	else {
		this->page->ShowDialog<LoginDialog>();
	}

	this->save_button->Enable();

	this->dialog->Hide();
}

void SettingsDialog::SaveButtonClick(wxCommandEvent& event)
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

void SettingsDialog::Show(bool auto_connect)
{
	if (!InitDialog::Show())
		return;

	if (!wxXmlResource::Get()->LoadDialog(this->dialog, nullptr,
		"SettingsDialog"))
		wxLogError("Coudn't load SettingsDialog from resources !");

	this->database_choice = XRCCTRL(*dialog, "database_choice", wxChoice);
	this->hostname_ctr = XRCCTRL(*dialog, "hostname_ctr", wxTextCtrl);
	this->username_ctr = XRCCTRL(*dialog, "username_ctr", wxTextCtrl);
	this->password_ctr = XRCCTRL(*dialog, "password_ctr", wxTextCtrl);
	this->save_button = XRCCTRL(*dialog, "save_button", wxButton);

	save_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&SettingsDialog::SaveButtonClick, this);

	this->Init(auto_connect);

	dialog->Show();

}

SettingsDialog::~SettingsDialog()
{
	delete database_choice,
		hostname_ctr,
		username_ctr,
		password_ctr,
		save_button;
}