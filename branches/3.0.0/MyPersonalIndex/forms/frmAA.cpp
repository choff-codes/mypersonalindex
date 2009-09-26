#include "frmAA.h"
#include "frmAAEdit.h"

frmAA::frmAA(const int &portfolioID, QWidget *parent, queries *sql, const QMap<int, globals::assetAllocation> &aa):
    frmTableViewBase<globals::assetAllocation, frmAAEdit>(portfolioID, parent, sql, aa, false, "Asset Allocation", 2)
{
    if(!m_sql || !m_sql->isOpen())
    {
        reject();
        return;
    }

    this->setWindowTitle("Edit Asset Allocation");
    connectSlots();
    loadItems();
}

void frmAA::connectSlots()
{
    connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addAA()));
    connect(ui.btnEdit, SIGNAL(clicked()), this, SLOT(editAA()));
    connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(removeAA()));
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
}

void frmAA::updateList(const globals::assetAllocation &aa, const int &row)
{
    int i = row == -1 ? m_model->rowCount() : row; // -1 is an insert

    QStandardItem *desc = new QStandardItem(aa.description);
    QStandardItem *target = new QStandardItem(
            aa.target < 0 ? "None" :
            QLocale().toString(aa.target, 'f', 2).append("%")
        );
    m_model->setItem(i, 0, desc);
    m_model->setItem(i, 1, target);
}

void frmAA::updateHeader()
{
    double d = 0;
    foreach(const globals::assetAllocation &aa, m_list)
        if (aa.target >= 0)
            d += aa.target;

    m_model->setHeaderData(0, Qt::Horizontal, "Description");
    m_model->setHeaderData(1, Qt::Horizontal, QString("Target (%L1%)").arg(d, 0, 'f', 2));
}

void frmAA::accept()
{
    frmTableViewBase<globals::assetAllocation, frmAAEdit>::accept();
}

void frmAA::saveItem(const globals::assetAllocation &aa)
{
    m_sql->executeNonQuery(m_sql->updateAA(m_portfolioID, aa));
}

void frmAA::deleteItem(const globals::assetAllocation &aa)
{
    m_sql->executeNonQuery(m_sql->deleteItem(queries::table_AA, aa.id));
}

void frmAA::addAA()
{
    addItem();
}

void frmAA::editAA()
{
    editItem();
}

void frmAA::removeAA()
{
    removeItem();
}
