#ifndef NAV_H
#define NAV_H

#include "queries.h"
#include "calculations.h"
#include "prices.h"
#include "portfolio.h"
#include "security.h"

class nav : public QThread
{
    Q_OBJECT

public:
    nav(const QMap<int, portfolio*> &data, const int &calculationDate, QObject *parent = 0, const int &portfolioID = -1):
        QThread(parent), m_data(data), m_dates(prices::instance().dates()), m_calculationDate(calculationDate), m_portfolioID(portfolioID) { }

    void run();

signals:
    void calculationFinished();
    void statusUpdate(const QString &message);

private:
    const QMap<int, portfolio*> m_data;
    const QList<int> m_dates;
    int m_calculationDate;
    int m_portfolioID;
    QVariantList m_NAV_Portfolio, m_NAV_Dates, m_NAV_Totalvalue, m_NAV_Nav;
    QVariantList m_ExecutedTrades_SecurityID, m_ExecutedTrades_Dates, m_ExecutedTrades_Shares, m_ExecutedTrades_Price, m_ExecutedTrades_Commission, m_ExecutedTrades_Code;
    calculations m_calculations;

    typedef QMap<int, trade>::const_iterator navTradePointer;
    typedef QMap<int, QList<navTradePointer> > navTradeList;

    navTradeList appendNavTrades(const navTradeList &first, const navTradeList &second);
    void addToNAVList(portfolio *currentPortfolio, const int &date, const double &totalValue, const double &nav);
    void addToExecutedTradeList(portfolio *currentPortfolio, const int &securityID, const int &date, const double &shares, const double &price, const double &commission, const QString &code);
    void clearVariantLists();
    void insertVariantLists();
    void deleteOldValues(portfolio *currentPortfolio, const int &calculationDate, const bool &calculateFromStartDate);
    int checkCalculationDate(const portfolio *currentPortfolio, int calculationDate, bool *calcuateFromStartDate);
    void calculateNAVValues(portfolio *currentPortfolio);
    bool getCurrentDateOrNext(int *date);
    QMap<int, navTradeList> calculateExecutedTrades(const portfolio *currentPortfolio, const int &minDate, const bool &calculateFromStartDate);
    QList<int> getPortfolioSecurityReinvestment(const portfolio *currentPortfolio);
    QList<int> computeOnceTrades(const trade &singleTade, const int &minDate, const int &maxDate, const bool &calculateFromStartDate);
    QList<int> computeWeeklyTrades(const int &tradeDate, const int &minDate, const int &maxDate);
    QList<int> computeMonthlyTrades(const int &tradeDate, const int &minDate, const int &maxDate);
    QList<int> computeYearlyTrades(const int &tradeDate, const int &minDate, const int &maxDate);
    void insertPortfolioReinvestments(portfolio *currentPortfolio, const int &date, const QList<int> &securityReinvestments, const dailyInfoPortfolio *previousInfo);
    void insertPortfolioCashTrade(portfolio *currentPortfolio, const int &cashAccount, const dailyInfoPortfolio *previousInfo, const int &date, const double &reverseTradeValue);
    void insertPortfolioTrades(portfolio *currentPortfolio, const int &date, const dailyInfoPortfolio *previousInfo, const navTradeList &trades);
    void insertFirstPortfolioTrades(portfolio *currentPortfolio, const int &startDate, const QMap<int, navTradeList> &allTrades);
};

#endif // NAV_H
