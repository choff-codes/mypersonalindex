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

    void run(int date_, const QList<portfolio> &portfolios_);

private:
    struct tradePointerWithDates
    {
        tradePointerWithDates(const QMap<int, security>::iterator &parent_, const QMap<int, trade>::iterator &trade_, const QList<int> &dates_):
            dates(dates_),
            parentIterator(parent_),
            tradeIterator(trade_)
        {}

        bool operator==(const tradePointerWithDates &other_) const { return this->tradeIterator == other_.tradeIterator; }

        // the dates must be always be inserted in sorted order
        QList<int> dates;
        QMap<int, security>::iterator parentIterator;
        QMap<int, trade>::iterator tradeIterator;

        security& parent() const { return parentIterator.value(); }
        trade& value() const { return tradeIterator.value(); }
    };

    queries m_dataSource;

    void clearHistoricalValues(portfolio portfolio_, int beginDate_, bool recalculateAll_);

    void calculateNAV(portfolio portfolio_, int date_);
    QList<tradePointerWithDates> calculateTradesWithDates(portfolio portfolio_, int date_, bool recalculateAll_) const;
    double calculateShares(const tradePointerWithDates &trade_, double purchasePrice_, portfolio portfolio_,
        const snapshotPortfolio &priorDaySnapshot_) const;

    void insertExecutedTrades(portfolio portfolio_, int date_, const snapshotPortfolio &priorDaySnapshot_, const QList<tradePointerWithDates> &trades_);
    void insertExecutedTradesPreStartDate(portfolio portfolio_, int beginDate_, const QList<tradePointerWithDates> &trades_);
    void insertPortfolioReversal(portfolio portfolio_, int cashAccountID_, int date_, int priorDate_, double value_);
};

#endif // NAV_H
