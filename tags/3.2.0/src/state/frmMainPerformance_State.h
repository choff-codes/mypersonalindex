#ifndef FRMMAINPERFORMANCE_STATE_H
#define FRMMAINPERFORMANCE_STATE_H

#include "frmMainStateTableWithTree.h"

class QTreeWidgetItem;
class frmMainPerformance_State : public frmMainStateTableWithTree
{
    Q_OBJECT

public:
    frmMainPerformance_State(int portfolioID_, const QMap<int, portfolio> &portfolios_, const settings &settings_,
        const QHash<QString, historicalPrices> &prices_, QWidget *parent_);

    ~frmMainPerformance_State();

protected slots:
    void itemClicked(QTreeWidgetItem *item_, int column_);

protected:
    settings::columns columnEnumValue();
    QMap<int, QString> tableColumns();

    mpiViewModelBase* createModel(int beginDate_, int endDate_);
    QTreeWidgetItem* createTreeItem(objectType type_, int portfolioID_, int id_, const QString &description_, const QString &itemData_ = QString());

private:
    void populateTree(int portfolioID_);
};

#endif // FRMMAINPERFORMANCE_STATE_H
