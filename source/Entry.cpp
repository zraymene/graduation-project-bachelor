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
	
	this->init_page = new InitialPage(this->application);
	this->init_page->ShowDialog<SettingsDialog>();

	return true;
}

int Entry::OnExit()
{
	delete this->application;
	delete this->init_page;

	wxLogDebug("SHEES");
	return 0;
}



