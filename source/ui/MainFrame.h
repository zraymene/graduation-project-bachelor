#pragma once

#include "wx/wx.h"

class MainFrame : public wxFrame
{
public:
	MainFrame();
	~MainFrame();

private:
	wxButton *button = nullptr;
	wxTextCtrl *text_area = nullptr;
	wxListBox* list_box = nullptr;

	wxBoxSizer* hbox = nullptr;

	void OnButttonClicked(wxCommandEvent& evt);
	
	wxDECLARE_EVENT_TABLE();
};

