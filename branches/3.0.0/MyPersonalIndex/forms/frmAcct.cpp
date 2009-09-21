#include "frmAcct.h"
#include "frmAcctEdit.h"

frmAcct::frmAcct(const int &portfolioID, QWidget *parent, const QMap<int, globals::account> &acct): QDialog(parent), m_acctMap(acct), m_portfolioID(portfolioID)
{
    sql = new acctQueries(m_portfolioID);

    ui.setupUI(this, "Account List", true, false);
    ui.table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->setWindowTitle("Edit Accounts");
    connectSlots();

    m_acct = m_acctMap.values();
    m_model = new modelWithNoEdit(m_acct.count(), 3, ui.table);
    m_model->setHeaderData(0, Qt::Horizontal, "Description");
    m_model->setHeaderData(1, Qt::Horizontal, "Tax Rate");
    m_model->setHeaderData(2, Qt::Horizontal, "Tax Deferred");
    loadAcct();
    ui.table->setModel(m_model);
}

void frmAcct::loadAcct()
{
    int i = 0;
    foreach(const globals::account &acct, m_acct)
    {
        updateList(acct, i);
        i++;
    }
}

void frmAcct::connectSlots()
{
    connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addAcct()));
    connect(ui.btnEdit, SIGNAL(clicked()), this, SLOT(editAcct()));
    connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(deleteAcct()));
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
}

void frmAcct::updateList(const globals::account &acct, const int &row)
{
    int i = row == -1 ? m_model->rowCount() : row;

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

void frmAcct::accept()
{
    QMap<int, globals::account> toReturn;
    bool changes = false;

    sql->getDatabase().transaction();

    for(int i = 0; i < m_acct.count(); i++)
    {
        if (m_acct[i].id == -1 || m_acctMap.value(m_acct[i].id) != m_acct[i])
        {
            changes = true;
            sql->executeNonQuery(sql->updateAcct(&m_acct[i]));
            if (m_acct[i].id == -1)
                m_acct[i].id = sql->executeScalar(sql->getIdentity()).toInt();
        }
        toReturn.insert(m_acct[i].id, m_acct[i]);
    }

    foreach(const globals::account &acct, m_acctMap)
        if(!toReturn.contains(acct.id))
        {
            changes = true;
            sql->executeNonQuery(sql->deleteAcct(acct.id));
        }

    sql->getDatabase().commit();

    if (changes)
    {
        m_acctMap = toReturn;
        QDialog::accept();
    }
    else
        QDialog::reject();
}

void frmAcct::addAcct()
{
    frmAcctEdit f(this);

    if (f.exec())
    {
        globals::account acct = f.getReturnValues();
        m_acct.append(acct);
        updateList(acct);
    }
}

void frmAcct::editAcct()
{
    QModelIndexList il = ui.table->selectionModel()->selectedRows();

    foreach(const QModelIndex &q, il)
    {
        int i = q.row();
        frmAcctEdit f(this, m_acct.at(i));

        if (f.exec())
        {
            globals::account acct = f.getReturnValues();
            m_acct[i] = acct;
            updateList(acct, i);
        }
        ui.table->selectionModel()->setCurrentIndex(q, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void frmAcct::deleteAcct()
{
    QModelIndexList il = ui.table->selectionModel()->selectedRows();
    QList<int> indexes;
    foreach(const QModelIndex &q, il)
        indexes.append(q.row());
    qSort(indexes);

    for(int i = indexes.count() - 1; i >= 0; i--)
    {
        m_acct.removeAt(indexes.at(i));
        m_model->removeRow(indexes.at(i));
    }
}

