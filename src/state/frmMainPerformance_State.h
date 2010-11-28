#ifndef FRMMAINPERFORMANCE_STATE_H
#define FRMMAINPERFORMANCE_STATE_H

#include <frmMainStateTable.h>
#include "historicalPrices.h"

class QTreeWidgetItem;
class frmMainTableView_UI;
class frmMainPerformance_State : public frmMainStateTable
{
public:
    frmMainPerformance_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_,
        const QHash<QString, historicalPrices> &prices_, QWidget *parent_);

    ~frmMainPerformance_State();

private:
    const QHash<QString, historicalPrices> m_prices;

    settings::columns columnEnumValue();
    QMap<int, QString> tableColumns();

    frmMainTableView_UI* createUI();
    mpiViewModelBase* createModel(int beginDate_, int endDate_);
    QTreeWidgetItem* createTreeItem(int type_, const QString description_);
    QTreeWidgetItem* createTreeItem(int type_, const QString description_, const QString itemData_);

};

#endif // FRMMAINPERFORMANCE_STATE_H
