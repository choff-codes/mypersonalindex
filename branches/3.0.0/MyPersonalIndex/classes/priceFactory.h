#ifndef PRICEFACTORY_H
#define PRICEFACTORY_H

#include <QHash>
#include "historicalPrices.h"
#include "queries.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class priceFactory
{
public:

    static priceFactory& getPriceFactory()
    {
        static priceFactory m_priceFactory; // Guaranteed to be destroyed and instantiated on first use.
        return m_priceFactory;
    }

    historicalPrices getPrices(const QString &symbol_, const queries &dataSource_);
    void refreshCache(const queries &dataSource_);

private:
    QHash<QString, historicalPrices> m_historicalPricesCache;

    priceFactory();
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singelton appearing.
    priceFactory(priceFactory const&);  // Don't Implement
    void operator=(priceFactory const&); // Don't implement

    void updateHistoricalPrices(const QString &symbol_, const historicalPrices &priceData_, const queries &dataSource_);
    QMap<int, double> loadPrices(const queries &dataSource_);
    QMap<int, double> loadDividends(const queries &dataSource_);
    QMap<int, double> loadSplits(const queries &dataSource_);
};

#endif // PRICEFACTORY_H
