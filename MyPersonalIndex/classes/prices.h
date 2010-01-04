#ifndef PRICES_H
#define PRICES_H

#include <QtGui>
#include "queries.h"

//sqliteQuery* queries::deleteUnusedPrices(const QString &table)
//{
//    return new sqliteQuery(
//        QString(
//            "DELETE FROM %1"
//            " WHERE Ticker IN (SELECT a.Ticker"
//                            " FROM (SELECT DISTINCT Ticker FROM %1) AS a"
//                            " LEFT JOIN Tickers AS b"
//                                " ON a.Ticker = b.Ticker AND b.CashAccount = 0"
//                            " WHERE b.Ticker IS NULL )").arg(table),
//        QList<sqliteParameter>()
//    );
//}

class prices
{
public:
    static prices& instance()
    {
        static prices m_instance; // Guaranteed to be destroyed and instantiated on first use.
        return m_instance;
    }

    struct securityPrice
    {
        double close;
        double dividend;
        double split;

        securityPrice(): close(0), dividend(0), split(1) {}
        securityPrice(double p_close, double p_dividend, double p_split): close(p_close), dividend(p_dividend), split(p_split) {}
    };

    struct securityPrices
    {
        QMap<int, double> splits;
        QMap<int, double> dividends;
        QMap<int, double> prices;

        double price(const int &date) const { return prices.value(date, 0); }
        double dividend(const int &date) const { return dividends.value(date, 0); }
        double split(const int &date) const { return splits.value(date, 1); }
        securityPrice dailyPriceInfo(const int &date) const { return securityPrice(price(date), dividend(date), split(date)); }
    };

    typedef QHash<QString, securityPrices> securityPriceList;

    QStringList symbols() { return m_securityPriceList.keys(); }
    QList<int> dates() { return m_dates; }

    void insertPrice(const QString &ticker, const int &date, const double &price) { m_securityPriceList[ticker].prices.insert(date, price); insertDate(date);}
    void insertDividend(const QString &ticker, const int &date, const double &dividend) { m_securityPriceList[ticker].dividends.insert(date, dividend); }
    void insertSplit(const QString &ticker, const int &date, const double &split) { m_securityPriceList[ticker].splits.insert(date, split); }

    QMap<int, double> price(const QString &ticker);
    QMap<int, double> dividend(const QString &ticker);
    QMap<int, double> split(const QString &ticker);
    securityPrices history(const QString &ticker) { return m_securityPriceList.value(ticker); }

    double price(const QString &ticker, const int &date) { return history(ticker).price(date); }
    double dividend(const QString &ticker, const int &date) { return history(ticker).dividend(date); }
    double split(const QString &ticker, const int &date) { return history(ticker).split(date); }

    int firstDate() { return m_dates.isEmpty() ? 0 : m_dates.first(); }
    int firstDate(const QString &ticker);

    int lastDate() { return m_dates.isEmpty() ? 0 : m_dates.last(); }
    int lastDate(const QString &ticker);

    securityPrice dailyPriceInfo(const QString &ticker, const int &date) { return history(ticker).dailyPriceInfo(date); }

    void insertCashSecurity(const QString &ticker) { m_cashSecurities.insert(ticker); }
    bool isCashSecurity(const QString &ticker) { return m_cashSecurities.contains(ticker); }

private:
    securityPriceList m_securityPriceList;
    QList<int> m_dates;
    QSet<QString> m_cashSecurities;
    securityPrices m_cashPrices;

    void loadPrices(QSqlQuery);
    void loadDividends(QSqlQuery);
    void loadSplits(QSqlQuery);
    void insertDate(const int &date);

    prices();
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singelton appearing.
    prices(prices const&);  // Don't Implement
    void operator=(prices const&); // Don't implement
};

#endif // PRICES_H
