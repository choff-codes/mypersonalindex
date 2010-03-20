#ifndef NAV_H
#define NAV_H

#include <QtCore>
#include "queries.h"
#include "calculations.h"
#include "prices.h"
#include "portfolio.h"

class nav
{
public:
    nav(const int &calculationDate, const int &portfolioID = -1):
        m_dates(prices::instance().dates()), m_calculationDate(calculationDate), m_portfolioID(portfolioID) {}

    void run();

private:
    const QList<int> m_dates;
    int m_calculationDate;
    int m_portfolioID;
    QVariantList m_NAV_Portfolio, m_NAV_Dates, m_NAV_Totalvalue, m_NAV_Nav;
    QVariantList m_ExecutedTrades_SecurityID, m_ExecutedTrades_Dates, m_ExecutedTrades_Shares, m_ExecutedTrades_Price, m_ExecutedTrades_Commission;
    calculations m_calculations;

    typedef QMap<int, trade>::const_iterator navTradePointer;
    typedef QMap<int, QList<navTradePointer> > navTradeList;

    navTradeList appendNavTradeLists(const navTradeList &first, const navTradeList &second);
    void addToNAVList(const int &portfolioID, const int &date, const double &totalValue, const double &nav);
    void addToExecutedTradeList(const int &portfolioID, const int &securityID, const int &date, const double &shares, const double &price, const double &commission);
    void clearVariantLists();
    void insertVariantLists();
    void deleteOldValues(const int &portfolioID, const int &calculationDate, const bool &calculateFromStartDate);
    int checkCalculationDate(const int &portfolioID, int calculationDate, bool *calcuateFromStartDate);
    void calculateNAVValues(const int &portfolioID);
    bool getCurrentDateOrNext(int *date) const;
    QMap<int, navTradeList> calculateExecutedTrades(const int &portfolioID, const int &calculationDate, const bool &calculateFromStartDate);
    QList<int> getPortfolioSecurityReinvestment(const int &portfolioID);
    QList<int> computeDailyTrade(const int &minDate, const int &maxDate) const;
    QList<int> computeOnceTrades(const trade &singleTade, const int &minDate, const int &maxDate, const bool &calculateFromStartDate) const;
    QList<int> computeWeeklyTrades(const int &tradeDate, const int &minDate, const int &maxDate) const;
    QList<int> computeMonthlyTrades(const int &tradeDate, const int &minDate, const int &maxDate) const;
    QList<int> computeYearlyTrades(const int &tradeDate, const int &minDate, const int &maxDate) const;
    void insertPortfolioReinvestments(const int &portfolioID, const int &date, const QList<int> &securityReinvestments, const dailyInfoPortfolio *previousInfo);
    void insertPortfolioCashTrade(const int &portfolioID, const int &cashAccount, const dailyInfoPortfolio *previousInfo, const int &date, const double &reverseTradeValue);
    void insertPortfolioTrades(const int &portfolioID, const int &date, const dailyInfoPortfolio *previousInfo, const navTradeList &trades);
    void insertFirstPortfolioTrades(const int &portfolioID, const int &startDate, const QMap<int, navTradeList> &allTrades);
};

#endif // NAV_H
