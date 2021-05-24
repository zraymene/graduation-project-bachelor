#pragma once

#include "wx/wx.h"
#include "../../core/Application.h"

class RegisterDialog : public wxDialog
{
private:

protected:
	wxStaticText* m_staticText5;
	wxTextCtrl* username_ctr;
	wxStaticText* m_staticText6;
	wxTextCtrl* password_ctr;
	wxButton* m_button5;
	wxButton* m_button7;

	// Virtual event handlers, overide them in your derived class
	void Register(wxCommandEvent& event);
	void SwapToSettings(wxCommandEvent& event);

	Application* app;
public:

	RegisterDialog(Application* app);
	~RegisterDialog();

};
