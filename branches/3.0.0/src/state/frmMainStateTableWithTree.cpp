#include "frmMainStateTableWithTree.h"
#include "frmMainTableViewTree_UI.h"
#include "account.h"
#include "assetAllocation.h"
#include "security.h"
#include "symbol.h"
#include "historicalNAV.h"

frmMainStateTableWithTree::frmMainStateTableWithTree(int portfolioID_, const QMap<int, portfolio> &portfolios_, const settings &settings_,
    const QHash<QString, historicalPrices> &prices_, QWidget *parent_):
    frmMainStateTable(portfolios_.value(portfolioID_), settings_, parent_),
    frmMainStateTree(portfolios_, prices_)
{
}

frmMainStateTableWithTree::~frmMainStateTableWithTree()
{
}

void frmMainStateTableWithTree::setupUI(bool hasRowLabels_)
{
    frmMainStateTable::setupUI(hasRowLabels_);
    QComboBox* treeCmb = static_cast<frmMainTableViewTree_UI*>(ui)->treeCmb;
    foreach(const portfolio &p, m_portfolios)
        treeCmb->addItem(p.displayText(), p.id());
    treeCmb->setCurrentIndex(treeCmb->findData(m_portfolio.id()));
    populateTree(m_portfolio.id());
    connect(treeCmb, SIGNAL(currentIndexChanged(int)), SLOT(portfolioChange(int)));
}

void frmMainStateTableWithTree::portfolioChange(int index_)
{
    populateTree(static_cast<frmMainTableViewTree_UI*>(ui)->treeCmb->itemData(index_).toInt());
}

frmMainTableView_UI* frmMainStateTableWithTree::createUI()
{
    return new frmMainTableViewTree_UI();
}

QTreeWidget* frmMainStateTableWithTree::treeWidget()
{
    return static_cast<frmMainTableViewTree_UI*>(ui)->tree;
}
