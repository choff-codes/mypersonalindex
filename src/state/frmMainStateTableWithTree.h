#ifndef FRMMAINSTATETABLEWITHTREE_H
#define FRMMAINSTATETABLEWITHTREE_H

#include "frmMainStateTable.h"
#include "historicalPrices.h"

class QTreeWidgetItem;
class frmMainTableView_UI;
class frmMainStateTableWithTree : public frmMainStateTable
{
public:
    frmMainStateTableWithTree(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_,
        const QHash<QString, historicalPrices> &prices_, QWidget *parent_);

    virtual ~frmMainStateTableWithTree();

protected:
    QHash<QString, historicalPrices> m_prices;

    virtual void setupUI();

    virtual frmMainTableView_UI* createUI();
    virtual QTreeWidgetItem* createTreeItem(int type_, const QString description_) = 0;
    virtual QTreeWidgetItem* createTreeItem(int type_, const QString description_, const QString itemData_) = 0;

};

#endif // FRMMAINSTATETABLEWITHTREE_H
