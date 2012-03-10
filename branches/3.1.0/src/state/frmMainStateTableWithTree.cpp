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
    QPushButton* treeAllPortfolios = static_cast<frmMainTableViewTree_UI*>(ui)->treeAllPortfolios;
    QPushButton* treeClearBtn = static_cast<frmMainTableViewTree_UI*>(ui)->treeClearBtn;
    foreach(const portfolio &p, m_portfolios)
        treeCmb->addItem(p.displayText(), p.id());
    treeCmb->setCurrentIndex(treeCmb->findData(m_portfolio.id()));
    populateTree(m_portfolio.id());
    connect(treeCmb, SIGNAL(currentIndexChanged(int)), SLOT(portfolioChange(int)));
    connect(treeAllPortfolios, SIGNAL(clicked()), this, SLOT(addAllPortfolios()));
    connect(treeClearBtn, SIGNAL(clicked()), this, SLOT(clearTree()));
}

void frmMainStateTableWithTree::clearTree()
{
    treeWidget()->blockSignals(true);
    for(int i = 0; i < treeWidget()->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = treeWidget()->topLevelItem(i);
        for(int x = 0; x < item->childCount(); ++x)
            if (item->child(x)->checkState(0) == Qt::Checked)
                item->child(x)->setCheckState(0, Qt::Unchecked);
    }
    m_selectedItems.clear();
    treeWidget()->blockSignals(false);
    refreshTab();
}

void frmMainStateTableWithTree::addAllPortfolios()
{
    QTreeWidgetItem *item = treeWidget()->topLevelItem(0);
    item->setCheckState(0, Qt::Checked);
    item->child(0)->setCheckState(0, Qt::Checked);
    foreach(portfolio p, m_portfolios)
    {
        treeItemKey t(objectType_Portfolio, p.id(), p.id(), p.displayText());
        if (!m_selectedItems.contains(t))
            m_selectedItems.append(t);
    }
    refreshTab();
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
