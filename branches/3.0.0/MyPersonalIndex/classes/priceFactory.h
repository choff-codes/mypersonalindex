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

    static historicalPrices getPrices(const QString &symbol_, const queries &dataSource_);
    static void flagDirty(const queries &dataSource_);
    static void close(const queries &dataSource_) { m_historicalPricesCache.remove(dataSource_); }

private:
    static QHash<queries, QHash<QString, historicalPrices> > m_historicalPricesCache;

    static void updateHistoricalPrices(const QString &symbol_, historicalPrices priceData_, const queries &dataSource_);
    static QMap<int, double> loadPrices(const QString &symbol_, const queries &dataSource_);
    static QMap<int, double> loadDividends(const QString &symbol_, const queries &dataSource_);
    static QMap<int, double> loadSplits(const QString &symbol_, const queries &dataSource_);
};

#endif // PRICEFACTORY_H
