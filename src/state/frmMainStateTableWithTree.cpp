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

    QTreeWidget *tree = static_cast<frmMainTableViewTree_UI*>(ui)->tree;

    QList<QTreeWidgetItem*> items;
    items << createTreeItem(objectType_Portfolio, "Portfolio")
          << createTreeItem(objectType_Account, "Accounts")
          << createTreeItem(objectType_AA, "Asset Classes")
          << createTreeItem(objectType_Security, "Securities")
          << createTreeItem(objectType_Symbol, "Symbols");

    foreach(QTreeWidgetItem* item, items)
    {
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0, font);
        item->setFlags(item->flags() | Qt::ItemIsTristate);
    }

    tree->insertTopLevelItems(0, items);

    items.at(0)->addChild(createTreeItem(m_portfolio.id(), m_portfolio.displayText()));

    foreach(const account &acct, m_portfolio.accounts())
    {
        if (acct.deleted())
            continue;
        items.at(1)->addChild(createTreeItem(acct.id(), acct.displayText()));
    }

    foreach(const assetAllocation &aa, m_portfolio.assetAllocations())
    {
        if (aa.deleted())
            continue;
        items.at(2)->addChild(createTreeItem(aa.id(), aa.displayText()));
    }

    QSet<QString> symbolsWithDividends;
    QSet<QString> symbolsWithoutDividends;

    foreach(const security &sec, m_portfolio.securities())
    {
        if (sec.deleted())
            continue;

        items.at(3)->addChild(createTreeItem(sec.id(), sec.displayText()));

        if (sec.cashAccount())
            continue;

        if (sec.dividendNAVAdjustment())
            symbolsWithDividends.insert(sec.description());
        else
            symbolsWithoutDividends.insert(sec.description());
    }

    foreach(const QString &sym, symbolsWithoutDividends)
        items.at(4)->addChild(createTreeItem(0, QString("%1 (ex. dividends)").arg(sym), sym));

    foreach(const QString &sym, symbolsWithDividends)
        items.at(4)->addChild(createTreeItem(1, sym, sym));

    foreach(QTreeWidgetItem* item, items)
    {
        item->setExpanded(true);
        item->sortChildren(0, Qt::AscendingOrder);
    }

    connect(tree, SIGNAL(itemSelectionChanged()), this, SLOT(refreshTab()));
}

frmMainTableView_UI* frmMainStateTableWithTree::createUI()
{
    return new frmMainTableViewTree_UI();
}

historicalNAV frmMainStateTableWithTree::calculateNAV(QTreeWidgetItem *item_, int beginDate_, int endDate_)
{
    if (!item_->parent())
        return historicalNAV();

    switch((objectType)item_->parent()->type())
    {
        case objectType_Portfolio:
            return m_calculator.changeOverTime(m_portfolio, beginDate_, endDate_);
        case objectType_Account:
            return m_calculator.changeOverTime(m_portfolio.accounts().value(item_->type()), beginDate_, endDate_);
        case objectType_AA:
            return m_calculator.changeOverTime(m_portfolio.assetAllocations().value(item_->type()), beginDate_, endDate_);
        case objectType_Security:
            return m_calculator.changeOverTime(m_portfolio.securities().value(item_->type()), beginDate_, endDate_);
        case objectType_Symbol:
        {
            symbol s(item_->data(0, Qt::UserRole).toString(), item_->type() == 1);
            s.setHistoricalPrices(m_prices.value(s.description()));
            return m_calculator.changeOverTime(s, beginDate_, endDate_);
        }
        case objectType_Trade:
            break;
    }
    return historicalNAV();
}

QList<QTreeWidgetItem*> frmMainStateTableWithTree::selectedItems()
{
    QList<QTreeWidgetItem*> items;
    QTreeWidget *tree = static_cast<frmMainTableViewTree_UI*>(ui)->tree;

    for(int i = 0; i < tree->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *parent = tree->topLevelItem(i);
        for(int x = 0; x < parent->childCount(); ++x)
        {
            QTreeWidgetItem *item = parent->child(x);
            if (item->checkState(0) == Qt::Checked)
                items.append(item);
        }
    }
    return items;
}
