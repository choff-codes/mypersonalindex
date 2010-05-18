#ifndef PRICES_H
#define PRICES_H

#include <QMap>
#include <QHash>
#include <QSet>
#include <QDate>
#include "securityPrice.h"
#include "queries.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class prices
{
public:
    static prices& instance()
    {
        static prices m_instance; // Guaranteed to be destroyed and instantiated on first use.
        return m_instance;
    }

    QStringList symbols() const { return m_securityPriceList.keys(); }
    bool exists(const QString &symbol) const { return m_securityPriceList.contains(symbol); }
    const QList<int> dates() const { return m_dates; }

    void insertPrice(const QString &symbol, const int &date, const double &price) { m_securityPriceList[symbol].prices.insert(date, price); insertDate(date);}
    void insertDividend(const QString &symbol, const int &date, const double &dividend) { m_securityPriceList[symbol].dividends.insert(date, dividend); }
    void insertSplit(const QString &symbol, const int &date, const double &split) { m_securityPriceList[symbol].splits.insert(date, split); }

    QMap<int, double> price(const QString &symbol) const;
    QMap<int, double> dividend(const QString &symbol) const;
    QMap<int, double> split(const QString &symbol) const;
    securityPrices history(const QString &symbol) const { return isCashSecurity(symbol) ? m_cashPrices : m_securityPriceList.value(symbol); }

    double price(const QString &symbol, const int &date) const { return history(symbol).price(date); }
    double dividend(const QString &symbol, const int &date) const { return history(symbol).dividend(date); }
    double split(const QString &symbol, const int &date) const { return history(symbol).split(date); }

    int firstDate() const { return m_dates.isEmpty() ? 0 : m_dates.first(); }
    int lastDate() const { return m_dates.isEmpty() ? 0 : m_dates.last(); }
    QList<int>::const_iterator iteratorFirstDate() const { return m_dates.constBegin(); }
    QList<int>::const_iterator iteratorLastDate() const { return m_dates.isEmpty() ? m_dates.constEnd() : m_dates.constEnd() - 1; }
    QList<int>::const_iterator iteratorEnd() const { return m_dates.constEnd(); }
    QList<int>::const_iterator iteratorPreviousDate(int date) const { return iteratorCurrentDateOrPrevious(date - 1); }
    QList<int>::const_iterator iteratorCurrentDateOrNext(int date) const { return qLowerBound(m_dates, date); }
    QList<int>::const_iterator iteratorCurrentDateOrPrevious(int date) const;
    int currentDateOrPrevious(int date) const { return *iteratorCurrentDateOrPrevious(date); };

    securityPrice dailyPriceInfo(const QString &symbol, const int &date) const { return history(symbol).dailyPriceInfo(date); }

    void remove(const QStringList &removedSymbols);
    void removeUnusedSymbols(const QStringList &currentSymbols);

    void insertCashSecurity(const QString &symbol) { m_cashSecurities.insert(symbol); }
    void removeCashSecurity(const QString &symbol) { m_cashSecurities.remove(symbol); }
    bool isCashSecurity(const QString &symbol) const { return m_cashSecurities.contains(symbol); }

private:
    QHash<QString, securityPrices> m_securityPriceList;
    QList<int> m_dates;
    QSet<QString> m_cashSecurities;
    securityPrices m_cashPrices;

    void loadPrices();
    void loadDividends();
    void loadSplits();
    void insertDate(const int &date);

    prices();
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singelton appearing.
    prices(prices const&);  // Don't Implement
    void operator=(prices const&); // Don't implement
};

#endif // PRICES_H
