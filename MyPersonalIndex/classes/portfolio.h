#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QtGui>
#include "navInfo.h"
#include "security.h"
#include "assetAllocation.h"
#include "account.h"

class executedTrade
{
public:
    int date;
    double shares;
    double price;
    double commission;

    executedTrade(): date(0), shares(0), price(0), commission(0) {}
};

class portfolioData
{
public:
    typedef QMap<int, QList<executedTrade> > executedTradeList;

    QMap<int, security> tickers;
    QMap<int, assetAllocation> aa;
    QMap<int, account> acct;
    QList<int> stats;
    executedTradeList executedTrades;
    navInfo nav;
};

class portfolioInfo
{
public:
    enum thesholdMethod { threshold_Portfolio, theshold_AA };
    enum avgPriceCalculation { avgPriceCalculation_FIFO, avgPriceCalculation_LIFO, avgPriceCalculation_AVG };

    int id;
    QString description;
    bool dividends;
    avgPriceCalculation avgPriceCalc;
    int startValue;
    int aaThreshold;
    thesholdMethod aaThresholdMethod;
    int startDate;
    // start date may be updated if it is a non-market day, but the original date also needs to be tracked
    int origStartDate;
    bool holdingsShowHidden;
    bool navSortDesc;
    bool aaShowBlank;
    bool correlationShowHidden;
    bool acctShowBlank;
    QString holdingsSort;
    QString aaSort;
    QString acctSort;

    portfolioInfo(): id(-1), dividends(true), avgPriceCalc(avgPriceCalculation_FIFO), startValue(100),
        aaThreshold(5), aaThresholdMethod(threshold_Portfolio), startDate(QDate::currentDate().toJulianDay()), origStartDate(QDate::currentDate().toJulianDay()),
        holdingsShowHidden (true), navSortDesc(true), aaShowBlank(true), correlationShowHidden(true), acctShowBlank(true) {}

    bool operator==(const portfolioInfo &other) const
    {
        return this->id == other.id
                && this->description == other.description
                && this->dividends == other.dividends
                && this->avgPriceCalc == other.avgPriceCalc
                && this->startValue == other.startValue
                && this->aaThreshold == other.aaThreshold
                && this->aaThresholdMethod == other.aaThresholdMethod
                // exclude startDate
                && this->origStartDate == other.origStartDate
                && this->holdingsShowHidden == other.holdingsShowHidden
                && this->navSortDesc == other.navSortDesc
                && this->aaShowBlank == other.aaShowBlank
                && this->correlationShowHidden == other.correlationShowHidden
                && this->acctShowBlank == other.acctShowBlank
                && this->holdingsSort == other.holdingsSort
                && this->aaSort == other.aaSort
                && this->acctSort == other.acctSort;
    }

    bool operator!=(const portfolioInfo &other) const
    {
        return !(*this == other);
    }
};

class portfolio
{
public:
    portfolioData data;
    portfolioInfo info;

    portfolio(const portfolioInfo &pi): info(pi) {}
};

#endif // PORTFOLIO_H
