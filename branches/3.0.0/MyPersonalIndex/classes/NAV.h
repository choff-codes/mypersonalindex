#ifndef NAV_H
#define NAV_H

#include "globals.h"
#include "queries.h"

class NAV : public QThread
{
    Q_OBJECT

public:
    NAV(const QMap<int, globals::myPersonalIndex*> &data, const QList<int> &dates, const globals::splitData &splits, const int &calculationDate, QObject *parent = 0, const int &portfolioID = -1):
        QThread(parent), m_data(data), m_dates(dates), m_splits(splits), m_calculationDate(calculationDate), m_portfolioID(portfolioID)
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "nav");
        db.setDatabaseName(queries::getDatabaseLocation());
        m_sql = new queries(db);
    }

    ~NAV() { delete m_sql; QSqlDatabase::removeDatabase("nav"); }
    void run();

signals:
    void calculationFinished();
    void statusUpdate(const QString &message);

private:
    queries *m_sql;
    const QMap<int, globals::myPersonalIndex*> &m_data;
    const QList<int> &m_dates;
    const globals::splitData &m_splits;
    int m_calculationDate;
    int m_portfolioID;
    QVariantList m_NAV_Portfolio, m_NAV_Dates, m_NAV_Totalvalue, m_NAV_Nav;
    QVariantList m_Trades_Portfolio, m_Trades_TickerID, m_Trades_Dates, m_Trades_Shares, m_Trades_Price, m_Trades_Commission, m_Trades_Code;
    int m_TradesPosition;

    typedef QList<globals::dynamicTradeInfo> dynamicTradeList;
    typedef QMap<int, dynamicTradeList> dynamicTrades;

    void clearVariantLists();
    void insertVariantLists();
    double getPortfolioNAV(const int &portfolioID, const int &date, double *totalValue = 0);
    void deleteOldValues(globals::myPersonalIndex *currentPortfolio, const int &calculationDate, const bool &portfolioStartDate);
    double getPortfolioTotalValue(const globals::myPersonalIndex *currentPortfolio, const QMap<QString, globals::securityInfo> &tickerInfo, const int &date, double *dividendValue = 0);
    double getPortfolioDailyActivity(const int &portfolioID, const int &date, const QMap<QString, globals::securityInfo> &tickerInfo);
    int checkCalculationDate(const int &portfolioID, int calculationDate, bool &calcuateFromStartDate);
    void getPortfolioNAVValues(const int &portfolioID, const int &calculationDate, const bool &portfolioStartDate);
    bool getCurrentDateOrNext(int &date);
    dynamicTrades getPortfolioTrades(const int &portfolioID, const int &minDate, const bool &portfolioStartDate);
    QList<int> getPortfolioTickerReinvestment(const int &portfolioID);
    QList<int> getOnceTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate, const bool &portfolioStartDate);
    QList<int> getWeeklyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    QList<int> getMonthlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    QList<int> getYearlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    void insertPortfolioTrades(const int &portfolioID, const int &date, const int &previousDate, const double &previousTotalValue,
        const dynamicTradeList &trades, const QMap<QString, globals::securityInfo> &tickerInfo, const QList<int> &tickerReinvestments);
};

#endif // NAV_H
