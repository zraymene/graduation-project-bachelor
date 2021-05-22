#pragma once

#include "wx/wx.h"

#include "ui/MainFrame.h"

class Entry : public wxApp
{
public:
	virtual bool OnInit();

private:
	MainFrame* main_frame = nullptr;
	wxLogWindow* log_windows = nullptr;
};

