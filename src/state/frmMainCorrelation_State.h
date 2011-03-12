#ifndef FRMMAINCORRELATION_STATE_H
#define FRMMAINCORRELATION_STATE_H

#include <QTreeWidgetItem>
#include "frmMainStateTableWithTree.h"

class frmMainCorrelation_State : public frmMainStateTableWithTree
{
    Q_OBJECT

public:
    frmMainCorrelation_State(int portfolioID_, const QMap<int, portfolio> &portfolios_, const settings &settings_,
        const QHash<QString, historicalPrices> &prices_, QWidget *parent_);

    ~frmMainCorrelation_State();

protected slots:
    void itemChecked(QTreeWidgetItem *item_, int column_);

protected:
    settings::columns columnEnumValue();
    QMap<int, QString> tableColumns();

    mpiViewModelBase* createModel(int beginDate_, int endDate_);
};


#endif // FRMMAINCORRELATION_STATE_H
