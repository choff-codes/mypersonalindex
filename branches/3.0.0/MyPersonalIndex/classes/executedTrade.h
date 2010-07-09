#ifndef EXECUTEDTRADE_H
#define EXECUTEDTRADE_H

#include <QMap>
#include "queries.h"
#include "objectBase.h"

class executedTrade
{
public:
    int date;
    double shares;
    double price;
    double commission;

    executedTrade(int date_, double shares_, double price_, double commission_):
        date(date_),
        shares(shares_),
        price(price_),
        commission(commission_)
    {}
};

class executedTradeList: public objectBase
{
public:
    executedTradeList(int parent_):
        objectBase(parent_)
    {}

    const QList<executedTrade> executedTrades(int id_) const { return m_trades.value(id_); }

    QMap<int, QList<executedTrade> >::const_iterator constBegin() const { return m_trades.constBegin(); }
    QMap<int, QList<executedTrade> >::const_iterator constEnd() const { return m_trades.constEnd(); }

    void insert(int id_, const executedTrade &executedTrade_) { m_trades[id_].append(executedTrade_); }

    void remove(const queries &dataSource_, int beginDate_);
    void remove(const queries &dataSource_);

private:
   QMap<int, QList<executedTrade> > m_trades;
};

#endif // EXECUTEDTRADE_H
