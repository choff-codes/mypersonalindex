#ifndef FRMMAINSTATETABLEWITHTREE_H
#define FRMMAINSTATETABLEWITHTREE_H

#include "frmMainStateTable.h"
#include "historicalPrices.h"
#include "frmMainStateTree.h"

class QTreeWidgetItem;
class frmMainTableView_UI;
class frmMainStateTableWithTree : public frmMainStateTable, public frmMainStateTree
{
    Q_OBJECT

public:
    frmMainStateTableWithTree(int portfolioID_, const QMap<int, portfolio> &portfolios_, const settings &settings_,
        const QHash<QString, historicalPrices> &prices_, QWidget *parent_);

    virtual ~frmMainStateTableWithTree();

protected slots:
    void portfolioChange(int index_);
    void clearTree();
    void addAllPortfolios();

protected:
    virtual void setupUI(bool hasRowLabels_ = false);
    QTreeWidget* treeWidget();

    virtual frmMainTableView_UI* createUI();
};

#endif // FRMMAINSTATETABLEWITHTREE_H
