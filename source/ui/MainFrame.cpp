#include "MainFrame.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_BUTTON(1001, MainFrame::OnButttonClicked)
wxEND_EVENT_TABLE();


MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "TITLE",
						wxDefaultPosition, wxSize(800,600))
{
	button = new wxButton(this, 1001, "Click Me!");
	text_area = new wxTextCtrl(this, wxID_ANY, "");
	list_box = new wxListBox(this, 1002, wxDefaultPosition, wxSize(300, 150));

	hbox = new wxBoxSizer(wxVERTICAL);

	hbox->Add(button, 1, wxEXPAND);
	hbox->Add(text_area,1, wxEXPAND);
	hbox->Add(list_box, 1, wxEXPAND);

	hbox->Layout();
	this->SetSizer(hbox);
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnButttonClicked(wxCommandEvent& evt)
{
	list_box->Append(text_area->GetValue());
	evt.Skip();
}
