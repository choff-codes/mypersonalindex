#ifndef NAV_H
#define NAV_H

#include <QMap>
#include "queries.h"
#include "calculations.h"
#include "portfolio.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class nav
{
public:
    nav(queries dataSource_):
        m_dataSource(dataSource_)
    {}

    void run(int date_, const QList<portfolio> portfolios_);

private:
    queries m_dataSource;

    typedef QMap<int, trade>::const_iterator tradePointer;
    typedef QMap<int, QList<tradePointer> > tradePointerMap;

    void insertNAV(portfolio portfolio_, int date_, double endValue_, double nav_);
    void insertExecutedTrade(portfolio portfolio_, int id_, int date_, double shares_, double price_, double commission_);

    void clearHistoricalValues(portfolio portfolio_, int beginDate_, bool recalculateAll_);

    void calculateNAV(portfolio portfolio_, int date_);
    QMap<int, tradePointerMap> calculateExecutedTrades(portfolio portfolio_, int date_, bool recalculateAll_) const;
    double calculateShares(trade::tradeType tradeType_, double value_, double purchasePrice_, int id_, portfolio portfolio_, const snapshotPortfolio &priorDaySnapshot_) const;

    void insertExecutedTrades(portfolio portfolio_, int date_, const snapshotPortfolio &priorDaySnapshot_, const tradePointerMap &trades_);
    void insertExecutedTradesPreStartDate(portfolio portfolio_, int beginDate_, const QMap<int, tradePointerMap> &trades_);
    void insertPortfolioReinvestments(portfolio portfolio_, int date_, const QList<int> &securityReinvestments_, const snapshotPortfolio &priorDaySnapshot_);
    void insertPortfolioCashReversal(portfolio portfolio_, int cashAccountID_, int date_, int priorDate_, double value_);
};

#endif // NAV_H
