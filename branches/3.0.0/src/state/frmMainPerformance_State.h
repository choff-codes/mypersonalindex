#ifndef FRMMAINPERFORMANCE_STATE_H
#define FRMMAINPERFORMANCE_STATE_H

#include "frmMainStateTableWithTree.h"

class QTreeWidgetItem;
class frmMainPerformance_State : public frmMainStateTableWithTree
{
public:
    frmMainPerformance_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_,
        const QHash<QString, historicalPrices> &prices_, QWidget *parent_);

    ~frmMainPerformance_State();

protected:
    settings::columns columnEnumValue();
    QMap<int, QString> tableColumns();

    mpiViewModelBase* createModel(int beginDate_, int endDate_);
    QTreeWidgetItem* createTreeItem(int type_, const QString &description_);
    QTreeWidgetItem* createTreeItem(int type_, const QString &description_, const QString &itemData_);

};

#endif // FRMMAINPERFORMANCE_STATE_H
