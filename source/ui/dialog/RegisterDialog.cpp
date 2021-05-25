#include "../InitialPage.h"

void RegisterDialog::Register(wxCommandEvent& event)
{
	wxLogDebug("REGISTERS - SHEEESH");
	event.Skip();
}

void RegisterDialog::SwapToSettings(wxCommandEvent& event)
{
	this->dialog->Hide();

	this->page->ShowDialog<SettingsDialog>();

	event.Skip();
}

void RegisterDialog::Show()
{
	if (!InitDialog::Show())
		return;

	if (!wxXmlResource::Get()->LoadDialog(this->dialog, nullptr,
		"RegisterDialog"))
		wxLogError("Coudn't load RegisterDialog from resources !");

	this->username_ctr = XRCCTRL(*dialog, "username_ctr", wxTextCtrl);
	this->password_ctr = XRCCTRL(*dialog, "password_ctr", wxTextCtrl);
	this->register_button = XRCCTRL(*dialog, "register_button", wxButton);
	this->settings_button = XRCCTRL(*dialog, "settings_button", wxButton);

	register_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&RegisterDialog::Register, this);
	settings_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&RegisterDialog::SwapToSettings, this);

	dialog->Show();
}

RegisterDialog::~RegisterDialog()
{
	delete register_button,
		settings_button,
		username_ctr,
		password_ctr;
}
