#include "frmStat.h"
#include "frmStatEdit.h"

frmStat::frmStat(const int &portfolioID, QWidget *parent, queries *sql, const QMap<int, globals::statistic> &stat, const QList<int> &statList):
    QDialog(parent), m_sql(sql), m_map(stat), m_portfolio(portfolioID), m_selected(statList)
{
    if(!m_sql || !m_sql->isOpen())
    {
        reject();
        return;
    }

    ui.setupUI(this, "Statistics", true);
    this->setWindowTitle("Edit Statistics");

    m_model = new statModel(m_map.values(), m_selected, 1, ui.table, this);
    ui.table->setModel(m_model);

    connectSlots();
}

void frmStat::connectSlots()
{
    connect(ui.btnAdd, SIGNAL(clicked()), m_model, SLOT(addNew()));
    connect(ui.btnEdit, SIGNAL(clicked()), m_model, SLOT(editSelected()));
    connect(ui.table, SIGNAL(doubleClicked(QModelIndex)), m_model, SLOT(editSelected()));
    connect(ui.btnDelete, SIGNAL(clicked()), m_model, SLOT(deleteSelected()));
    connect(ui.btnSelectAll, SIGNAL(clicked()), m_model, SLOT(selectAll()));
    connect(ui.btnClear, SIGNAL(clicked()), m_model, SLOT(clearAll()));
    connect(ui.btnMoveUp, SIGNAL(clicked()), m_model, SLOT(moveSelectedUp()));
    connect(ui.btnMoveDown, SIGNAL(clicked()), m_model, SLOT(moveSelectedDown()));
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(m_model, SIGNAL(saveItem(globals::statistic*)), this, SLOT(saveItem(globals::statistic*)));
    connect(m_model, SIGNAL(deleteItem(globals::statistic)), this, SLOT(deleteItem(globals::statistic)));
}

void frmStat::accept()
{
    QMap<int, globals::statistic> returnValues = m_model->saveList(m_map);
    QList<int> returnValuesSelected = m_model->getSelected();

    if (returnValues == m_map && returnValuesSelected == m_selected)
    {
        QDialog::reject();
        return;
    }

    QVariantList portfolio, stat, sequence;
    for (int i = 0; i < returnValuesSelected.count(); ++i)
    {
        stat.append(returnValuesSelected.at(i));
        portfolio.append(m_portfolio);
        sequence.append(i);
    }

    QMap<QString, QVariantList> tableValues;
    tableValues.insert(queries::statMappingColumns.at(queries::statMapping_PortfolioID), portfolio);
    tableValues.insert(queries::statMappingColumns.at(queries::statMapping_StatID), stat);
    tableValues.insert(queries::statMappingColumns.at(queries::statMapping_Sequence), sequence);

    m_sql->executeNonQuery(m_sql->deletePortfolioItems(queries::table_StatMapping, m_portfolio));
    if (stat.count() != 0)
        m_sql->executeTableUpdate(queries::table_StatMapping, tableValues);

    m_map = returnValues;
    m_selected = returnValuesSelected;
    QDialog::accept();
}

void frmStat::saveItem(globals::statistic *stat)
{
    m_sql->executeNonQuery(m_sql->updateStat((*stat)));
    if (stat->id == -1)
        stat->id = m_sql->executeScalar(m_sql->getIdentity()).toInt();
}

void frmStat::deleteItem(const globals::statistic &stat)
{
    m_sql->executeNonQuery(m_sql->deleteItem(queries::table_Stat, stat.id));
}
