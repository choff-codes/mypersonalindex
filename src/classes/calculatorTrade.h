#ifndef NAV_H
#define NAV_H

#include <QMap>
#include "trade.h"

class portfolio;
class assetAllocation;
class account;
class security;
class calculatorNAV;
class executedTrade;
class calculatorTrade
{
public:
    calculatorTrade(int beginDate_):
        m_beginDate(beginDate_)
    {}

    typedef bool result_type;

    result_type operator()(const portfolio &portfolio_);

private:
    typedef QList<trade> tradeList;
    typedef QMap<int, tradeList> tradeListBySecurity;
    typedef QMap<int, tradeListBySecurity> tradeMapByDate;

    int m_beginDate;

    void insertDividendReinvestmentPlaceholders(portfolio portfolio_);
    void removeDividendReinvestmentPlaceholders(portfolio portfolio_);

    void clearExecutedTrades(portfolio portfolio_, int beginDate_, bool recalculateAll_);

    void calculate(portfolio portfolio_, int beginDate_);

    tradeMapByDate calculateTradeDates(portfolio portfolio_, int date_, bool recalculateAll_) const;
    QList<int> calculateDividendReinvestmentDates(int date_, const QMap<int, double> dividends_) const;

    executedTrade calculateExecutedTrade(int date_, const calculatorNAV &calc_, const QMap<int, assetAllocation> &aa, const security &parent_,
        const trade &trade_) const;

    executedTrade calculateExecutedTradeReversal(const security &security_, int date_, double shares_, double price_, int spawningID_) const;

    double calculateTradePrice(trade::tradeAction type_, double price_) const;

    double calculateTradeShares(int date_, double price_, calculatorNAV calc_, const QMap<int, assetAllocation> &aa, const security &parent_,
        const trade &trade_) const;

    void insertExecutedTrade(security security_, int date_, const executedTrade &trade_);
};

#endif // NAV_H
