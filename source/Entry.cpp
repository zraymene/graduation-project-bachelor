#include "Entry.h"

#include "core/dal/UnitOfWork.h"
#include "core/dal/mysql/MYSQLUnitOfWork.h"

//wxIMPLEMENT_APP_CONSOLE(Entry);
wxIMPLEMENT_APP(Entry);

bool Entry::OnInit()
{

	/*main_frame = new MainFrame();
	main_frame->Show();
	*/

	log_windows = new wxLogWindow(nullptr, "Console");
	log_windows->Show();

	//Database::test();
	MYSQLUnitOfWork::test();
	return true;
}


