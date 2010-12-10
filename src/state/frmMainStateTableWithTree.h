#ifndef FRMMAINSTATETABLEWITHTREE_H
#define FRMMAINSTATETABLEWITHTREE_H

#include "frmMainStateTable.h"
#include "historicalPrices.h"
#include "frmMainStateTree.h"

class QTreeWidgetItem;
class frmMainTableView_UI;
class frmMainStateTableWithTree : public frmMainStateTable, public frmMainStateTree
{
public:
    frmMainStateTableWithTree(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_,
        const QHash<QString, historicalPrices> &prices_, QWidget *parent_);

    virtual ~frmMainStateTableWithTree();

protected:
    QHash<QString, historicalPrices> m_prices;

    virtual void setupUI(bool hasRowLabels_ = false);
    QTreeWidget* treeWidget();

    virtual frmMainTableView_UI* createUI();

    virtual historicalNAV calculateNAV(QTreeWidgetItem *item_, int beginDate_, int endDate_);
};

#endif // FRMMAINSTATETABLEWITHTREE_H
