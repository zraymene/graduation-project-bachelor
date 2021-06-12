#pragma once

#include "wx/wx.h"

#include "core/Application.h"
#include "wx/xrc/xmlres.h"

#include "ui/MainPage.h"
#include "ui/InitialPage.h"

class InitialPage;
class MainPage;

class Entry : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();

	void SwitchToMainPage();

	Application* application = nullptr;

private:
	
	wxLogWindow* log_windows = nullptr;

	InitialPage* init_page = nullptr;
	MainPage* main_page = nullptr;
};

