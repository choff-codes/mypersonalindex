#ifndef NAVTRADE_H
#define NAVTRADE_H

#include <QtCore>
#include "trade.h"

class navTrade
{
public:
    QString symbol;
    int securityID;
    trade singleTrade;

    navTrade(const QString &p_symbol, const int &p_securityID, const trade &p_singleTrade): symbol(p_symbol), securityID(p_securityID), singleTrade(p_singleTrade) {}
};

typedef QList<navTrade> navTradeList;
typedef QMap<int, navTradeList> navTrades;

#endif // NAVTRADE_H
