#include "frmAcct.h"

frmAcct::frmAcct(const int &portfolioID, const QMap<int, account> &acct, QWidget *parent):
    QDialog(parent), m_portfolio(portfolioID), m_sql(queries("account")), m_map(acct)
{
    ui.setupUI(this, "Accounts", false);
    this->setWindowTitle("Edit Accounts");

    m_model = new acctModel(m_map.values(), 3, ui.table, this);
    ui.table->setModel(m_model);

    connectSlots();
}

void frmAcct::connectSlots()
{
    connect(ui.btnAdd, SIGNAL(clicked()), m_model, SLOT(addNew()));
    connect(ui.btnEdit, SIGNAL(clicked()), m_model, SLOT(editSelected()));
    connect(ui.table, SIGNAL(doubleClicked(QModelIndex)), m_model, SLOT(editSelected()));
    connect(ui.btnDelete, SIGNAL(clicked()), m_model, SLOT(deleteSelected()));
    connect(ui.copy, SIGNAL(triggered()), m_model, SLOT(copy()));
    connect(ui.copyShortcut, SIGNAL(activated()), m_model, SLOT(copy()));
    connect(ui.paste, SIGNAL(triggered()), m_model, SLOT(paste()));
    connect(ui.pasteShortcut, SIGNAL(activated()), m_model, SLOT(paste()));
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(m_model, SIGNAL(saveItem(account*)), this, SLOT(saveItem(account*)));
    connect(m_model, SIGNAL(deleteItem(account)), this, SLOT(deleteItem(account)));
    connect(ui.table, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
}

void frmAcct::accept()
{
    QMap<int, account> returnValues = m_model->saveList(m_map);

    if (returnValues != m_map)
    {
        m_map = returnValues;
        QDialog::accept();
    }
    else
        QDialog::reject();
}

void frmAcct::saveItem(account *acct)
{
    m_sql.executeNonQuery(queries::updateAcct(m_portfolio, (*acct)));
    if (acct->id == -1)
        acct->id = m_sql.getIdentity();
}

void frmAcct::deleteItem(const account &acct)
{
    m_sql.executeNonQuery(queries::deleteItem(queries::table_Acct, acct.id));
}

void frmAcct::customContextMenuRequested(const QPoint&)
{
    ui.popup->popup(QCursor::pos());
}
