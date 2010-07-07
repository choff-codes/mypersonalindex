#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <qmath.h>
#include <qnumeric.h>
#include <QHash>
#include "snapshot.h"
#include "portfolio.h"
#include "splits.h"
#include "epsilon.h"
#include "avgPricePerShare.h"
#include "tradeDateCalendar.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class calculations
{
public:
    calculations(const portfolio &portfolio_):
            m_portfolio(portfolio_)
    {}

    void clearCache() { m_cache.clear(); }

    snapshotPortfolio portfolioSnapshot(const int &date_, const bool &calcAveragePrices_ = false);
    snapshotSecurity securitySnapshot(const int &date_, const int &id_);
    snapshot assetAllocationSnapshot(const int &date_, const int &id_);
    snapshot accountSnapshot(const int &date_, const int &id_);

    navInfoStatistic changeOverTime(const objectKey &key_, const int &beginDate_, const int &endDate_, const bool &dividends_);
    navInfoStatistic changeOverTime(const int &beginDate_, const int &endDate_); // overload that uses portfolio's NAV values, much faster than recalc'ing

    static double correlation(const navInfoStatistic &first_, const navInfoStatistic &second_);
    static double change(const double &beginValue, const double &endValue_, const double &activity_,
                         const double &dividends_, const double &beginNav_ = 1);

private:
    const m_portfolio;
    QHash<int, snapshotPortfolio> m_cache;

    navInfoStatistic changeOverTime(const QString &symbol_, const int &beginDate_, const int &endDate_, const bool &dividends_);
    snapshot snapshotByKey(const int &date_, const objectKey &key_);
    QMap<int, double> avgPricePerShare(const int &date_);
};


#endif // CALCULATIONS_H
