#include "frmStat.h"

frmStat::frmStat(const int &portfolioID, const QMap<int, statistic> &stat, const QList<int> &statList, const queries &sql, QWidget *parent):
    QDialog(parent), m_portfolio(portfolioID), m_map(stat), m_selected(statList), m_sql(sql)
{
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
    connect(m_model, SIGNAL(saveItem(statistic*)), this, SLOT(saveItem(statistic*)));
    connect(m_model, SIGNAL(deleteItem(statistic)), this, SLOT(deleteItem(statistic)));
}

void frmStat::accept()
{
    QMap<int, statistic> returnValues = m_model->saveList(m_map);
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
    tableValues.insert(queries::statMappingColumns.at(queries::statMappingColumns_PortfolioID), portfolio);
    tableValues.insert(queries::statMappingColumns.at(queries::statMappingColumns_StatID), stat);
    tableValues.insert(queries::statMappingColumns.at(queries::statMappingColumns_Sequence), sequence);

    m_sql.executeNonQuery(queries::deletePortfolioItems(queries::table_StatMapping, m_portfolio, false));
    if (!stat.isEmpty())
    {
        queries::queries &tableUpdateQuery = const_cast<queries::queries&>(m_sql);
        tableUpdateQuery.executeTableUpdate(queries::table_StatMapping, tableValues);
    }

    m_map = returnValues;
    m_selected = returnValuesSelected;
    QDialog::accept();
}

void frmStat::saveItem(statistic *stat)
{
    m_sql.executeNonQuery(queries::updateStat((*stat)));
    if (stat->id == -1)
        stat->id = m_sql.getIdentity();
}

void frmStat::deleteItem(const statistic &stat)
{
    m_sql.executeNonQuery(queries::deleteItem(queries::table_Stat, stat.id));
}
