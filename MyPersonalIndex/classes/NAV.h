#ifndef NAV_H
#define NAV_H

#include "globals.h"
#include "queries.h"

class NAV : public QThread
{
    Q_OBJECT

public:
    NAV(const QMap<int, globals::myPersonalIndex*> &data, const QList<int> &dates, const int &calculationDate, QObject *parent = 0, const int &portfolioID = -1):
        QThread(parent), m_data(data), m_dates(dates), m_calculationDate(calculationDate), m_portfolioID(portfolioID)
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "nav");
        db.setDatabaseName(queries::getDatabaseLocation());
        m_sql = new queries(db);
    }

    ~NAV() { delete m_sql; }
    void run();

signals:
    void calculationFinished();
    void statusUpdate(const QString &message);

private:
    queries *m_sql;
    const QMap<int, globals::myPersonalIndex*> &m_data;
    // key is portfolio ID and value islast date calculated
    QMap<int, int> m_portfolioLastDates;
    const QList<int> &m_dates;
    int m_calculationDate;
    int m_portfolioID;

    typedef QPair<QString /* ticker */, int /* tickerID */> tickerReinvestment;
    typedef QList<tickerReinvestment> tickerReinvestmentList;
    typedef QList<globals::dynamicTradeInfo> dynamicTradeList;
    typedef QMap<int, dynamicTradeList> dynamicTrades;
    typedef QPair<double /* total value */, double /* NAV */> portfolioNAV;

    void getPortfolioLastDates();
    portfolioNAV getPortfolioNAV(const int &portfolioID, const int &date);
    void getPortfolioTotalValue(const int &portfolioID, const int &date, double *totalValue = 0, double *dividendValue = 0);
    QMap<QString, globals::securityInfo> getPortfolioTickerInfo(const int &portfolioID, const int &date, const int &previousDay);
    int checkCalculationDate(const int &portfolioID, int calculationDate, bool &calcuateFromStartDate);
    void getPortfolioNAVValues(const int &portfolioID, const int &calculationDate, const bool &portfolioStartDate);
    bool getCurrentDateOrNext(int &date);
    dynamicTrades getPortfolioTrades(const int &portfolioID, const int &minDate, const bool &portfolioStartDate);
    tickerReinvestmentList getPortfolioTickerReinvestment(const int &portfolioID);
    QList<int> getOnceTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate, const bool &portfolioStartDate);
    QList<int> getWeeklyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    QList<int> getMonthlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    QList<int> getYearlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    void insertPortfolioTrades(const int &portfolioID, const int &date, const int &previousDate, const double &previousTotalValue,
        const dynamicTradeList &trades, const QMap<QString, globals::securityInfo> &tickerInfo, const tickerReinvestmentList &tickerReinvestments);
};

#endif // NAV_H
