#ifndef NAV_H
#define NAV_H

#include "globals.h"
#include "queries.h"
#include "calculations.h"

class NAV : public QThread
{
    Q_OBJECT

public:
    NAV(const QMap<int, globals::myPersonalIndex*> &data, const QList<int> &dates, const int &calculationDate, QObject *parent = 0, const int &portfolioID = -1):
        QThread(parent), m_sql(new queries("nav")), m_data(data), m_dates(dates), m_calculationDate(calculationDate), m_portfolioID(portfolioID), m_TradesPosition(0) { }

    ~NAV() { delete m_sql; }
    void run();

signals:
    void calculationFinished();
    void statusUpdate(const QString &message);

private:
    queries *m_sql;
    const QMap<int, globals::myPersonalIndex*> &m_data;
    const QList<int> &m_dates;
    int m_calculationDate;
    int m_portfolioID;
    QVariantList m_NAV_Portfolio, m_NAV_Dates, m_NAV_Totalvalue, m_NAV_Nav;
    QVariantList m_Trades_TickerID, m_Trades_Dates, m_Trades_Shares, m_Trades_Price, m_Trades_Commission, m_Trades_Code;
    int m_TradesPosition;
    calculations m_calculations;

    typedef QList<globals::dynamicTradeInfo> dynamicTradeList;
    typedef QMap<int, dynamicTradeList> dynamicTrades;

    void clearVariantLists();
    void insertVariantLists();
    void deleteOldValues(globals::myPersonalIndex *currentPortfolio, const int &calculationDate, const bool &portfolioStartDate);
    int checkCalculationDate(const int &portfolioID, int calculationDate, bool &calcuateFromStartDate);
    void getPortfolioNAVValues(const int &portfolioID, const int &calculationDate, const bool &portfolioStartDate);
    bool getCurrentDateOrNext(int &date);
    dynamicTrades getPortfolioTrades(const int &portfolioID, const int &minDate, const bool &portfolioStartDate);
    QList<int> getPortfolioTickerReinvestment(const int &portfolioID);
    QList<int> getOnceTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate, const bool &portfolioStartDate);
    QList<int> getWeeklyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    QList<int> getMonthlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    QList<int> getYearlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    void insertPortfolioReinvestments(const globals::myPersonalIndex *currentPortfolio, const int &date, const QList<int> &tickerReinvestments, const calculations::portfolioDailyInfo *previousInfo);
    void insertPortfolioCashTrade(const globals::myPersonalIndex *currentPortfolio, const int &cashAccount, const calculations::portfolioDailyInfo *previousInfo, const int &date, const double &reverseTradeValue);
    void insertPortfolioTrades(const globals::myPersonalIndex *currentPortfolio, const int &date, const calculations::portfolioDailyInfo *previousInfo, const dynamicTradeList &trades);
    void insertFirstPortfolioTrades(const globals::myPersonalIndex *currentPortfolio, const int &startDate, const dynamicTrades &allTrades);
    void insertPortfolioTradesToObject(globals::myPersonalIndex *currentPortfolio);
};

#endif // NAV_H
