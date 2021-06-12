#include "../InitialPage.h"

void RegisterDialog::Register(wxCommandEvent& event)
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

	try {

		this->page->app->GetUsersManage()->RegisterUser({
			this->username_ctr->GetValue().ToStdString(),
			this->password_ctr->GetValue().ToStdString(),
			});

		wxMessageBox("User registered !\nYou are logged in now.",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();

		this->page->entry->SwitchToMainPage();
	}
	catch (std::runtime_error e)
	{
		wxMessageBox(e.what(),
			"Registeration Error",
			wxICON_ERROR | wxOK);
		event.Skip();
	}
}

void RegisterDialog::SwapToSettings(wxCommandEvent& event)
{
	this->dialog.Hide();

	this->page->ShowDialog<SettingsDialog>();

	event.Skip();
}

void RegisterDialog::Show()
{
	if (!InitDialog::Show())
		return;

	if (!wxXmlResource::Get()->LoadDialog(&this->dialog, nullptr,
		"RegisterDialog"))
		wxLogError("Coudn't load RegisterDialog from resources !");

	this->dialog.Bind(wxEVT_CLOSE_WINDOW,
		&RegisterDialog::OnClose, this);

	this->username_ctr = XRCCTRL(dialog, "username_ctr", wxTextCtrl);
	this->password_ctr = XRCCTRL(dialog, "password_ctr", wxTextCtrl);
	this->register_button = XRCCTRL(dialog, "register_button", wxButton);
	this->settings_button = XRCCTRL(dialog, "settings_button", wxButton);

	register_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&RegisterDialog::Register, this);
	settings_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&RegisterDialog::SwapToSettings, this);

	dialog.Show();
}

RegisterDialog::~RegisterDialog()
{
	delete register_button,
		settings_button,
		username_ctr,
		password_ctr;
}
