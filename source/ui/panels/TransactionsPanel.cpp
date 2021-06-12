#include "../MainPage.h"

#include "wx/wx.h"
#include <wx/numformatter.h>

#define TRANS_MANAGER this->main->app->GetTransactionsManager()

TransactionsPanel::TransactionsPanel(MainPage* main)
{
	this->main = main;

	this->transaction_name_ctr = XRCCTRL(*(this->main->frame), "transaction_name_ctr", wxTextCtrl);
	this->transaction_amount_ctr = XRCCTRL(*(this->main->frame), "transaction_amount_ctr", wxTextCtrl);
	this->transaction_amount_ctr->SetValidator(*this->main->prices_validator);

	this->transactions_grid = XRCCTRL(*(this->main->frame), "transactions_grid", wxGrid);

	this->add_btn = XRCCTRL(*(this->main->frame), "add_transaction_button", wxButton);
	this->edit_btn = XRCCTRL(*(this->main->frame), "edit_transaction_button", wxButton);
	this->delete_btn = XRCCTRL(*(this->main->frame), "delete_transaction_button", wxButton);

	// Event binding
	this->add_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&TransactionsPanel::AddButtonOnClick, this);
	this->edit_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&TransactionsPanel::EditButtonOnClick, this);
	this->delete_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
		&TransactionsPanel::DeleteButtonOnClick, this);

	this->transactions_grid->Bind(wxEVT_GRID_SELECT_CELL,
		&TransactionsPanel::OnTransactionSelected, this);

}

TransactionsPanel::~TransactionsPanel()
{

}

void TransactionsPanel::PrepareGrids()
{
	this->transactions_grid->CreateGrid(0, 3);
	this->PopulateTransactionsTable();
	this->transactions_grid->SetColLabelValue(0, "Name");
	this->transactions_grid->SetColLabelValue(1, "Amount");
	this->transactions_grid->SetColLabelValue(2, "Date");
	MainPage::StyleGrid(this->transactions_grid);

}

void TransactionsPanel::AddButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Add transaction?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		Transaction t;
		t.name = this->transaction_name_ctr->GetValue().ToStdString();
		t.amount = std::atof(this->transaction_amount_ctr->GetValue().c_str());

		TRANS_MANAGER->AddTransaction(&t);

		this->transaction_list.push_back(t);
		this->PopulateTransactionsTable();

		this->ResetControls();

		wxMessageBox("Transaction added!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::runtime_error e)
	{
		wxMessageBox(e.what(),
			"Transaction adding Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void TransactionsPanel::EditButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Edit transaction?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		this->selected_transaction.name =
			this->transaction_name_ctr->GetValue().ToStdString();
		this->selected_transaction.amount =
			std::atof(this->transaction_amount_ctr->GetValue().c_str());

		TRANS_MANAGER->EditTransaction(this->selected_transaction);

		// Update transaction in student list
		this->transaction_list.at(selected_row) = this->selected_transaction;
		this->SetTransactionRow(this->selected_transaction, selected_row);

		this->ResetControls();

		wxMessageBox("Transaction edited!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::runtime_error e)
	{
		wxMessageBox(e.what(),
			"Transaction Editing Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void TransactionsPanel::DeleteButtonOnClick(wxCommandEvent& event)
{
	int answer = wxMessageBox("Delete transaction?", "Confirm",
		wxYES_NO, nullptr);
	if (answer != wxYES)
		return;

	try {

		TRANS_MANAGER->DeleteTransaction(
			this->selected_transaction.id
		);

		this->transaction_list.erase(this->transaction_list.begin() + (selected_row));
		this->transactions_grid->DeleteRows(selected_row);

		this->ResetControls();

		wxMessageBox("Transaction deleted!",
			"Success", wxICON_INFORMATION | wxOK);

		event.Skip();
	}
	catch (std::runtime_error e)
	{
		wxMessageBox(e.what(),
			"Transaction Deleting Error",
			wxICON_ERROR | wxOK);

		event.Skip();
	}
}

void TransactionsPanel::OnTransactionSelected(wxGridEvent& e)
{
	this->ResetControls();

	this->selected_transaction = this->transaction_list.at(e.GetRow());
	this->selected_row = e.GetRow();

	this->transaction_name_ctr->SetValue(selected_transaction.name);
	this->transaction_amount_ctr->SetValue(
		wxNumberFormatter::ToString(selected_transaction.amount, 3)
	);

	this->edit_btn->Enable();
	this->delete_btn->Enable();
}

void TransactionsPanel::SetTransactionRow(Transaction p, int i)
{
	if(p.amount > 0)
		this->transactions_grid->SetCellTextColour(i, 1, *wxGREEN);
	else
		this->transactions_grid->SetCellTextColour(i, 1, *wxRED);

	this->transactions_grid->ForceRefresh();

	this->transactions_grid->SetCellValue(wxGridCellCoords{ i,0 },
		p.name);
	this->transactions_grid->SetCellValue(wxGridCellCoords{ i,1 },
		wxNumberFormatter::ToString(p.amount, 3));
	this->transactions_grid->SetCellValue(wxGridCellCoords{ i,2 },
		p.date);
}

void TransactionsPanel::PopulateTransactionsTable()
{
	if (this->transactions_grid->GetNumberRows() > 0)
	{
		this->transactions_grid->DeleteRows(0, this->transactions_grid->GetNumberRows());
		this->transactions_grid->ForceRefresh();
	}

	try
	{
		this->transaction_list = TRANS_MANAGER->GetTransactions();
	}
	catch (std::runtime_error e)
	{
		wxMessageBox(e.what(),
			"Loading transactions table",
			wxICON_ERROR | wxOK);
	}

	std::vector<Transaction>::iterator iter;

	int j = 0;
	for (iter = this->transaction_list.begin(); iter < this->transaction_list.end(); iter++, j++)
	{
		this->transactions_grid->AppendRows(1, false);
		this->SetTransactionRow((*iter), j);
	}
}

void TransactionsPanel::ResetControls()
{
	// Reset input
	this->transaction_name_ctr->SetValue("");
	this->transaction_amount_ctr->SetValue("");

	// Disable buttons
	this->edit_btn->Enable(0);
	this->delete_btn->Enable(0);
}
