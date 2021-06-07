#include "MainPage.h"

#include "wx/xrc/xmlres.h"

#define XRC_PATH "rc/main.xrc"

MainPage::MainPage(Application* app)
{
	this->app = app;

	if (!wxXmlResource::Get()->Load(XRC_PATH))
		wxLogError("Coudn't load XRC resources !");

	if (!(this->frame = wxXmlResource::Get()->LoadFrame(nullptr,
		"MainFrame")))
		wxLogError("Coudn't load Main Frame from resources !");

	this->student_panel = new StudentsPanel(this);
}

MainPage::~MainPage()
{
	if (wxXmlResource::Get()->Unload(XRC_PATH))
		wxLogError("Entry XRC resources unloaded!");

	delete student_panel;

}

void MainPage::Show()
{
	this->frame->Show();
	this->student_panel->PrepareGrids();
}

void MainPage::StyleGrid(wxGrid* grid, int height )
{
	// Width : auto expand width 
	// Height : parent height - magical number (trial & error)

	if (height == 0)
		height = grid->GetParent()->GetSize().GetHeight() - 30;

	grid->SetSize(
		wxSize(grid->GetSize().GetWidth(),
		height)
	);

	grid->SetColLabelSize(40);
	grid->SetRowLabelSize(30);

	// (Total grid size - scroll bar size - magical number) / columns number
	int col_size = (grid->GetSize().GetWidth() - wxSYS_VSCROLL_X - 19) /
		grid->GetNumberCols();

	for (int i = 0; i < grid->GetNumberCols(); i++)
	{
		grid->SetColSize(i, col_size);
		grid->DisableColResize(i);
	}

	grid->SetMargins(0 - wxSYS_VSCROLL_X - 30, 0);

	grid->EnableEditing(false);
	//grid->DisableRowResize(true);

	grid->SetDefaultCellFont(
		wxFont{ 10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL }
	);
	grid->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
}
