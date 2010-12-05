#include "frmMainStateTableWithTree.h"
#include "frmMainTableViewTree_UI.h"
#include "account.h"
#include "assetAllocation.h"
#include "security.h"
#include "symbol.h"
#include "historicalNAV.h"

frmMainStateTableWithTree::frmMainStateTableWithTree(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_,
    const QHash<QString, historicalPrices> &prices_, QWidget *parent_):
    frmMainStateTable(portfolio_, calculator_, settings_, parent_),
    m_prices(prices_)
{
}

frmMainStateTableWithTree::~frmMainStateTableWithTree()
{
}

void frmMainStateTableWithTree::setupUI(bool hasRowLabels_)
{
    frmMainStateTable::setupUI(hasRowLabels_);
    populateTree(m_portfolio);
    connect(treeWidget(), SIGNAL(itemSelectionChanged()), this, SLOT(refreshTab()));
}

frmMainTableView_UI* frmMainStateTableWithTree::createUI()
{
    return new frmMainTableViewTree_UI();
}

QTreeWidget* frmMainStateTableWithTree::treeWidget()
{
    return static_cast<frmMainTableViewTree_UI*>(ui)->tree;
}

historicalNAV frmMainStateTableWithTree::calculateNAV(QTreeWidgetItem *item_, int beginDate_, int endDate_)
{
    return frmMainStateTree::calculateNAV(item_, beginDate_, endDate_, m_portfolio, m_calculator, m_prices);
}
