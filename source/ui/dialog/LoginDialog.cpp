#include "../InitialPage.h"


void LoginDialog::Login(wxCommandEvent& event)
{
	// Username musn't be empty
	if (this->username_ctr->IsEmpty())
	{
		wxMessageBox("Username field is empty!",
			"Input error",
			wxICON_ERROR | wxOK);
		event.Skip();
		return;
	}

	if (!this->page->app->GetUsersManage()->Login({
		this->username_ctr->GetValue().ToStdString(),
		this->password_ctr->GetValue().ToStdString(),
		}))
	{
		wxMessageBox("Error while logging in !",
			"Login error",
			wxICON_ERROR | wxOK);
		event.Skip();
		return;
	}

	wxMessageBox("You are loged in now !",
		"Success", wxICON_INFORMATION | wxOK);
	return;

	// Swap to dashboard page
	//
	event.Skip();
}

void LoginDialog::SwapToSettings(wxCommandEvent& event)
{
	this->dialog->Hide();

	this->page->ShowDialog<SettingsDialog>();

	event.Skip();
}


void LoginDialog::Show()
{
	if (!InitDialog::Show())
		return;

	if (!wxXmlResource::Get()->LoadDialog(this->dialog, nullptr,
		"LoginDialog"))
		wxLogError("Coudn't load LoginDialog from resources !");

	this->username_ctr = XRCCTRL(*dialog, "username_ctr", wxTextCtrl);
	this->password_ctr = XRCCTRL(*dialog, "password_ctr", wxTextCtrl);
	this->login_button = XRCCTRL(*dialog, "login_button", wxButton);
	this->settings_button = XRCCTRL(*dialog, "settings_button", wxButton);

	login_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&LoginDialog::Login, this);
	settings_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&LoginDialog::SwapToSettings, this);

	dialog->Show();
}

LoginDialog::~LoginDialog()
{
	delete
		username_ctr,
		password_ctr,
		login_button,
		settings_button;
}
