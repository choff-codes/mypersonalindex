#ifndef NAV_H
#define NAV_H

#include "globals.h"
#include "queries.h"

class NAV : public QThread
{
    Q_OBJECT

public:
    NAV(QMap<int, globals::myPersonalIndex*> *data, QList<int> *dates, const int &lastDate, QObject *parent = 0, const int &portfolioID = -1):
        QThread(parent), m_data(data), m_dates(dates), m_lastDate(lastDate), m_portfolioID(portfolioID)
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "nav");
        db.setDatabaseName(queries::getDatabaseLocation());
        m_sql = new queries(db);
    }

    ~NAV() { delete m_sql; }

signals:
    void calculationFinished();

private:
    queries *m_sql;
    QMap<int, globals::myPersonalIndex*> *m_data;
    // key is portfolio ID and value islast date calculated
    QMap<int, int> m_portfolioLastDates;
    QList<int> *m_dates;
    int m_lastDate;
    int m_portfolioID;

    typedef QList<QPair<QString, int> > tickerReinvestment;
    typedef QList<globals::dynamicTradeInfo> dynamicTradeList;
    typedef QMap<int, dynamicTradeList> dynamicTrades;

    void run();
    void getPortfolioLastDates();
    QPair<double /* total value */, double /* NAV */> getPortfolioNAV(const int &portfolioID, const int &date);
    void getPortfolioTotalValue(const int &portfolioID, const int &date, double *totalValue = 0, double *dividendValue = 0);
    QMap<QString, globals::securityInfo> getPortfolioTickerInfo(const int &portfolioID, const int &date, const int &previousDay);
    int checkCalculationDate(const int &portfolioID, int calculationDate, bool &calcuateFromStartDate);
    void getPortfolioNAVValues(const int &portfolioID, const int &calculationDate, const bool &portfolioStartDate);
    bool getCurrentDateOrNext(int &date);
    dynamicTrades getPortfolioTrades(const int &portfolioID, const int &minDate);
    tickerReinvestment getPortfolioTickerReinvestment(const int &portfolioID);
    QList<int> getOnceTrades(const globals::dynamicTrade &d);
    QList<int> getWeeklyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    QList<int> getMonthlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    QList<int> getYearlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate);
    void insertPortfolioTrades(globals::myPersonalIndex *portfolio, const int &date, const double &previousTotalValue,
        const dynamicTradeList &trades, const QMap<QString, globals::securityInfo> &tickerInfo, const tickerReinvestment &tickerReinvestments);
};

#endif // NAV_H
