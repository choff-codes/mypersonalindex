#ifndef PRICES_H
#define PRICES_H

#include <QtGui>
#include "queries.h"

class prices
{
public:
    static prices& instance()
    {
        static prices m_instance; // Guaranteed to be destroyed and instantiated on first use.
        return m_instance;
    }

    struct securityPrices
    {
        QMap<int, double> splits;
        QMap<int, double> dividends;
        QMap<int, double> prices;
    };

    struct securityPrice
    {
        double close;
        double dividend;
        double split;

        securityPrice(): close(0), dividend(0), split(1) {}
        securityPrice(double p_close, double p_dividend, double p_split): close(p_close), dividend(p_dividend), split(p_split) {}
    };

    typedef QHash<QString, securityPrices> securityPriceList;

    securityPriceList priceList() { return m_securityPriceList; }

    QStringList symbols() { return m_securityPriceList.keys(); }
    QList<int> dates() { return m_dates; }

    void insertPrice(const QString &ticker, const int &date, const double &price) { m_securityPriceList[ticker].prices.insert(date, price); insertDate(date);}
    void insertDividend(const QString &ticker, const int &date, const double &dividend) { m_securityPriceList[ticker].dividends.insert(date, dividend); }
    void insertSplit(const QString &ticker, const int &date, const double &split) { m_securityPriceList[ticker].splits.insert(date, split); }

    QMap<int, double> price(const QString &ticker);
    QMap<int, double> dividend(const QString &ticker);
    QMap<int, double> split(const QString &ticker);

    double price(const QString &ticker, const int &date) { return price(ticker).value(date, 1); }
    double dividend(const QString &ticker, const int &date) { return dividend(ticker).value(date, 0); }
    double split(const QString &ticker, const int &date) { return split(ticker).value(date, 1); }

    int firstDate() { return m_dates.isEmpty() ? 0 : m_dates.first(); }
    int firstDate(const QString &ticker);

    int lastDate() { return m_dates.isEmpty() ? 0 : m_dates.last(); }
    int lastDate(const QString &ticker);

    securityPrice dailyPriceInfo(const QString &ticker, const int &date) { return securityPrice(price(ticker, date), dividend(ticker, date), split(ticker, date)); }

private:
    securityPriceList m_securityPriceList;
    QList<int> m_dates;
    QSet<QString> m_cashSecurities;
    securityPrices m_cashPrices;

    enum query_Type { query_Price, query_Dividend, query_Split };

    void loadPrices(query_Type type, QSqlQuery *q);
    void loadCashSecurities(QSqlQuery *q);
    void insertDate(const int &date);

    prices();
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singelton appearing.
    prices(prices const&);  // Don't Implement
    void operator=(prices const&); // Don't implement
};

#endif // PRICES_H
