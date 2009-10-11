#include "frmAcct.h"
#include "frmAcctEdit.h"

frmAcct::frmAcct(const int &portfolioID, QWidget *parent, queries *sql, const QMap<int, globals::account> &acct):
    QDialog(parent), m_sql(sql), m_map(acct), m_portfolio(portfolioID)
{
    if(!m_sql || !m_sql->isOpen())
    {
        reject();
        return;
    }

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
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(m_model, SIGNAL(saveItem(globals::account*)), this, SLOT(saveItem(globals::account*)));
    connect(m_model, SIGNAL(deleteItem(globals::account)), this, SLOT(deleteItem(globals::account)));
}

void frmAcct::accept()
{
    QMap<int, globals::account> returnValues = m_model->saveList(m_map);

    if (returnValues != m_map)
    {
        m_map = returnValues;
        QDialog::accept();
    }
    else
        QDialog::reject();
}

void frmAcct::saveItem(globals::account *acct)
{
    m_sql->executeNonQuery(m_sql->updateAcct(m_portfolio, (*acct)));
    if (acct->id == -1)
        acct->id = m_sql->executeScalar(m_sql->getIdentity()).toInt();
}

void frmAcct::deleteItem(const globals::account &acct)
{
    m_sql->executeNonQuery(m_sql->deleteItem(queries::table_Acct, acct.id));
}

