#include "InitialPage.h"

#include "wx/xrc/xmlres.h"

#define XRC_PATH "rc/entry.xrc"

InitialPage::InitialPage(Application* app)
{
	this->app = app;

	if (!wxXmlResource::Get()->Load(XRC_PATH))
		wxLogError("Coudn't load XRC resources !");
}

InitialPage::~InitialPage()
{
	if (wxXmlResource::Get()->Unload(XRC_PATH))
		wxLogError("Entry XRC resources unloaded!");

}

