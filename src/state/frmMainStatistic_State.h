#ifndef FRMMAINSTATISTIC_STATE_H
#define FRMMAINSTATISTIC_STATE_H

#include <QTreeWidgetItem>
#include "frmMainStateTableWithTree.h"

class frmMainStatistic_State : public frmMainStateTableWithTree
{
    Q_OBJECT

public:
    frmMainStatistic_State(int portfolioID_, const QMap<int, portfolio> &portfolios_, const settings &settings_,
        const QHash<QString, historicalPrices> &prices_, QWidget *parent_);

    ~frmMainStatistic_State();

protected slots:
    void itemChecked(QTreeWidgetItem *item_, int column_);

protected:
    settings::columns columnEnumValue();
    QMap<int, QString> tableColumns();

    mpiViewModelBase* createModel(int beginDate_, int endDate_);
};


#endif // FRMMAINSTATISTIC_STATE_H
