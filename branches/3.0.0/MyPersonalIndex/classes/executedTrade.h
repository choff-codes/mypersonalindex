#ifndef EXECUTEDTRADE_H
#define EXECUTEDTRADE_H

#include <QMap>
#include <QPair>
#include "queries.h"
#include "objectBase.h"

class executedTrade
{
public:

    double shares;
    double price;
    double commission;

    executedTrade(double shares_, double price_, double commission_):
        shares(shares_),
        price(price_),
        commission(commission_)
    {}
};

class executedTradeList: public objectBase, public queriesBatch
{
public:
    executedTradeList(int parent_):
        objectBase(parent_)
    {}

    QMap<int, executedTrade>::const_iterator constBegin() const { return m_trades.constBegin(); }
    QMap<int, executedTrade>::const_iterator constEnd() const { return m_trades.constEnd(); }

    void insert(int date_, const executedTrade &executedTrade_) { m_trades.insertMulti(date_, executedTrade_); }
    void insert(const queries &dataSource_, int date_, const executedTrade &executedTrade_);

    void remove(const queries &dataSource_);
    void remove(const queries &dataSource_, int beginDate_);

    void insertBatch(queries dataSource_);

    int rowsToBeInserted() const { return m_valuesToBeInserted.count(); }
    QVariant data(int row_, int column_) const;

private:
   QMap<int, executedTrade> m_trades;
   QList<QMap<int, executedTrade>::iterator> m_valuesToBeInserted;
};

#endif // EXECUTEDTRADE_H
