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

    executedTrade(const int &date_, const double &shares_, const double &price_, const double &commission_):
        date(date_),
        shares(shares_),
        price(price_),
        commission(commission_)
    {}
};

class executedTradeList: public objectBase
{
public:
    executedTradeList(const int &parent_):
            objectBase(parent_)
    {}

    const QList<executedTrade> executedTrades(const int &securityID) { return m_trades.value(securityID); }

    void insert(const int &securityID, const executedTrade &executedTrade_) { m_trades[securityID].append(executedTrade_); }

    void remove(const queries &dataSource, const int &startDate);
    void remove(const queries &dataSource);

private:
   QMap<int, QList<executedTrade> > m_trades;
};

#endif // EXECUTEDTRADE_H
