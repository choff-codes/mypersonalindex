#include "frmStat.h"
#include "frmStatEdit.h"

frmStat::frmStat(const int &portfolioID, QWidget *parent, queries *sql, const QMap<int, globals::statistic> &stat):
    frmTableViewBase<globals::statistic, frmStatEdit>(portfolioID, parent, sql, stat, true, "Choose Statistics", 1)
{
    if(!m_sql || !m_sql->isOpen())
    {
        reject();
        return;
    }

    this->setWindowTitle("Edit Statistics");
    m_model->setHeaderData(0, Qt::Horizontal, "Statistic");
    connectSlots();
    loadItems();

//    for(int i = 0; i < m_model->rowCount(); ++i)
//    {
//        m_model->item(i, 0)->setCheckable(true);
//        //m_model->item(i, 0)->setCheckState(m_list[i].
//    }

//    for(int i = 0; i < m_model->rowCount(); ++i)
//    {
//        //m_model->setData(m_model->index(i, 0), Qt::Checked, Qt::CheckStateRole);
//        m_model->item(i, 0)->setCheckable(true);// ->setFlags(m_model->item(i,0)->flags() | Qt::ItemIsUserCheckable );
//        if (i == 1) m_model->item(i, 0)->setCheckState(Qt::Checked);
//    }
//
//    connect(m_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemischaged(QStandardItem*)));

}

void frmStat::connectSlots()
{
    connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addStat()));
    connect(ui.btnEdit, SIGNAL(clicked()), this, SLOT(editStat()));
    connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(removeStat()));
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
}

void frmStat::itemChecked(QStandardItem *q)
{
    Qt::CheckState c;
    int i = q->index().row();
    c = q->checkState();
    QString s;
    s = "test";
}

void frmStat::updateList(const globals::statistic &stat, const int &row)
{
    int i = row == -1 ? m_model->rowCount() : row; // -1 is an insert

    QStandardItem *desc = new QStandardItem(stat.description);
    m_model->setItem(i, 0, desc);
}

void frmStat::accept()
{
    frmTableViewBase<globals::statistic, frmStatEdit>::accept();
}

void frmStat::saveItem(const globals::statistic &stat)
{
    //m_sql->executeNonQuery(m_sql->updateAA(m_portfolioID, aa));
}

void frmStat::deleteItem(const globals::statistic &stat)
{
    //m_sql->executeNonQuery(m_sql->deleteItem(queries::table_AA, aa.id));
}

void frmStat::addStat()
{
    addItem();
}

void frmStat::editStat()
{
    editItem();
}

void frmStat::removeStat()
{
    removeItem();
}
