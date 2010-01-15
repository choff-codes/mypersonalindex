#ifndef EXECUTEDTRADE_H
#define EXECUTEDTRADE_H

#include <QtCore>
#include "queries.h"

class executedTrade
{
public:
    int date;
    double shares;
    double price;
    double commission;

    executedTrade(): date(0), shares(0), price(0), commission(0) {}
};

class executedTradeList: public QMap<int, QList<executedTrade> >
{
public:
    void remove(const int &portfolioID, const int &startDate);
    void remove(const int &portfolioID);
};

#endif // EXECUTEDTRADE_H
