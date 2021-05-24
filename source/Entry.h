#pragma once

#include "wx/wx.h"

#include "ui/MainFrame.h"
#include "core/Application.h"

class Entry : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();

private:
	MainFrame* main_frame = nullptr;
	wxLogWindow* log_windows = nullptr;

	Application* application;
};

