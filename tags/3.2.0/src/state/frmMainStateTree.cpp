#include "frmMainStateTree.h"
#include <QFont>
#include <QTreeWidget>
#include "portfolio.h"
#include "account.h"
#include "assetAllocation.h"
#include "security.h"
#include "symbol.h"
#include "historicalNAV.h"
#include "calculatorNAV.h"
#include "historicalPrices.h"
#include "snapshot.h"

bool treeItemKey::operator<(const treeItemKey &other_) const
{
    if (type != other_.type)
        return type < other_.type;
    if (portfolioID != other_.portfolioID)
        return portfolioID < other_.portfolioID;
    if (id != other_.id)
        return id < other_.id;
    return symbol < other_.symbol;
}

bool treeItemKey::operator==(const treeItemKey &other_) const
{
    return type == other_.type && portfolioID == other_.portfolioID && id == other_.id && symbol == other_.symbol;
}

void frmMainStateTree::populateTree(int portfolioID_)
{
    portfolio p = m_portfolios.value(portfolioID_);
    QTreeWidget *tree = treeWidget();
    bool first = tree->topLevelItemCount() == 0;
    tree->clear();
    tree->blockSignals(true);

    QList<QTreeWidgetItem*> items;
    items << createTreeItem(objectType_Portfolio, UNASSIGNED, objectType_Portfolio, "Portfolio")
          << createTreeItem(objectType_Account, UNASSIGNED, objectType_Account, "Accounts")
          << createTreeItem(objectType_AA, UNASSIGNED, objectType_AA, "Asset Classes")
          << createTreeItem(objectType_Security, UNASSIGNED, objectType_Security, "Securities")
          << createTreeItem(objectType_Symbol, UNASSIGNED, objectType_Symbol, "Symbols");

    foreach(QTreeWidgetItem* item, items)
    {
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0, font);
        item->setFlags(item->flags() | Qt::ItemIsTristate);
    }

    tree->insertTopLevelItems(0, items);

    // when this is first created, add the portfolio checked by default
    if (first)
        m_selectedItems.append(treeItemKey(objectType_Portfolio, p.id(), p.id(), p.displayText()));
    items.at(0)->addChild(createTreeItem(objectType_Portfolio, p.id(), p.id(), p.displayText()));

    foreach(const account &acct, p.accounts())
        items.at(1)->addChild(createTreeItem(objectType_Account, p.id(), acct.id(), acct.displayText()));

    foreach(const assetAllocation &aa, p.assetAllocations())
        items.at(2)->addChild(createTreeItem(objectType_AA, p.id(), aa.id(), aa.displayText()));

    QSet<QString> symbolsWithDividends;
    QSet<QString> symbolsWithoutDividends;

    foreach(const security &sec, p.securities())
    {
        items.at(3)->addChild(createTreeItem(objectType_Security, p.id(), sec.id(), sec.displayText()));

        if (sec.cashAccount())
            continue;

        if (sec.dividendNAVAdjustment())
            symbolsWithDividends.insert(sec.description());
        else
            symbolsWithoutDividends.insert(sec.description());
    }

    foreach(const QString &sym, symbolsWithoutDividends)
        items.at(4)->addChild(createTreeItem(objectType_Symbol, p.id(), 0, QString("%1 (ex. dividends)").arg(sym), sym));

    foreach(const QString &sym, symbolsWithDividends)
        items.at(4)->addChild(createTreeItem(objectType_Symbol, p.id(), 1, sym, sym));

    foreach(QTreeWidgetItem* item, items)
    {
        item->setExpanded(true);
        item->sortChildren(0, Qt::AscendingOrder);
    }

    tree->blockSignals(false);
}

QTreeWidgetItem* frmMainStateTree::createTreeItem(objectType type_, int portfolioID_, int id_, const QString &description_, const QString &itemData_)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << description_, id_);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    if (!itemData_.isEmpty())
        item->setData(0, Qt::UserRole, itemData_);

    if (m_selectedItems.contains(treeItemKey(type_, portfolioID_, id_, description_, itemData_)))
        item->setCheckState(0, Qt::Checked);
    else
        item->setCheckState(0, Qt::Unchecked);

    return item;
}

historicalNAV frmMainStateTree::calculateNAV(const treeItemKey &item_, int beginDate_, int endDate_)
{
    if (item_.id == UNASSIGNED)
        return historicalNAV();

    portfolio p = m_portfolios.value(item_.portfolioID);

    switch(item_.type)
    {
        case objectType_Portfolio:
            return p.changeOverTime(p, beginDate_, endDate_);
        case objectType_Account:
            return p.changeOverTime(p.accounts().value(item_.id), beginDate_, endDate_);
        case objectType_AA:
            return p.changeOverTime(p.assetAllocations().value(item_.id), beginDate_, endDate_);
        case objectType_Security:
            return p.changeOverTime(p.securities().value(item_.id), beginDate_, endDate_);
        case objectType_Symbol:
        {
            symbol s;
            s.setDescription(item_.symbol);
            s.setIncludeDividends(item_.id == 1);
            s.setHistoricalPrices(m_prices.value(s.description()));
            return p.changeOverTime(s, beginDate_, endDate_);
        }
        case objectType_Trade:
            break;
    }
    return historicalNAV();
}

treeItemKey frmMainStateTree::createKeyFromTreeItem(QTreeWidgetItem *item_)
{
    if (!item_->parent())
        return treeItemKey(objectType_Portfolio, UNASSIGNED, UNASSIGNED, QString());

    return treeItemKey((objectType)item_->parent()->type(), treeWidget()->topLevelItem(0)->child(0)->type(),
        item_->type(), item_->text(0), item_->data(0, Qt::UserRole).toString());
}
