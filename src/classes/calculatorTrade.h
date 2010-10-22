#ifndef NAV_H
#define NAV_H

#include <QMap>
#include "trade.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class portfolio;
class assetAllocation;
class account;
class security;
class calculatorNAV;
class executedTrade;
class calculatorTrade
{
public:
    void run(int beginDate_, const QList<portfolio> &portfolios_);

private:
    typedef QMap<int, trade>::const_iterator tradeIterator;
    typedef QHash<int, QList<tradeIterator> > tradeDay;

    void insertDividendReinvestmentPlaceholders(portfolio portfolio_);
    void removeDividendReinvestmentPlaceholders(portfolio portfolio_);

    void clearExecutedTrades(portfolio portfolio_, int beginDate_, bool recalculateAll_);

    void calculate(portfolio portfolio_, int beginDate_);

    QMap<int, tradeDay> calculateTradeDates(portfolio portfolio_, int date_, bool recalculateAll_) const;

    QList<int> calculateDividendReinvestmentDates(int date_, const QMap<int, double> dividends_) const;

    executedTrade calculateExecutedTrade(int date_, calculatorNAV calc_, const QMap<int, assetAllocation> &aa, const security &parent_,
        const trade &trade_) const;

    double calculateTradePrice(trade::tradeAction type_, double price_, double priorDayPrice_) const;

    double calculateTradeShares(int date_, double price_, calculatorNAV calc_, const QMap<int, assetAllocation> &aa, const security &parent_,
        const trade &trade_) const;

    void insertExecutedTrade(portfolio portfolio_, int id_, int date_, const executedTrade &trade_);

    executedTrade calculateExecutedTradeReversal(const security &security_, int date_, double shares_, double price_, int spawningID_) const;
};

#endif // NAV_H
