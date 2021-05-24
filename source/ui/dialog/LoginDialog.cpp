#include "LoginDialog.h"
#include "SettingsDialog.h"

void LoginDialog::Login(wxCommandEvent& event)
{
	wxLogDebug("SHEESH");
	event.Skip();
}

void LoginDialog::SwapToSettings(wxCommandEvent& event)
{
	SettingsDialog* settingd_dialog = new SettingsDialog(this->app, false);
	settingd_dialog->Show();

	this->Close();
	event.Skip();
}

LoginDialog::LoginDialog(Application * app) : wxDialog(nullptr, wxID_ANY, wxT("Login"),
	wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{
	this->app = app;

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxEmptyString), wxVERTICAL);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText5 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Username:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText5->Wrap(-1);
	bSizer4->Add(m_staticText5, 0, wxALIGN_CENTER | wxALL, 5);


	bSizer4->Add(0, 0, 1, wxEXPAND, 5);

	username_ctr = new wxTextCtrl(sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(username_ctr, 0, wxALIGN_CENTER | wxALL, 5);


	sbSizer3->Add(bSizer4, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText6 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Password:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText6->Wrap(-1);
	bSizer5->Add(m_staticText6, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


	bSizer5->Add(0, 0, 1, wxEXPAND, 5);

	password_ctr = new wxTextCtrl(sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	bSizer5->Add(password_ctr, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


	sbSizer3->Add(bSizer5, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxHORIZONTAL);


	bSizer6->Add(0, 0, 1, wxEXPAND, 5);

	m_button5 = new wxButton(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Login"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer6->Add(m_button5, 0, wxALL, 5);

	m_button7 = new wxButton(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Settings"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer6->Add(m_button7, 0, wxALL, 5);


	sbSizer3->Add(bSizer6, 1, wxEXPAND, 5);


	this->SetSizer(sbSizer3);
	this->Layout();
	sbSizer3->Fit(this);

	this->Centre(wxBOTH);

	// Connect Events
	m_button5->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LoginDialog::Login), NULL, this);
	m_button7->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LoginDialog::SwapToSettings), NULL, this);
}

LoginDialog::~LoginDialog()
{
	// Disconnect Events
	m_button5->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LoginDialog::Login), NULL, this);
	m_button7->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LoginDialog::SwapToSettings), NULL, this);

}
