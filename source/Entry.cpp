#include "Entry.h"

#include "core/dal/UnitOfWork.h"
#include "core/dal/mysql/MYSQLUnitOfWork.h"

//wxIMPLEMENT_APP_CONSOLE(Entry);
wxIMPLEMENT_APP(Entry);

bool Entry::OnInit()
{
	this->application = new Application();
	
	wxXmlResource::Get()->InitAllHandlers();

	log_windows = new wxLogWindow(nullptr, "Console");
	log_windows->Show();
	
	// Uncomment this for release
	this->init_page = new InitialPage(this);
	this->init_page->ShowDialog<SettingsDialog>();

	// this for debug only 
	//this->SwitchToMainPage();

	return true;
}

int Entry::OnExit()
{
	delete this->application;
	return 0;
}

void Entry::SwitchToMainPage()
{
	this->main_page = new MainPage(this->application);
	this->main_page->Show();

	delete this->init_page;
}



