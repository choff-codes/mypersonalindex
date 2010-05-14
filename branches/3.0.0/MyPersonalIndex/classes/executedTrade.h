#ifndef EXECUTEDTRADE_H
#define EXECUTEDTRADE_H

#include <QMap>
#include "queries.h"

class executedTrade
{
public:
    int date;
    double shares;
    double price;
    double commission;

    executedTrade(): date(0), shares(0), price(0), commission(0) {}
    executedTrade(const int &p_date, const double &p_shares, const double &p_price, const double &p_commission):
        date(p_date), shares(p_shares), price(p_price), commission(p_commission) {}
};

class executedTradeList: public QMap<int, QList<executedTrade> >
{
public:
    void remove(const int &portfolioID, const int &startDate);
    void remove(const int &portfolioID);
};

#endif // EXECUTEDTRADE_H
