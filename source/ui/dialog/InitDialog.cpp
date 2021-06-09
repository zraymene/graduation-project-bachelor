#include "../InitialPage.h"

InitDialog::InitDialog(InitialPage* page)
{
	this->page = page;
}

bool InitDialog::Show()
{
	if (dialog)
	{
		this->dialog->Show();
		return false;
	}

	this->dialog = new wxDialog();

	return true;
}

void InitDialog::Close(bool hide)
{
	if (hide)
		this->dialog->Hide();
	else
		this->dialog->Destroy();
}
