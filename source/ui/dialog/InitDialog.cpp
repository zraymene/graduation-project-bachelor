#include "../InitialPage.h"

InitDialog::InitDialog(InitialPage* page)
{
	this->page = page;
	
}

bool InitDialog::Show()
{
	// SEG FAULT IN LINUX HIR !!!
	if (dialog_loaded)
	{
		this->dialog.Show();
		return false;
	}

	dialog_loaded = true;
	//this->dialog = new wxDialog();

	return true;
}

void InitDialog::Close(bool hide)
{
	if (hide)
		this->dialog.Hide();
	else
	{
		if(dialog_loaded)
			this->dialog.Destroy();
	}
}

void InitDialog::OnClose(wxCloseEvent& e)
{
	//this->page->entry->Exit();
	
	this->page->Close();
	e.Skip();
}
