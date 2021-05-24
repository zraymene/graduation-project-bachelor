#include "Entry.h"

#include "core/dal/UnitOfWork.h"
#include "core/dal/mysql/MYSQLUnitOfWork.h"
#include "ui/dialog/SettingsDialog.h"
#include "ui/dialog/LoginDialog.h"
#include "ui/dialog/RegisterDialog.h"

//wxIMPLEMENT_APP_CONSOLE(Entry);
wxIMPLEMENT_APP(Entry);

bool Entry::OnInit()
{
	this->application = new Application();

	/*main_frame = new MainFrame();
	main_frame->Show();
	*/

	log_windows = new wxLogWindow(nullptr, "Console");
	log_windows->Show();

	SettingsDialog* settingd_dialog = new SettingsDialog(this->application, false);
	settingd_dialog->Show();

	/*RegisterDialog* register_dialog = new RegisterDialog(this->application);
	register_dialog->Show();
	*/
	//Database::test();
	//MYSQLUnitOfWork::test();
	return true;
}

int Entry::OnExit()
{
	delete this->application;

	wxLogDebug("SHEES");
	return 0;
}



