#ifndef NAV_H
#define NAV_H

#include <QMap>
#include "queries.h"
#include "calculations.h"
#include "prices.h"
#include "portfolio.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class nav
{
public:
    nav(const int &calculationDate, const int &portfolioID = -1): m_calculationDate(calculationDate), m_portfolioID(portfolioID) {}
    void run();

private:
    int m_calculationDate;
    int m_portfolioID;
    QVariantList m_NAV_Portfolio, m_NAV_Dates, m_NAV_Totalvalue, m_NAV_Nav;
    QVariantList m_ExecutedTrades_SecurityID, m_ExecutedTrades_Dates, m_ExecutedTrades_Shares, m_ExecutedTrades_Price, m_ExecutedTrades_Commission;

    typedef QMap<int, trade>::const_iterator navTradePointer;
    typedef QMap<int, QList<navTradePointer> > navTradeList;

    navTradeList appendNavTradeLists(const navTradeList &first, const navTradeList &second);
    void addToNAVList(const int &portfolioID, const int &date, const double &totalValue, const double &nav);
    void addToExecutedTradeList(const int &portfolioID, const int &securityID, const int &date, const double &shares, const double &price, const double &commission);

    void clearNAVValues();
    void insertNAVValues();
    void deleteNAVValues(const int &portfolioID, const int &calculationDate, const bool &calculateFromStartDate);
    void calculateNAVValues(const int &portfolioID);

    const QMap<int, navTradeList> calculateExecutedTrades(const int &portfolioID, const int &calculationDate, const bool &calculateFromStartDate);
    void insertExecutedTrades(const int &portfolioID, const int &date, const snapshotPortfolio &previousInfo, const navTradeList &trades);
    void insertExecutedTradesPreStartDate(const int &portfolioID, const int &startDate, const QMap<int, navTradeList> &allTrades);
    void insertPortfolioReinvestments(const int &portfolioID, const int &date, const QList<int> &securityReinvestments, const snapshotPortfolio &previousInfo);
    void insertPortfolioCashTrade(const int &portfolioID, const int &cashAccount, const snapshotPortfolio &previousInfo, const int &date, const double &reverseTradeValue);

    int checkCalculationDate(const int &portfolioID, int calculationDate, bool *calcuateFromStartDate);
};

#endif // NAV_H
