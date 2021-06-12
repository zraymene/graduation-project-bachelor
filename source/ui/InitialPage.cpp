#include "InitialPage.h"

#include "wx/xrc/xmlres.h"

#define XRC_PATH "rc/entry.xrc"

InitialPage::InitialPage(Entry* entry)
{
	this->entry = entry;
	this->app = entry->application;

	if (!wxXmlResource::Get()->Load(XRC_PATH))
		wxLogError("Coudn't load XRC resources !");
}

InitialPage::~InitialPage()
{
	if (!wxXmlResource::Get()->Unload(XRC_PATH))
		wxLogError("Entry XRC resources unloaded!");

	this->Close();

	delete this->settings_dialog;
	delete this->login_dialog;
	delete this->register_dialog;

}

void InitialPage::Close()
{
	if(this->login_dialog)
		this->login_dialog->Close();

	if(this->settings_dialog)
		this->settings_dialog->Close();

	if(this->register_dialog)
		this->register_dialog->Close();
}

