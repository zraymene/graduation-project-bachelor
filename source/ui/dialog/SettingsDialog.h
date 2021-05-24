#pragma once

#include "wx/wx.h"
#include "../../core/Application.h"

class SettingsDialog : public wxDialog
{
private:
	void Init(bool auto_connect);
	void TryConnect();

protected:
	wxStaticText* m_staticText20;
	wxChoice* database_choice;
	wxStaticText* m_staticText5;
	wxTextCtrl* hostname_ctr;
	wxStaticText* m_staticText6;
	wxTextCtrl* username_ctr;
	wxStaticText* m_staticText61;
	wxTextCtrl* password_ctr;
	wxButton* m_button7;

	// Virtual event handlers, overide them in your derived class
	void SwapToSettings(wxCommandEvent& event);
	
	Application* app;
public:

	SettingsDialog(Application* app, bool auto_connect = true);
	~SettingsDialog();

};

