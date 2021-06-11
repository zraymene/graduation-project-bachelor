#include "MainPage.h"

#include "wx/xrc/xmlres.h"
#include "wx/app.h"
#define XRC_PATH "rc/main.xrc"

MainPage::MainPage(Entry* entry)
{
	this->entry = entry;
	this->app = entry->application;

	// Prepare prices ctr validator
	this->prices_validator = new wxTextValidator(wxFILTER_NUMERIC);

	if (!wxXmlResource::Get()->Load(XRC_PATH))
		wxLogError("Coudn't load XRC resources !");

	if (!(this->frame = wxXmlResource::Get()->LoadFrame(nullptr,
		"MainFrame")))
		wxLogError("Coudn't load Main Frame from resources !");

	this->frame->Bind(wxEVT_CLOSE_WINDOW,
		&MainPage::OnClose, this);

	this->students_panel = new StudentsPanel(this);
	this->teachers_panel = new TeachersPanel(this);
	this->groups_panel = new GroupsPanel(this);
	this->transactions_panel = new TransactionsPanel(this);
	this->dashboard_panel = new DashboardPanel(this);

	this->PrepareNotebookHeader();

}

MainPage::~MainPage()
{
	if (wxXmlResource::Get()->Unload(XRC_PATH))
		wxLogError("Entry XRC resources unloaded!");

	delete students_panel,
		teachers_panel,
		groups_panel,
		transactions_panel,
		prices_validator,
		dashboard_panel;
}

void MainPage::Show()
{
	this->frame->Show();

	this->dashboard_panel->UpdateDashboard();

	this->students_panel->PrepareGrids();
	this->teachers_panel->PrepareGrids();
	this->groups_panel->PrepareGrids();
	this->transactions_panel->PrepareGrids();
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
	//	grid->DisableColResize(i);
	}

	grid->SetMargins(0 - wxSYS_VSCROLL_X - 30, 0);

	grid->EnableEditing(false);
	//grid->DisableRowResize(true);

	grid->SetDefaultCellFont(
		wxFont{ 10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL }
	);
	grid->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
}

void MainPage::OnClose(wxCloseEvent& e)
{
	// Forced it to close
	this->entry->Exit();
}

void MainPage::OnNotePageChanged(wxBookCtrlEvent& e)
{
	switch (e.GetSelection())
	{
		// Dashboard
		case 0:
			this->dashboard_panel->UpdateDashboard();
			break;
		// Transactions page is selected
		case 4:
			// Refresh transactions panel
			this->transactions_panel->PopulateTransactionsTable();
			break;
		case wxNOT_FOUND:
			break;
	}

	e.Skip();
}

void MainPage::PrepareNotebookHeader()
{
	this->notebook = XRCCTRL(*frame, "m_notebook1", wxNotebook);
	this->notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED,
		&MainPage::OnNotePageChanged, this);

	this->notebook->SetPageText(0, "Dashboard");
	this->notebook->SetPageText(1, "Students");
	this->notebook->SetPageText(2, "Teachers");
	this->notebook->SetPageText(3, "Groups");
	this->notebook->SetPageText(4, "Transactions");
}
