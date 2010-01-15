#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QtGui>
#include "executedTrade.h"
#include "navInfo.h"
#include "security.h"
#include "assetAllocation.h"
#include "account.h"
#include "queries.h"
#include "prices.h"

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

    portfolioInfo();
    void save();

    bool operator==(const portfolioInfo &other) const;
    bool operator!=(const portfolioInfo &other) const { return !(*this == other); }
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
