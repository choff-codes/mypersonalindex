#ifndef FRMMAINSTATETREE_H
#define FRMMAINSTATETREE_H

#include <QHash>
#include <QMap>
#include "portfolio.h"
#include "historicalPrices.h"

class treeItemKey
{
public:
    objectType type;
    int portfolioID;
    int id;
    QString displayText;
    QString symbol;

    treeItemKey(objectType type_, int parent_, int id_, const QString &displayText_, const QString &symbol_ = QString()):
        type(type_),
        portfolioID(parent_),
        id(id_),
        displayText(displayText_),
        symbol(symbol_)
    {}

    bool operator<(const treeItemKey &other_) const;
    bool operator==(const treeItemKey &other_) const;
};

class QTreeWidget;
class QTreeWidgetItem;
class QString;
class historicalNAV;
class portfolio;
class historicalPrices;
class frmMainStateTree
{
public:
    frmMainStateTree(const QMap<int, portfolio> &portfolios_, const QHash<QString, historicalPrices> &prices_):
        m_portfolios(portfolios_),
        m_prices(prices_)
    {}

    virtual ~frmMainStateTree() {}

protected:
    QList<treeItemKey> m_selectedItems;
    QMap<int, portfolio> m_portfolios;
    QHash<QString, historicalPrices> m_prices;

    virtual QTreeWidget* treeWidget() = 0;
    virtual QTreeWidgetItem* createTreeItem(objectType type_, int portfolioID_, int id_, const QString &description_, const QString &itemData_ = QString());
    treeItemKey createKeyFromTreeItem(QTreeWidgetItem *item_);

    virtual void populateTree(int portfolioID_);
    virtual historicalNAV calculateNAV(const treeItemKey &item_, int beginDate_, int endDate_);
};

#endif // FRMMAINSTATETREE_H
