///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __NONAME_H__
#define __NONAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/dialog.h>
#include <wx/choice.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class LoginDialog
///////////////////////////////////////////////////////////////////////////////
class LoginDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText5;
		wxTextCtrl* username_ctr;
		wxStaticText* m_staticText6;
		wxTextCtrl* password_ctr;
		wxButton* m_button5;
		wxButton* m_button7;
		
		// Virtual event handlers, overide them in your derived class
		virtual void Login( wxCommandEvent& event ) { event.Skip(); }
		virtual void SwapToSettings( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		LoginDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Login"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~LoginDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class RegisterDialog
///////////////////////////////////////////////////////////////////////////////
class RegisterDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText5;
		wxTextCtrl* username_ctr;
		wxStaticText* m_staticText6;
		wxTextCtrl* password_ctr;
		wxButton* m_button5;
		wxButton* m_button7;
		
		// Virtual event handlers, overide them in your derived class
		virtual void Register( wxCommandEvent& event ) { event.Skip(); }
		virtual void SwapToSettings( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		RegisterDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Register"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~RegisterDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SettingsDialog
///////////////////////////////////////////////////////////////////////////////
class SettingsDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText20;
		wxChoice* database_choice;
		wxStaticText* m_staticText5;
		wxTextCtrl* hostname_ctr;
		wxStaticText* m_staticText6;
		wxTextCtrl* username_ctr;
		wxStaticText* m_staticText61;
		wxTextCtrl* password_ctr;
		wxButton* m_button7;
		
		// Virtual event handlers, overide them in your derived class
		virtual void SaveButton( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SettingsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~SettingsDialog();
	
};

#endif //__NONAME_H__
