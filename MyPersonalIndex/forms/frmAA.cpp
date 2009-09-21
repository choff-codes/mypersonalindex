#include "frmAA.h"
#include "frmAAEdit.h"

frmAA::frmAA(const int &portfolioID, QWidget *parent, const QMap<int, globals::assetAllocation> &aa): QDialog(parent), m_aaMap(aa), m_portfolioID(portfolioID)
{
    sql = new aaQueries(m_portfolioID);

    ui.setupUI(this, "Desired Allocation", true, false);
    ui.table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->setWindowTitle("Edit Asset Allocation");
    connectSlots();

    m_aa = m_aaMap.values();
    m_model = new modelWithNoEdit(m_aa.count(), 2, ui.table);
    loadAA();
    ui.table->setModel(m_model);
}

void frmAA::loadAA()
{
    int i = 0;
    foreach(const globals::assetAllocation &aa, m_aa)
    {
        updateList(aa, i);
        i++;
    }
    updateHeader();
}

void frmAA::connectSlots()
{
    connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addAA()));
    connect(ui.btnEdit, SIGNAL(clicked()), this, SLOT(editAA()));
    connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(deleteAA()));
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
}

void frmAA::updateList(const globals::assetAllocation &aa, const int &row)
{
    int i = row == -1 ? m_model->rowCount() : row;

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
    foreach(const globals::assetAllocation &aa, m_aa)
        if (aa.target >= 0)
            d += aa.target;

    m_model->setHeaderData(0, Qt::Horizontal, "Description");
    m_model->setHeaderData(1, Qt::Horizontal, QString("Target (%L1%)").arg(d, 0, 'f', 2));
}

void frmAA::accept()
{
    QMap<int, globals::assetAllocation> toReturn;
    bool changes = false;

    sql->getDatabase().transaction();

    for(int i = 0; i < m_aa.count(); i++)
    {
        if (m_aa[i].id == -1 || m_aaMap.value(m_aa[i].id) != m_aa[i])
        {
            changes = true;
            sql->executeNonQuery(sql->updateAA(&m_aa[i]));
            if (m_aa[i].id == -1)
                m_aa[i].id = sql->executeScalar(sql->getIdentity()).toInt();
        }
        toReturn.insert(m_aa[i].id, m_aa[i]);
    }

    foreach(const globals::assetAllocation &aa, m_aaMap)
        if(!toReturn.contains(aa.id))
        {
            changes = true;
            sql->executeNonQuery(sql->deleteAA(aa.id));
        }

    sql->getDatabase().commit();

    if (changes)
    {
        m_aaMap = toReturn;
        QDialog::accept();
    }
    else
        QDialog::reject();
}

void frmAA::addAA()
{
    frmAAEdit f(this);

    if (f.exec())
    {
        globals::assetAllocation aa = f.getReturnValues();
        m_aa.append(aa);
        updateList(aa);
        updateHeader();
    }
}

void frmAA::editAA()
{
    QModelIndexList il = ui.table->selectionModel()->selectedRows();

    foreach(const QModelIndex &q, il)
    {
        int i = q.row();
        frmAAEdit f(this, m_aa.at(i));

        if (f.exec())
        {
            globals::assetAllocation aa = f.getReturnValues();
            m_aa[i] = aa;
            updateList(aa, i);
        }
        ui.table->selectionModel()->setCurrentIndex(q, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
    updateHeader();
}

void frmAA::deleteAA()
{
    QModelIndexList il = ui.table->selectionModel()->selectedRows();
    QList<int> indexes;
    foreach(const QModelIndex &q, il)
        indexes.append(q.row());
    qSort(indexes);

    for(int i = indexes.count() - 1; i >= 0; i--)
    {
        m_aa.removeAt(indexes.at(i));
        m_model->removeRow(indexes.at(i));
    }
    updateHeader();
}
