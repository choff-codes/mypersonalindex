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

    static QMap<int, portfolio*> loadPortfolios();

private:
    enum { getPortfolio_PortfolioID, getPortfolio_Description, getPortfolio_Dividends, getPortfolio_StartValue,
           getPortfolio_CostCalc, getPortfolio_AAThreshold, getPortfolio_AAThresholdMethod,
           getPortfolio_StartDate, getPortfolio_HoldingsShowHidden, getPortfolio_HoldingsSort,
           getPortfolio_NAVSortDesc, getPortfolio_AASort, getPortfolio_AAShowBlank,
           getPortfolio_CorrelationShowHidden, getPortfolio_AcctSort, getPortfolio_AcctShowBlank };
    static QString getPortfolio();

    enum { getStatMapping_PortfolioID, getStatMapping_StatID };
    static QString getStatMapping();

    enum { getNAV_Date, getNAV_PortfolioID, getNAV_NAV, getNAV_TotalValue };
    static QString getNAV();

    enum { getAA_ID, getAA_PortfolioID, getAA_Description, getAA_Target };
    static QString getAA();

    enum { getAcct_ID, getAcct_PortfolioID, getAcct_Description, getAcct_TaxRate, getAcct_TaxDeferred };
    static QString getAcct();

    enum { getSecurity_ID, getSecurity_PortfolioID, getSecurity_Ticker, getSecurity_Account,
           getSecurity_Expense, getSecurity_DivReinvest, getSecurity_CashAccount,
           getSecurity_IncludeInCalc, getSecurity_Hide };
    static QString getSecurity();

    enum { getSecurityTrade_ID, getSecurityTrade_PortfolioID, getSecurityTrade_TickerID, getSecurityTrade_Type,
           getSecurityTrade_Value, getSecurityTrade_Price, getSecurityTrade_Commission, getSecurityTrade_CashAccountID,
           getSecurityTrade_Frequency, getSecurityTrade_Date, getSecurityTrade_StartDate, getSecurityTrade_EndDate };
    static QString getSecurityTrade();

    enum { getSecurityAA_PortfolioID, getSecurityAA_TickerID, getSecurityAA_AAID, getSecurityAA_Percent };
    static QString getSecurityAA();

    enum { getTrade_PortfolioID, getTrade_TickerID, getTrade_Date, getTrade_Shares, getTrade_Price, getTrade_Commission };
    static QString getTrade();

    static void loadPortfoliosInfo(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q);
    static void loadPortfoliosTickers(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q);
    static void loadPortfoliosTickersAA(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q);
    static void loadPortfoliosTickersTrades(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q);
    static void loadPortfoliosExecutedTrades(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q);
    static void loadPortfoliosAA(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q);
    static void loadPortfoliosAcct(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q);
    static void loadPortfoliosStat(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q);
    static void loadPortfoliosNAV(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q);
};

#endif // PORTFOLIO_H
