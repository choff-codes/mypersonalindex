#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QtGui>
#include "navInfo.h"
#include "security.h"
#include "assetAllocation.h"
#include "account.h"
#include "queries.h"
#include "prices.h"

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

class portfolioData
{
public:
    QMap<int, security> securities;
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
    bool holdingsShowHidden;
    bool navSortDesc;
    bool aaShowBlank;
    bool correlationShowHidden;
    bool acctShowBlank;
    QString holdingsSort;
    QString aaSort;
    QString acctSort;

    portfolioInfo(): id(-1), dividends(true), avgPriceCalc(avgPriceCalculation_FIFO), startValue(100),
        aaThreshold(5), aaThresholdMethod(threshold_Portfolio), startDate(QDate::currentDate().toJulianDay()),
        holdingsShowHidden (true), navSortDesc(true), aaShowBlank(true), correlationShowHidden(true), acctShowBlank(true) {}

    void save();

    bool operator==(const portfolioInfo &other) const
    {
        return this->id == other.id
                && this->description == other.description
                && this->dividends == other.dividends
                && this->avgPriceCalc == other.avgPriceCalc
                && this->startValue == other.startValue
                && this->aaThreshold == other.aaThreshold
                && this->aaThresholdMethod == other.aaThresholdMethod
                && this->startDate == other.startDate
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

    void remove() const;

    static QMap<int, portfolio*> loadPortfolios();

private:
    static void loadPortfoliosInfo(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q);
    static void loadPortfoliosSecurity(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q);
    static void loadPortfoliosSecurityAA(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q);
    static void loadPortfoliosSecurityTrades(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q);
    static void loadPortfoliosExecutedTrades(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q);
    static void loadPortfoliosAA(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q);
    static void loadPortfoliosAcct(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q);
    static void loadPortfoliosStat(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q);
    static void loadPortfoliosNAV(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q);
};

#endif // PORTFOLIO_H
