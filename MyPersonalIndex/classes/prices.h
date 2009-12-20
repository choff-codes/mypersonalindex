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

    enum query_Type { query_Price, query_Dividend, query_Split };

    const securityPriceList* priceList() { return &m_securityPriceList; }
    static QStringList symbols() { return instance().priceList()->keys(); }

    static double price(const QString &ticker, const int &date) { return instance().priceList()->value(ticker).prices.value(date, 0); }
    static double dividend(const QString &ticker, const int &date) { return instance().priceList()->value(ticker).dividends.value(date, 0); }
    static double split(const QString &ticker, const int &date) { return instance().priceList()->value(ticker).splits.value(date, 1); }

    static QMap<int, double> price(const QString &ticker) { return instance().priceList()->value(ticker).prices; }
    static QMap<int, double> dividend(const QString &ticker) { return instance().priceList()->value(ticker).dividends; }
    static QMap<int, double> split(const QString &ticker) { return instance().priceList()->value(ticker).splits; }

    static QList<int> dates() { return instance().getDates(); }

    QList<int> getDates() { return m_dates; }
    void insertPrice(const QString &ticker, const int &date, const double &price) { m_securityPriceList[ticker].prices.insert(date, price); insertDate(date);}
    void insertDividend(const QString &ticker, const int &date, const double &dividend) { m_securityPriceList[ticker].dividends.insert(date, dividend); }
    void insertSplit(const QString &ticker, const int &date, const double &split) { m_securityPriceList[ticker].splits.insert(date, split); }

    static void addPrice(const QString &ticker, const int &date, const double &price) { instance().insertPrice(ticker, date, price); }
    static void addDividend(const QString &ticker, const int &date, const double &dividend) { instance().insertDividend(ticker, date, dividend); }
    static void addSplit(const QString &ticker, const int &date, const double &split) { instance().insertSplit(ticker, date, split); }

    static securityPrice dailyPriceInfo(const QString &ticker, const int &date);

private:
    securityPriceList m_securityPriceList;
    QList<int> m_dates;

    void loadPrices(query_Type type, QSqlQuery *q);
    void insertDate(const int &date);

    prices();
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singelton appearing.
    prices(prices const&);  // Don't Implement
    void operator=(prices const&); // Don't implement
};

#endif // PRICES_H
