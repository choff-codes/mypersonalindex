#ifndef FRMMAINCORRELATION_STATE_H
#define FRMMAINCORRELATION_STATE_H

#include <QTreeWidgetItem>
#include "frmMainStateTableWithTree.h"

class frmMainCorrelation_State : public frmMainStateTableWithTree
{
    Q_OBJECT

public:
    frmMainCorrelation_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_,
        const QHash<QString, historicalPrices> &prices_, QWidget *parent_);

    ~frmMainCorrelation_State();

protected slots:
    void itemChecked(QTreeWidgetItem *item_, int column_);

protected:
    settings::columns columnEnumValue();
    QMap<int, QString> tableColumns();

    mpiViewModelBase* createModel(int beginDate_, int endDate_);
    QTreeWidgetItem* createTreeItem(int type_, const QString &description_);
    QTreeWidgetItem* createTreeItem(int type_, const QString &description_, const QString &itemData_);

    historicalNAV calculateNAV(QTreeWidgetItem *item_, int beginDate_, int endDate_);

private:
    QMap<QTreeWidgetItem*, historicalNAV> m_cache;
};


#endif // FRMMAINCORRELATION_STATE_H
