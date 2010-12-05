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

void frmMainStateTree::populateTree(const portfolio &portfolio_)
{
    QTreeWidget *tree = treeWidget();

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

    items.at(0)->addChild(createTreeItem(portfolio_.id(), portfolio_.displayText()));

    foreach(const account &acct, portfolio_.accounts())
    {
        if (acct.deleted())
            continue;
        items.at(1)->addChild(createTreeItem(acct.id(), acct.displayText()));
    }

    foreach(const assetAllocation &aa, portfolio_.assetAllocations())
    {
        if (aa.deleted())
            continue;
        items.at(2)->addChild(createTreeItem(aa.id(), aa.displayText()));
    }

    QSet<QString> symbolsWithDividends;
    QSet<QString> symbolsWithoutDividends;

    foreach(const security &sec, portfolio_.securities())
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
}

historicalNAV frmMainStateTree::calculateNAV(QTreeWidgetItem *item_, int beginDate_, int endDate_, const portfolio &portfolio_,
    calculatorNAV calculator_, const QHash<QString, historicalPrices> prices_)
{
    if (!item_->parent())
        return historicalNAV();

    switch((objectType)item_->parent()->type())
    {
        case objectType_Portfolio:
            return calculator_.changeOverTime(portfolio_, beginDate_, endDate_);
        case objectType_Account:
            return calculator_.changeOverTime(portfolio_.accounts().value(item_->type()), beginDate_, endDate_);
        case objectType_AA:
            return calculator_.changeOverTime(portfolio_.assetAllocations().value(item_->type()), beginDate_, endDate_);
        case objectType_Security:
            return calculator_.changeOverTime(portfolio_.securities().value(item_->type()), beginDate_, endDate_);
        case objectType_Symbol:
        {
            symbol s(item_->data(0, Qt::UserRole).toString(), item_->type() == 1);
            s.setHistoricalPrices(prices_.value(s.description()));
            return calculator_.changeOverTime(s, beginDate_, endDate_);
        }
        case objectType_Trade:
            break;
    }
    return historicalNAV();
}

QList<QTreeWidgetItem*> frmMainStateTree::selectedItems()
{
    QList<QTreeWidgetItem*> items;
    QTreeWidget *tree = treeWidget();

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
