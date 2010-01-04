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
        QThread(parent), m_data(data), m_dates(prices::instance().dates()), m_calculationDate(calculationDate), m_portfolioID(portfolioID), m_TradesPosition(0) { }

    void run();

signals:
    void calculationFinished();
    void statusUpdate(const QString &message);

private:
    const QMap<int, portfolio*> &m_data;
    const QList<int> m_dates;
    int m_calculationDate;
    int m_portfolioID;
    QVariantList m_NAV_Portfolio, m_NAV_Dates, m_NAV_Totalvalue, m_NAV_Nav;
    QVariantList m_Trades_TickerID, m_Trades_Dates, m_Trades_Shares, m_Trades_Price, m_Trades_Commission, m_Trades_Code;
    int m_TradesPosition;
    calculations m_calculations;

    struct dynamicTradeInfo
    {
        QString ticker;
        int tickerID;
        ::trade trade;

        dynamicTradeInfo(const QString &p_ticker, const int &p_tickerID, const ::trade &p_trade): ticker(p_ticker), tickerID(p_tickerID), trade(p_trade) {}
    };

    typedef QList<dynamicTradeInfo> dynamicTradeList;
    typedef QMap<int, dynamicTradeList> dynamicTrades;

    void clearVariantLists();
    void insertVariantLists();
    void deleteOldValues(portfolio *currentPortfolio, const int &calculationDate, const bool &portfolioStartDate);
    int checkCalculationDate(const int &portfolioID, int calculationDate, bool &calcuateFromStartDate);
    void getPortfolioNAVValues(const int &portfolioID, const int &calculationDate, const bool &portfolioStartDate);
    bool getCurrentDateOrNext(int &date);
    dynamicTrades getPortfolioTrades(const int &portfolioID, const int &minDate, const bool &portfolioStartDate);
    QList<int> getPortfolioTickerReinvestment(const int &portfolioID);
    QList<int> getOnceTrades(const trade &d, const int &minDate, const int &maxDate, const bool &portfolioStartDate);
    QList<int> getWeeklyTrades(const trade &d, const int &minDate, const int &maxDate);
    QList<int> getMonthlyTrades(const trade &d, const int &minDate, const int &maxDate);
    QList<int> getYearlyTrades(const trade &d, const int &minDate, const int &maxDate);
    void insertPortfolioReinvestments(const portfolio *currentPortfolio, const int &date, const QList<int> &tickerReinvestments, const calculations::portfolioDailyInfo *previousInfo);
    void insertPortfolioCashTrade(const portfolio *currentPortfolio, const int &cashAccount, const calculations::portfolioDailyInfo *previousInfo, const int &date, const double &reverseTradeValue);
    void insertPortfolioTrades(const portfolio *currentPortfolio, const int &date, const calculations::portfolioDailyInfo *previousInfo, const dynamicTradeList &trades);
    void insertFirstPortfolioTrades(const portfolio *currentPortfolio, const int &startDate, const dynamicTrades &allTrades);
    void insertPortfolioTradesToObject(portfolio *currentPortfolio);
};

#endif // NAV_H
