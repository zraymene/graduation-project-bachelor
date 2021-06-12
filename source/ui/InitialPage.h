#pragma once
#include "../core/Application.h"

#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "../Entry.h"

class Entry;
class InitialPage;

class InitDialog {
public:

	InitDialog(InitialPage * page);

	bool Show();
	void Close(bool hide = false);

protected:
	wxDialog dialog;
	InitialPage* page = nullptr;

	bool dialog_loaded = false;

	void OnClose(wxCloseEvent& e);
};

class SettingsDialog : public InitDialog {
public:

	SettingsDialog(InitialPage* app) : InitDialog(app) {};
	~SettingsDialog();

	void Show(bool auto_connect = false);

private:

	wxChoice* database_choice;
	wxTextCtrl* hostname_ctr;
	wxTextCtrl* username_ctr;
	wxTextCtrl* password_ctr;
	wxButton* save_button;

	void SaveButtonClick(wxCommandEvent& event);
	void Init(bool auto_connect);
	void TryConnect();

};

class RegisterDialog : public InitDialog {
public:

	RegisterDialog(InitialPage* app) : InitDialog(app) {};
	~RegisterDialog();

	void Show();

private:

	wxTextCtrl* username_ctr;
	wxTextCtrl* password_ctr;
	wxButton* register_button;
	wxButton* settings_button;

	void Register(wxCommandEvent& event);
	void SwapToSettings(wxCommandEvent& event);
};

class LoginDialog : public InitDialog{
public:

	LoginDialog(InitialPage* app) : InitDialog(app) {};
	~LoginDialog();

	void Show();

private:

	wxTextCtrl* username_ctr;
	wxTextCtrl* password_ctr;
	wxButton* login_button;
	wxButton* settings_button;

	void Login(wxCommandEvent& event);
	void SwapToSettings(wxCommandEvent& event);
};

class InitialPage
{
public:

	Application* app;
	Entry* entry;

	InitialPage(Entry* entry);
	~InitialPage();

	template<class T>
	void ShowDialog() {
		if (std::is_same<T, SettingsDialog>::value)
		{
			if (!this->settings_dialog)
				this->settings_dialog = new SettingsDialog(this);

			this->settings_dialog->Show();
			return;
		}

		if (std::is_same<T, LoginDialog>::value)
		{
			if (!this->login_dialog)
				this->login_dialog = new LoginDialog(this);

			this->login_dialog->Show();
			return;
		}

		if (std::is_same<T, RegisterDialog>::value)
		{
			if (!this->register_dialog)
				this->register_dialog = new RegisterDialog(this);

			this->register_dialog->Show();
			return;
		}
	}

	template<class T>
	void HideDialog() {
		if (std::is_same<T, SettingsDialog>::value)
		{
			this->settings_dialog->Close(true);
			return;
		}

		if (std::is_same<T, LoginDialog>::value)
		{
			this->login_dialog->Close(true);
			return;
		}

		if (std::is_same<T, RegisterDialog>::value)
		{
			this->register_dialog->Close(true);
			return;
		}
	}

	void Close();

private:
	 
	SettingsDialog* settings_dialog = nullptr;
	LoginDialog* login_dialog = nullptr;
	RegisterDialog* register_dialog = nullptr;
};

