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

    typedef QMap<int, trade>::const_iterator navTradePointer;
    typedef QMap<int, QList<navTradePointer> > navTradeList;

    void insertNAV(portfolio portfolio_, int date_, double endValue_, double nav_);
    void insertExecutedTrade(portfolio portfolio_, int id_, int date_, double shares_, double price_, double commission_);

    void removeHistoricalValues(portfolio portfolio_, int beginDate_, bool recalculateAll_);
    void calculateNAVValues(portfolio portfolio_, int date_);

    const QMap<int, navTradeList> calculateExecutedTrades(portfolio portfolio_, int date_, bool recalculateAll_);
    void insertExecutedTrades(portfolio portfolio_, int date_, const snapshotPortfolio &priorDaySnapshot_, const navTradeList &trades_);
    void insertExecutedTradesPreStartDate(portfolio portfolio_, int &beginDate_, const QMap<int, navTradeList> &trades_);
    void insertPortfolioReinvestments(portfolio portfolio_, int date_, const QList<int> &securityReinvestments_, const snapshotPortfolio &priorDaySnapshot_);
    void insertPortfolioCashTrade(portfolio portfolio_, int cashAccountID_, int date_, int priorDate_, double &value_);
};

#endif // NAV_H
