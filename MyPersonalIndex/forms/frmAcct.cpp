#include "frmAcct.h"
#include "frmAcctEdit.h"

frmAcct::frmAcct(const int &portfolioID, QWidget *parent, queries *sql, const QMap<int, globals::account> &acct):
    frmTableViewBase<globals::account, frmAcctEdit>(portfolioID, parent, sql, acct, false, "Edit Accounts", 3)
{
    if(!m_sql || !m_sql->isOpen())
    {
        reject();
        return;
    }

    this->setWindowTitle("Edit Accounts");
    m_model->setHeaderData(0, Qt::Horizontal, "Description");
    m_model->setHeaderData(1, Qt::Horizontal, "Tax Rate");
    m_model->setHeaderData(2, Qt::Horizontal, "Tax Deferred");
    connectSlots();
    loadItems();
}

void frmAcct::connectSlots()
{
    connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addAcct()));
    connect(ui.btnEdit, SIGNAL(clicked()), this, SLOT(editAcct()));
    connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(removeAcct()));
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
}

void frmAcct::updateList(const globals::account &acct, const int &row)
{
    int i = row == -1 ? m_model->rowCount() : row; // -1 is an insert

    QStandardItem *desc = new QStandardItem(acct.description);
    QStandardItem *taxRate = new QStandardItem(
            acct.taxRate < 0 ? "None" :
            QLocale().toString(acct.taxRate, 'f', 2).append("%")
        );
    QStandardItem *taxDeferred = new QStandardItem(acct.taxDeferred ? "Yes" : "No");
    m_model->setItem(i, 0, desc);
    m_model->setItem(i, 1, taxRate);
    m_model->setItem(i, 2, taxDeferred);
}

void frmAcct::deleteItem(const globals::account &acct)
{
    m_sql->executeNonQuery(m_sql->deleteItem(queries::table_Acct, acct.id));
}

void frmAcct::saveItem(const globals::account &acct)
{
    m_sql->executeNonQuery(m_sql->updateAcct(m_portfolioID, acct));
}

void frmAcct::accept()
{
    frmTableViewBase<globals::account, frmAcctEdit>::accept();
}

void frmAcct::addAcct()
{
    addItem();
}

void frmAcct::editAcct()
{
    editItem();
}

void frmAcct::removeAcct()
{
    removeItem();
}

